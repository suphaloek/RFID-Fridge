#include "HX711.h"
#include "dw_font.h"
#include <TFT_eSPI.h>   
#include <WiFi.h>
#include "FirebaseESP32.h"
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ArduinoJson.h>

TaskHandle_t Task1,Task2,Task3;
SemaphoreHandle_t xMutex;

Adafruit_PN532 nfc(17, 16, 5, 0);//(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS)

extern dw_font_info_t font_th_sarabunpsk_regular34;
extern dw_font_info_t font_th_sarabunpsk_regular40;
dw_font_t myfont;
TFT_eSPI tft = TFT_eSPI();   

// Config Firebase
#define FIREBASE_HOST "testdata-79638.firebaseio.com"
#define FIREBASE_AUTH "4W713StGVhnm8LwRaCpZJTfLixgRMByFMvCEhVsG"
// Config connect WiFi
#define WIFI_SSID "DESKTOP-UQULUGG 9584"
#define WIFI_PASSWORD "X6a1982!"

/////////////////////////////////////////////////////////////////////////////

HX711 scale1(25, 26);//DOUT,CLK
HX711 scale2(14, 27);//DOUT,CLK

String w1,w2;
unsigned int set_scale1,set_scale2;
float Weight_per_piece1=0,Weight_per_piece2=0;
float p=0,f,pweight1=0.0,pweight2=0.0;
float weight1=0.1,weight2 = 0,a=0;
int s=0,time1=0;
int wpiece1 = 0,wpiece2 = 0;
int pwpiece1 = 0,pwpiece2 = 0;
/////////////////////////////////////////////////////////////////////////////

float asd;
unsigned char row2[100];
unsigned char qw,wq,hg=0,ph=0,ph1=0,kl=0,sound=0,lock=0,SetLCD=0;
String str,chRfid[40][40]={};
int hj;
unsigned char i = 0, u = 0, tt = 0,Key;
unsigned char num = 0,numpr = 0, rfidrow = 0, piece = 0, remain = 0;
char buf[100];
unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 1000;           // interval at which to blink (milliseconds)
unsigned long currentMillis;

uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
String scannedUID = ""; // this is where we'll store the scanned tag's UID

void Firebase_test();
boolean rfidread();
void draw_pixel(int16_t x, int16_t y);
void clear_pixel(int16_t x, int16_t y);
void Task2code( void * pvParameters );
void Lcdprint(String Data,unsigned char Set,boolean Reset);
void Lcdprint1(String Data,int x,int y);

WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
NTPClient timeClient(ntpUDP, "time.nist.gov", 25200, 60000);

FirebaseData firebaseData;
DynamicJsonDocument doc(1024);

void setup(void) { 
  Serial.begin(115200); 
  pinMode(33, OUTPUT);
  digitalWrite(33, HIGH); 
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  nfc.setPassiveActivationRetries(0xFF);
  nfc.SAMConfig();  
  Serial.println("Waiting for an ISO14443A card");
  
  xMutex = xSemaphoreCreateMutex();
  Serial.println("Hello!");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("%connected: ");
  Serial.println(WiFi.localIP());


  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  dw_font_init(&myfont,
               480,
               320,
               draw_pixel,
               clear_pixel);

  dw_font_setfont(&myfont, &font_th_sarabunpsk_regular34);
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  Firebase.setMaxRetry(firebaseData, 3);
  
  Firebase.getInt(firebaseData,"Esp32/set_scale1");
  set_scale1 = firebaseData.intData();
  Firebase.getInt(firebaseData,"Esp32/set_scale2");
  set_scale2 = firebaseData.intData();
  delay(1);
  Serial.println(set_scale1);
  Serial.println(set_scale2);
  scale1.set_gain(64);
  scale1.set_scale(set_scale1);
  scale1.tare(); //รีเซตน้ำหนักเป็น 0
  scale2.set_gain(64);
  scale2.set_scale(set_scale2);
  scale2.tare(); //รีเซตน้ำหนักเป็น 0
  
  Lcdprint("กำลังโหลดข้อมูล...",2,1);

  Firebase.getInt(firebaseData,"numrow/num");
  num = firebaseData.intData();
  for (int u = 1; u <= num; u++) {
    String Number =  String(u, DEC);
    Firebase.getInt(firebaseData,"DB-RFID/"+Number+"/piece");
    int row = firebaseData.intData();
    row2[u] = row;
    if(row > 0){
    Firebase.getJSON(firebaseData,"DB-RFID/"+Number+"/RFID");
    deserializeJson(doc, firebaseData.jsonData()); 
    for (int z = 1; z <= row; z++) {     
      String hh = doc[String(z, DEC)];
      chRfid[u][z] = hh;   
      }
    }
  }
  Firebase.getInt(firebaseData,"Loadcell/1/Weight_per_piece");
  Weight_per_piece1 = firebaseData.intData();
  Firebase.getInt(firebaseData,"Loadcell/2/Weight_per_piece");
  Weight_per_piece2 = firebaseData.intData();
  Weight_per_piece1 = round((Weight_per_piece1/1000)*100)/100;
  Weight_per_piece2 = round((Weight_per_piece2/1000)*100)/100;
  
  for (int u = 1; u <= num; u++) {
        Serial.print(u);
              for (int z = 1; z <= row2[u]; z++) {         
                Serial.print(" "+chRfid[u][z]);
   }
   Serial.println("");
   } 

  Lcdprint("พร้อมรอรับการสแกน Tag RFID",2,1);                         
  xTaskCreatePinnedToCore(
    Task2code, /* Function to implement the task */
    "Task2", /* Name of the task */
    6000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task2,  /* Task handle. */
    0); /* Core where the task should run */
    delay(10);
    
    xTaskCreatePinnedToCore(
    Task1code, /* Function to implement the task */
    "Task1", /* Name of the task */
    7000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    0,  /* Priority of the task */
    &Task1,  /* Task handle. */
    0); /* Core where the task should run */
    delay(10);

    xTaskCreatePinnedToCore(
    Task3code, /* Function to implement the task */
    "Task3", /* Name of the task */
    2000,  /* Stack size in words */
    NULL,  /* Task input parameter */
    1,  /* Priority of the task */
    &Task3,  /* Task handle. */
    0); /* Core where the task should run */
    delay(10);
}

