import { Component, ViewChild  } from '@angular/core';
import { Nav, Platform, MenuController } from 'ionic-angular';
//import { StatusBar } from '@ionic-native/status-bar';
//import { SplashScreen } from '@ionic-native/splash-screen';
import { basket } from '../providers/firebase/basket.model';
import { Splashscreen,StatusBar } from 'ionic-native';
//import { HomePage } from '../pages/home/home';
//import { TabsPage } from '../pages/tabs/tabs';
import { items } from '../providers/firebase/item.model';
import firebase from 'firebase';
import { TabsPage } from '../pages/tabs/tabs';
// import { MenuaddPage } from '../pages/menuadd/menuadd';
// import { BasketPage } from '../pages/basket/basket';
// import { SpecialPage } from '../pages/special/special';
import { barcode } from '../providers/firebase/barcode.model';
import { LocalNotifications, ELocalNotificationTriggerUnit } from '@ionic-native/local-notifications';
import { AngularFireDatabase } from 'angularfire2/database';
//import { TabsPage } from '../pages/tabs/tabs';
//import { AdminPage } from '../pages/admin/admin';
//import { BarcodePage } from '../pages/barcode/barcode';

@Component({
  templateUrl: 'app.html'
})
export class MyApp {
  public selectedRange1= 0;
  public selectedRange2= 0;
  public itemName1:string
  public itemName2:string
  public amount1 = 0 ; 
  public amount2 = 0 ;
  public temp1:any;
  public temp2:any;

  public code:string;
  public itemList: Array<any>;
  public loadedItemList: Array<any>;
  public listMonth = [31,28,31,30,31,30,31,31,30,31,30,31] ;
  public NumberDate = 0
  public NumberYear = 0
  public NumberMonth = 0
  public sumCheckMonth = 0
  public itemkey :any;
  public itemvale :any;

  public itemlist$: Array<barcode[]>;
  public removePath:any
  public expDate = 0
  public expMonth = 0
  public expYear = 0
  
  public startDate = new Date().getDate();
  public startMonth = new Date().getUTCMonth() + 1;
  public startYear = new Date().getFullYear();
  @ViewChild(Nav) nav: Nav;
  //rootPage:any = AdminPage;
  //rootPage:any = TabsPage;
  rootPage:any = 'LoginPage';
  activePage: any;
  pages: Array<{title: string, component: any}>;

  constructor( public platform: Platform, public menu: MenuController , public dbf: AngularFireDatabase,public localNotifications: LocalNotifications) {



    this.initializeApp();
    //this.CalculateDate1();
    //this.CalculateDate2();
    //this.loadcell();


 
  }