void draw_pixel(int16_t x, int16_t y){
  if(SetLCD == 0)tft.drawPixel(x, y, TFT_WHITE);
  else tft.drawPixel(x, y, TFT_GREEN);
}
void clear_pixel(int16_t x, int16_t y){
}

void Task1code( void * pvParameters ) {
  xSemaphoreTake( xMutex, portMAX_DELAY );
  delay(1);
  Firebase.getString(firebaseData,"Loadcell/1/Name");
  w1 = firebaseData.stringData();
  Firebase.getString(firebaseData,"Loadcell/2/Name");
  w2 = firebaseData.stringData();

  tft.fillRect(80, 226, 140 , 30, TFT_BLACK);//TFT_BLACK
  tft.fillRect(135, 260, 50 , 25, TFT_BLACK);
  
  tft.fillRect(320, 226, 140 , 30, TFT_BLACK);//TFT_BLACK
  tft.fillRect(375, 260, 50 , 25, TFT_BLACK);
  SetLCD = 1;
  Lcdprint1("ตะกร้า 1",50,225);
  SetLCD = 0;
  Lcdprint1(String("ชื่อ "+w1),50,255);
  Lcdprint1(String("จำนวน = " + String(ph)),50,280);
  SetLCD = 1;
  Lcdprint1("ตะกร้า 2",290,225);
  SetLCD = 0;
  Lcdprint1(String("ชื่อ "+w2),290,255);
  Lcdprint1(String("จำนวน = " + String(ph1)),290,280);
  xSemaphoreGive( xMutex ); 
  
  while(1){
    weight1 = scale1.get_units(), 2;
    weight2 = scale2.get_units(), 2;

    wpiece1 = abs(round(weight1/Weight_per_piece1));
    wpiece2 = abs(round(weight2/Weight_per_piece2));
    
    weight1 = round(weight1*100)/100;
    weight2 = round(weight2*100)/100;

    if(wpiece1 != pwpiece1){//pwpiece
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.getString(firebaseData,"Loadcell/1/Name");
      w1 = firebaseData.stringData();
      Firebase.getInt(firebaseData,"Loadcell/1/Weight_per_piece");
      Weight_per_piece1 = firebaseData.intData();
      Weight_per_piece1 = round((Weight_per_piece1/1000)*100)/100;
      if(wpiece1 <= 100){
      Firebase.setFloat(firebaseData,"Loadcell/1/weight", weight1);
      Firebase.setInt(firebaseData,"Loadcell/1/piece", wpiece1);
      }
      tft.fillRect(80, 226, 140 , 40, TFT_BLACK);//TFT_BLACK
      tft.fillRect(115, 260, 80 , 25, TFT_BLACK);
      Lcdprint1(String("ชื่อ "+w1),50,255);
      Lcdprint1(String("จำนวน = " + String(wpiece1)),50,280);
      Firebase.getInt(firebaseData,"Esp32/reset_scale");
      if(firebaseData.intData() == 1){
        scale1.tare();
        Firebase.setInt(firebaseData,"Esp32/reset_scale",0);
      }
      xSemaphoreGive( xMutex ); 
      pwpiece1 = wpiece1;
    }

    if(wpiece2 != pwpiece2){
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.getString(firebaseData,"Loadcell/2/Name");
      w2 = firebaseData.stringData();
      Firebase.getInt(firebaseData,"Loadcell/2/Weight_per_piece");
      Weight_per_piece2 = firebaseData.intData();
      Weight_per_piece2 = round((Weight_per_piece2/1000)*100)/100;
      if(wpiece2 <= 100){
      Firebase.setFloat(firebaseData,"Loadcell/2/weight", weight2);
      Firebase.setInt(firebaseData,"Loadcell/2/piece", wpiece2);
      }
      tft.fillRect(320, 226, 140 , 40, TFT_BLACK);//TFT_BLACK
      tft.fillRect(355, 260, 80 , 25, TFT_BLACK);
      Lcdprint1(String("ชื่อ "+w2),290,255);
      Lcdprint1(String("จำนวน = " + String(wpiece2)),290,280);
      Firebase.getInt(firebaseData,"Esp32/reset_scale");
      if(firebaseData.intData() == 1){
        scale2.tare();
        Firebase.setInt(firebaseData,"Esp32/reset_scale",0);
      }
      xSemaphoreGive( xMutex ); 
      pwpiece2 = wpiece2;
    }

    xSemaphoreTake( xMutex, portMAX_DELAY );
    delay(1);
    tft.fillRect(140, 290,80 , 25, TFT_BLACK);
    Lcdprint1(String("น้ำหนัก = " + String(weight1, 2)),50,310);
    
    tft.fillRect(380, 290, 80 , 25, TFT_BLACK);
    Lcdprint1(String("น้ำหนัก = " + String(weight2, 2)),290,310);

    xSemaphoreGive( xMutex );   
    delay(300);  
  }
}

void Task2code( void * pvParameters ) {
  
  while(1) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      timeClient.update();
      tt++;
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);      
      Lcdprint(timeClient.getFormattedTime(),0,1);   
      if(WiFi.RSSI() > -60){
        Lcdprint(("สัญญาณ: "+String("ดี")),1,1);
      }
      else if(WiFi.RSSI() <= -60 && WiFi.RSSI() >=-70){
        Lcdprint(("สัญญาณ: "+String("ปานกลาง")),1,1);
      }
      else if(WiFi.RSSI() < -70){
        Lcdprint(("สัญญาณ: "+String("น้อย")),1,1);
      }

      Firebase.getInt(firebaseData,"SendKey/Key");
      Key = firebaseData.intData();
      String Num =  String(Key, DEC);

      if(Key != numpr){
        lock = 0;
        Firebase.getString(firebaseData,"DB-RFID/"+Num+"/Status");
        String temp = firebaseData.stringData();
        if(lock == 0 && temp == "w"){
          numpr = Key;
          Firebase.getString(firebaseData,"DB-RFID/"+Num+"/Name");
          String temp = firebaseData.stringData();
          tft.fillRect(0, 33, 480, 162, TFT_BLACK);
          Lcdprint1("โปรดนำ ",100,80);
          SetLCD = 1;
          dw_font_setfont(&myfont, &font_th_sarabunpsk_regular40);
          Lcdprint1(temp,180,80);
          dw_font_setfont(&myfont, &font_th_sarabunpsk_regular34);
          SetLCD = 0;
          Lcdprint1(" มาสแกน Tag RFID",90,110);
          tt = 0;
          lock = 1;
        }
      }
      xSemaphoreGive( xMutex );
      
    }

    if(tt == 10 && hg == 0){
      scale1.tare();
      scale2.tare();
      hg=1;
    }
    
    if(tt == 30 && lock == 0){
            
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.setBool(firebaseData,"truth",true);
      Lcdprint("พร้อมรอรับการสแกน Tag RFID",2,1);
      xSemaphoreGive( xMutex );
      tt=0;
      
    }
  }
}