  CalculateDate1()
  {

    this.dbf.list<barcode>(`/DB-barcode/`).snapshotChanges().subscribe((res) => {
      let itemes = [];
      
      res.forEach((item) => {
        
      this.itemvale = item.payload.val()
      this.itemkey = item.key;
        //console.log(itemvale.Name)
        //console.log(itemkey);

        this.NumberDate = 0
        this.NumberYear = 0
        this.NumberMonth = 0
        this.sumCheckMonth = 0
        
        //ตัดข้อความเป็น Char จากนั้นแปลงเป็น Int เพื่อคำนวณ
        this.expYear = parseInt(this.itemvale.expitem[0]) * 1000 + parseInt(this.itemvale.expitem[1]) * 100 + parseInt(this.itemvale.expitem[2]) * 10 +parseInt(this.itemvale.expitem[3]) * 1;  
        this.expMonth = parseInt(this.itemvale.expitem[5]) * 10 + parseInt(this.itemvale.expitem[6]) * 1;   
        this.expDate = parseInt(this.itemvale.expitem[8]) * 10 + parseInt(this.itemvale.expitem[9]) * 1 ;

        
        this.NumberYear = this.expYear - this.startYear ;

        
        if(this.startMonth < this.expMonth && this.NumberYear == 0){
          this.NumberMonth = this.startMonth - this.expMonth


          for( let i = this.startMonth - 1 ; i < this.expMonth -1 ; i++ ){
          //console.log(this.listMonth[i])
          this.sumCheckMonth += this.listMonth[i] ; 
          
          }
          this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
       

        }else if(this.startMonth > this.expMonth && this.NumberYear == 0){
          this.NumberMonth = 0;
     

        }else if(this.startMonth < this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
        

        }else if(this.startMonth > this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
     

        }else if(this.startMonth < this.expMonth && this.NumberYear > 0 ){
          // console.log(this.NumberYear)
          // this.NumberMonth = (this.startMonth - this.expMonth) + 11
          for( let i = this.startMonth - 1,j = 0 ; i < this.expMonth + (12*this.NumberYear)-1 ; i ++ ){
            if(j>11){
              j = 0;
            }
            if(i > 11)
            {
              
              this.sumCheckMonth += this.listMonth[j] ; 
            // console.log(this.listMonth[j])
            //  console.log(j)
            
            j++;
            }
            else
            {
              this.sumCheckMonth += this.listMonth[i] ; 
              //console.log(this.listMonth[i])
            }
          
            }
            this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
           
        }else if(this.startMonth == this.expMonth && this.startDate <= this.expDate)
        {
            this.NumberDate =  this.expDate - this.startDate;
       
        }
        else if(this.startMonth == this.expMonth && this.startDate > this.expDate)
        {
            // this.NumberDate =  this.expDate - this.startDate;
            this.NumberDate = 0;
      

        }else{
          for( let i = this.startMonth - 1,j = 0 ; i < this.expMonth + 11 ; i++ ){
            
            if(i > 11)
            {
              
              this.sumCheckMonth += this.listMonth[j] ; 
            // console.log(this.listMonth[j])
            //console.log(j)
            j++;
            }
            else
            {
              this.sumCheckMonth += this.listMonth[i] ; 
              //console.log(this.listMonth[i])
            }
          
            }
            this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
    
           
        }

        if(this.NumberDate <= 3){
          
          this.localNotifications.schedule({
            
            title: 'มีวัตถุดิบใกล้หมดอายุ',
            text: this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน',
            trigger: { in: 5, unit: ELocalNotificationTriggerUnit.SECOND },
            foreground: true ,
           
         });
        
        }
        
        //this.color = '#ba1488';
     
        if(this.NumberDate == 0){
          console.log("หมดอายุ")
          const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.itemvale.code);
          items1.update({    
          CountDay:"หมดอายุ"
          })

          this.itemvale.CountDay = "หมดอายุ";
          itemes.push(this.itemvale);
          this.itemlist$ = itemes;

          //เมื่อวัตถุดิบหมดอายุจะส่งไปตารางหมดอายุ
          const items2: firebase.database.Reference = firebase.database().ref('/Expired/'+this.itemvale.code);
          items2.update({
          Name: this.itemvale.Name,
          piece:this.itemvale.piece,
          timeKey: this.itemvale.timeKey,
          expitem: this.itemvale.expitem,
          CountDay:"หมดอายุ" ,
          addBy: this.itemvale.addBy,
          code:this.itemvale,

          })//ตารางหมดอายุ
          this.dbf.list('/DB-barcode/').remove(this.itemvale.code);
          return false
        }

        const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.itemvale.code);
        
        items1.update({    
        CountDay:this.NumberDate.toString()
        })
 
        this.itemvale.CountDay = this.NumberDate+" วัน" ;
        itemes.push(this.itemvale);
        this.itemlist$ = itemes;
        //console.log(this.itemlist$)
        
        
        })
   
  

    });

    
  }
  
  CalculateDate2()
  {
   
    
 
    this.dbf.list<items>(`/items/`).snapshotChanges().subscribe((res) => {
      let itemes = [];
      
      res.forEach((item) => {
        
      this.itemvale = item.payload.val()
      this.itemkey = item.key;
        //console.log(itemvale.Name)
        //console.log(itemkey);
      
       
   

        this.NumberDate = 0
        this.NumberYear = 0
        this.NumberMonth = 0
        this.sumCheckMonth = 0
        
        //ตัดข้อความเป็น Char จากนั้นแปลงเป็น Int เพื่อคำนวณ
        this.expYear = parseInt(this.itemvale.expitem[0]) * 1000 + parseInt(this.itemvale.expitem[1]) * 100 + parseInt(this.itemvale.expitem[2]) * 10 +parseInt(this.itemvale.expitem[3]) * 1;  
        this.expMonth = parseInt(this.itemvale.expitem[5]) * 10 + parseInt(this.itemvale.expitem[6]) * 1;   
        this.expDate = parseInt(this.itemvale.expitem[8]) * 10 + parseInt(this.itemvale.expitem[9]) * 1 ;

        
        this.NumberYear = this.expYear - this.startYear ;

        
        if(this.startMonth < this.expMonth && this.NumberYear == 0){
          this.NumberMonth = this.startMonth - this.expMonth


          for( let i = this.startMonth - 1 ; i < this.expMonth -1 ; i++ ){
          //console.log(this.listMonth[i])
          this.sumCheckMonth += this.listMonth[i] ; 
          
          }
          this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
 
        }else if(this.startMonth > this.expMonth && this.NumberYear == 0){
          this.NumberMonth = 0;
          

        }else if(this.startMonth < this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
       

        }else if(this.startMonth > this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          

        }else if(this.startMonth < this.expMonth && this.NumberYear > 0 ){
          // console.log(this.NumberYear)
          // this.NumberMonth = (this.startMonth - this.expMonth) + 11
          for( let i = this.startMonth - 1,j = 0 ; i < this.expMonth + (12*this.NumberYear)-1 ; i ++ ){
            if(j>11){
              j = 0;
            }
            if(i > 11)
            {
              
              this.sumCheckMonth += this.listMonth[j] ; 
            // console.log(this.listMonth[j])
            //  console.log(j)
            
            j++;
            }
            else
            {
              this.sumCheckMonth += this.listMonth[i] ; 
              //console.log(this.listMonth[i])
            }
          
            }
            this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
           

        }else if(this.startMonth == this.expMonth && this.startDate <= this.expDate)
        {
            this.NumberDate =  this.expDate - this.startDate;
        
        }
        else if(this.startMonth == this.expMonth && this.startDate > this.expDate)
        {
            // this.NumberDate =  this.expDate - this.startDate;
            this.NumberDate = 0;
        

        }else{
          for( let i = this.startMonth - 1,j = 0 ; i < this.expMonth + 11 ; i++ ){
            
            if(i > 11)
            {
              
              this.sumCheckMonth += this.listMonth[j] ; 
            // console.log(this.listMonth[j])
            //console.log(j)
            j++;
            }
            else
            {
              this.sumCheckMonth += this.listMonth[i] ; 
              //console.log(this.listMonth[i])
            }
          
            }
            this.NumberDate = this.sumCheckMonth - this.startDate + this.expDate
           
        }
        if(this.NumberDate <= 3){
          this.localNotifications.schedule({
            
            title: 'มีวัตถุดิบใกล้หมดอายุ',
            text: this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน',
            trigger: { in: 5, unit: ELocalNotificationTriggerUnit.SECOND },
            foreground: true ,
           
         });
       
        }
        //this.color = '#ba1488';


      const nuw: firebase.database.Reference = firebase.database().ref('/items/'+this.itemkey+'/RFID/row');
        nuw.on('value', itemSnapshot => {
        if(itemSnapshot.val() == 0 ){
  
        }

        // if(itemSnapshot.val() != 0 ){
        //    this.renderer.setElementStyle(this.myButton.nativeElement,'opacity','0');
        // }
        
        }) //จำนวนของ tag RFID
        
        if(this.NumberDate == 0){
          const items1: firebase.database.Reference = firebase.database().ref('/items/'+this.itemkey);
          items1.update({    
          CountDay:"หมดอายุ"
          })

          this.itemvale.CountDay = "หมดอายุ";
          itemes.push(this.itemvale);
          this.itemlist$ = itemes;

          //เมื่อวัตถุดิบหมดอายุจะส่งไปตารางหมดอายุ
          const items2: firebase.database.Reference = firebase.database().ref('/Expired/'+this.itemkey);
          items2.update({
          Name: this.itemvale.Name,
          piece:this.itemvale.piece,
          timeKey: this.itemvale.timeKey,
          expitem: this.itemvale.expitem,
          CountDay:"หมดอายุ" ,
          Status:"N" ,
          addBy: this.itemvale.addBy,

          })//ตารางหมดอายุ
          this.dbf.list('/items/').remove(this.itemkey);
          return false
        }

    
        
        
        const items1: firebase.database.Reference = firebase.database().ref('/items/'+this.itemkey);
        
        items1.update({    
        CountDay:this.NumberDate.toString(),
        NumberPath: parseInt(this.itemkey) ,   //อ่านเลข Path มาเก็บใน NumberPath
        })
        
        this.itemvale.CountDay = this.NumberDate+" วัน" ;
        itemes.push(this.itemvale);
        this.itemlist$ = itemes;
        //console.log(this.itemlist$)
        
        
        })
        
  

    });

    
  }

  loadcell(){
    this.dbf.list<basket>(`/Loadcell/`).snapshotChanges().subscribe((res) => {
      let itemes = [];
      res.forEach((basket) => {
      this.itemvale = basket.payload.val()
      itemes.push(this.itemvale);
      this.itemlist$ = itemes;

  
      })  
      this.selectedRange1 = itemes[0].Weight_per_piece
      this.itemName1 = itemes[0].Name
      this.amount1 = itemes[0].piece
      this.temp1 = itemes[0].weight

      this.selectedRange2 = itemes[1].Weight_per_piece
      this.itemName2 = itemes[1].Name
      this.amount2 = itemes[1].piece
      this.temp2 = itemes[1].weight
    
    
      this.amount1 = (this.selectedRange1 / (this.temp1/1000)) ;
      this.amount2 = (this.selectedRange2 / (this.temp2/1000)) ;

      this.amount1 = Math.round(this.amount1);
      this.amount2 = Math.round(this.amount2);
      console.log(Math.round(this.amount1));

      const items1: firebase.database.Reference = firebase.database().ref('/Loadcell/1');
        items1.update({  
        //Weight_per_piece: Math.floor(itemes[0].Weight_per_piece) ,
        piece:Math.round(this.amount1),
    
        })
      const items2: firebase.database.Reference = firebase.database().ref('/Loadcell/2');
        items2.update({    
        piece:Math.round(this.amount2),

        })
    });
  }


  initializeApp() {
    this.platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      StatusBar.styleDefault();
      Splashscreen.hide();
    });
  }
  openPage(page) {
    // Reset the content nav to have just this page
    // we wouldn't want the back button to show in this scenario
    this.nav.setRoot(page.component);
    this.activePage = page;

  }

  checkActive(page){
     
 
    return page == this.activePage;
  }


}