void Task3code( void * pvParameters ) {
  while(1){
  delay(10);
    if(sound == 1){
        fxsound(LOW,200);
        digitalWrite(33, HIGH);
      }
    else if(sound == 2){
      fxsound(LOW,200);
      fxsound(HIGH,100);
      fxsound(LOW,200);
      digitalWrite(33, HIGH);
    }
    else if(sound == 3){
      fxsound(LOW,600);
      digitalWrite(33, HIGH);
    }  
    sound = 0;  
  }
}

void loop(void) {
  boolean success;
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    str = "";
    for (uint8_t i = 0; i < uidLength; i++)
    {
      str += String(uid[i], HEX);
      str += " ";

    }
    xSemaphoreTake( xMutex, portMAX_DELAY );
    delay(1);
    Serial.println(str);
    sound = 1;
    Lcdprint("พบ RFID",2,1);  
    xSemaphoreGive( xMutex );
    Firebase_test();
  } 
}


void Firebase_test() {
  xSemaphoreTake( xMutex, portMAX_DELAY );
  delay(1);
  Firebase.getInt(firebaseData,"SendKey/Key");
  Key = firebaseData.intData();
  Firebase.getInt(firebaseData,"numrow/num");
  num = firebaseData.intData();
  xSemaphoreGive( xMutex );
  String Number =  String(Key, DEC);
  
  if(Key == 0){
    lock = 0;
    xSemaphoreTake( xMutex, portMAX_DELAY );
    delay(1);
    Lcdprint("ทำการนำวัตถุดิบออก",2,1); 
    xSemaphoreGive( xMutex );
     qw = 0;
     wq = 0;
     for (int u = 1; u <= num; u++) {
      for (int z = 1; z <= row2[u]; z++) {
        if (str.equals(chRfid[u][z])) {
          qw = u;
          wq = z;
          break;
        }
      }
     } 
      String Number =  String(qw, DEC);
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.getJSON(firebaseData,"DB-RFID/"+Number);
      deserializeJson(doc, firebaseData.jsonData());               
      xSemaphoreGive( xMutex );
      
      String RFID = doc["RFID"][String(wq, DEC)]; 
      Serial.println(RFID);  
        if (chRfid[qw][wq].equals(RFID) && qw != 0 && wq != 0) {
          const char* Temp = doc["timeKey"];
          String date = ("วันที่ : " + String(Temp));
          Temp = doc["expitem"];
          String exp = ("วันหมดอายุ : " + String(Temp));
          Temp = doc["Name"];
          String name = ("ชื่อ : " + String(Temp));       
          int piece = doc["RFID"]["row"];    
          int history = doc["history"];
          
          xSemaphoreTake( xMutex, portMAX_DELAY );
          delay(1);
          Firebase.deleteNode(firebaseData,"DB-RFID/" + Number + "/RFID/" + String(wq, DEC));
          Firebase.getInt(firebaseData,"History/"+String(history, DEC)+"/export");          
          int Export = firebaseData.intData();
          xSemaphoreGive( xMutex );        
          chRfid[qw][wq] = "";          
          piece--;
          Export++;
          xSemaphoreTake( xMutex, portMAX_DELAY );
          delay(1);        
          Firebase.setInt(firebaseData,"DB-RFID/" + Number + "/RFID/" + "row", piece);
          Firebase.setInt(firebaseData,"History/"+String(history, DEC)+"/export", Export);
          xSemaphoreGive( xMutex );
          String pieced = ("จำนวนวัตถุดิบคงเหลือ : " + String(piece, DEC));
          
          sound = 2;
          xSemaphoreTake( xMutex, portMAX_DELAY );
          delay(1);
          SetLCD = 1;
          Lcdprint(name,2,1);
          SetLCD = 0;
          Lcdprint(pieced,2,0);
          xSemaphoreGive( xMutex );
          tt = 0;
          return;
        }
          else{
            xSemaphoreTake( xMutex, portMAX_DELAY );
            delay(1);
            sound = 3;
            Lcdprint("ไม่พบข้อมล RFID",2,1);
            xSemaphoreGive( xMutex );
            tt = 0;          
          }
  }
  else if(Key > 0){
    xSemaphoreTake( xMutex, portMAX_DELAY );
    delay(1); 
    Lcdprint("ทำการเพิ่มวัตถุดิบ",2,1); 
    Firebase.getString(firebaseData,"DB-RFID/"+Number+"/Status");
    String Status = firebaseData.stringData();   
    xSemaphoreGive( xMutex );
    if (Status == "w"){
      for (int u = 1; u <= num; u++) {
        for (int z = 1; z <= row2[u]; z++) {
          if (str.equals(chRfid[u][z])) {
            xSemaphoreTake( xMutex, portMAX_DELAY );
            delay(1);
            sound = 3; 
            Lcdprint("รหัส RFID นี้อยู่ในระบบแล้ว   ",2,1); 
            xSemaphoreGive( xMutex );
            tt = 0;
            return;
          }
        }
      }
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.getJSON(firebaseData,"DB-RFID/"+Number); 
      deserializeJson(doc, firebaseData.jsonData());  
      xSemaphoreGive( xMutex );     
      piece = doc["piece"];
      rfidrow = doc["RFID"]["row"];

      rfidrow++;
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);      
      Firebase.setString(firebaseData,"DB-RFID/" + Number + "/RFID/" + rfidrow, str);
      Firebase.setInt(firebaseData,"DB-RFID/" + Number + "/RFID/" + "row", rfidrow);
      xSemaphoreGive( xMutex );
      chRfid[Key][rfidrow] = str;
      row2[Key] = rfidrow;
      
      if (rfidrow == piece) {
        xSemaphoreTake( xMutex, portMAX_DELAY );
        delay(1);
        lock = 0;
        Firebase.setInt(firebaseData,"SendKey/Key",0);      
        Firebase.setString(firebaseData,"DB-RFID/" + Number + "/Status", "D");    
        xSemaphoreGive( xMutex );    
      } 
      const char* Temp = doc["timeKey"];
      String date = ("วันที่ : " + String(Temp));
      Temp = doc["expitem"];
      String expd = ("วันหมดอายุ : " + String(Temp));
      Temp = doc["Name"];
      String named = ("ชื่อ : " + String(Temp));
      String pieced = ("จำนวนวัตถุดิบ : " + String(piece, DEC));
      String rfidrowd = ("จำนวนวัตถุดิบที่ทำการเพิ่ม : " + String(rfidrow, DEC));
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      sound = 2;
      SetLCD = 1;
      Lcdprint(named,2,1);
      SetLCD = 0;
      Lcdprint(expd,2,0);
      Lcdprint(pieced,2,0);
      Lcdprint(rfidrowd,2,0);
      xSemaphoreGive( xMutex );
       tt = 0;
      return;
    }
    else if(Status == "D"){
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      Firebase.getString(firebaseData,"DB-RFID/"+Number+"/Name");
      String named = ("ชื่อ : " + firebaseData.stringData());
      sound = 3;
      Lcdprint(named,2,1);
      Lcdprint("วัตถุดิบได้ทำการเพิ่มเรียบร้อยแล้ว",2,0);
      Firebase.setInt(firebaseData,"SendKey/Key",0);
      xSemaphoreGive( xMutex );  
      tt = 0;  
    }
    else{
      xSemaphoreTake( xMutex, portMAX_DELAY );
      delay(1);
      sound = 3;
      Lcdprint("ไม่สามารถทำการเพิ่มได้",2,1);
      xSemaphoreGive( xMutex );
      tt = 0;
    }
    }
  }

void Lcdprint(String Data,unsigned char Set,boolean Reset){
  
  Data.toCharArray(buf, 100);
  if(Set == 0){
    if(Reset == 1)tft.fillRect(0, 0, 100, 23, TFT_BLACK);
    dw_font_goto(&myfont, 10, 22);
    dw_font_print(&myfont, buf);
  }
  else if(Set == 1){
    if(Reset == 1)tft.fillRect(390, 0, 90, 23, TFT_BLACK);
      dw_font_goto(&myfont, 305, 22);
      dw_font_print(&myfont, buf);
  }
  else if(Set == 2){
    if(Reset == 1){
      tft.fillRect(0, 33, 480, 162, TFT_BLACK);
      tft.fillRect(0, 32, 480, 3, TFT_BLUE);
      tft.fillRect(0, 195, 480, 3, TFT_BLUE);
      tft.fillRect(240, 195, 3, 130, TFT_BLUE);
      dw_font_setfont(&myfont, &font_th_sarabunpsk_regular40);   
      hj=80;
    }      
      dw_font_goto(&myfont, 90, hj);
      dw_font_print(&myfont, buf);
      dw_font_setfont(&myfont, &font_th_sarabunpsk_regular34);
      if(hj <= 105)hj = hj+25;
      else hj = hj+30;
  }  
}

void Lcdprint1(String Data,int x,int y){
  String(Data).toCharArray(buf, 100);
  dw_font_goto(&myfont, x, y);
  dw_font_print(&myfont, buf);
}

void fxsound(bool Port,unsigned int Delay){
  digitalWrite(33, Port);
  delay(Delay);
}