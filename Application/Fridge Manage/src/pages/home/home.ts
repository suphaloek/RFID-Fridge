import { Component } from '@angular/core';
import { NavController , AlertController, ActionSheetController, LoadingController,} from 'ionic-angular';
//import { AddsPage } from '../adds/adds';
import { Platform } from 'ionic-angular';
import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen'
import { NavParams } from 'ionic-angular';
import firebase from 'firebase';
import { FirebaseProvider } from './../../providers/firebase/firebase';
//import { Observable } from 'rxjs/Observable';
import { items } from '../../providers/firebase/item.model';
import { AngularFireDatabase } from 'angularfire2/database';
import { LoginPage } from '../login/login';
import { VeryPage } from '../very/very';
import { AngularFireList } from 'angularfire2/database'
import { FirebaseListObservable } from '../../../node_modules/angularfire2/database-deprecated';
import { EditPage } from '../edit/edit';
import { Http } from '../../../node_modules/@angular/http';
import { LocalNotifications } from '@ionic-native/local-notifications';
import { ELocalNotificationTriggerUnit} from '@ionic-native/local-notifications';
import { barcode } from '../../providers/firebase/barcode.model';
import { BarcodeEditPage } from '../barcode-edit/barcode-edit';
import { AddbarcodePage } from '../addbarcode/addbarcode';
import { ProgressPage } from '../progress/progress';
import { ReadcodePage } from '../readcode/readcode';


@Component({
  selector: 'page-home',
  templateUrl: 'home.html'
})



export class HomePage {
  Tag:any
  scheduled = [];
  item = {};
  numrow: any;
  rfid:any;
  row = [];
  q = []
  public tabs:any
  public nameTag:any
  public itemList: Array<any>;
  public itemList2: Array<any>;
  public itemlist$: Array<items[]>;
  public removePath:any
  public expDate = 0
  public expMonth = 0
  public expYear = 0
  //public a = -1
  //b = Math.abs(this.a)

  public loadedItemList: Array<any>;
  public loadedItemList2: Array<any>;
  public itemlistA: AngularFireList<any[]>;
  shoppingItems: FirebaseListObservable<any[]>;
  public itemssss;
  public temp1:any;
  public temp2:any;
  public key:any
  public detail:any
  public loading:any
  itemcsd: items = {

    Name: "",
    Status: "",
    expitem: "",
    piece: 1,
    timeKey: "",
    NumberPath: 1,
   
  };


  public startDate = new Date().getDate();
  // public startMonth = new Date().getUTCMonth()+2;
  public startMonth = new Date().getMonth()+1;
  public startYear = new Date().getFullYear();
  // public startDate = 1;
  // public startMonth = 4+1;
  // public startYear = 2018;
  ////////////////////1 ,2 ,3, 4 ,5 , 6 ,7, 8,9 ,10,11,12//////////////////////////////////////
  public listMonth = [31,28,31,30,31,30,31,31,30,31,30,31] ;
  public NumberDate = 0
  public NumberYear = 0
  public NumberMonth = 0
  public sumCheckMonth = 0
  public itemkey :any;
  public itemvale :any;
  public Loop = 1;
  //posts: '#ba1488';
  public color1 = '#0000FF'
  public color2 = '#FF1493'
  public bgcolor = '#99CCFF'
  public bgcolor1 = '#F5F5F5'
  public bgcolor2 = '#99FF00'
  public colorButton1 = '#FF1493'
  public colorButton2 = '#00CC00'
  public sendKey:any
  public temp:any
  public temp3:any
 
  
  constructor(public navCtrl: NavController, platform: Platform, public dbf: AngularFireDatabase, statusBar: StatusBar,
              splashScreen: SplashScreen, public navParams: NavParams, public firebaseProvider: FirebaseProvider
              ,private alertCtrl: AlertController, private actionSheetCtrl: ActionSheetController ,public http: Http,public localNotifications: LocalNotifications,
              public loadingCtrl: LoadingController
              ) {

        if(this.Loop == 1){
          const items1: firebase.database.Reference = firebase.database().ref('/SendKey/');
          items1.update({    
          Key:0,
          })
          this.CalculateDate();
          this.CalculateDate2();

        this.Loop = 0
        }
        //console.log(this.Loop);
        //this.allList();
        
        this.pauseBarcode();
        this.alertTag();
        this.Out();

      }

      Out(){
        this.dbf.list<items>(`/History/`).snapshotChanges().subscribe((res) => {
          let Outs = [];
    
          res.forEach((item) => {
          var itemvaleOut = item.payload.val()
          Outs.push(itemvaleOut);
          //this.itemlist$ = Outs;
          if(itemvaleOut.export < itemvaleOut.piece && itemvaleOut.export != 0 && itemvaleOut.NumberPath < 100){
            const itemsH: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+itemvaleOut.NumberPath);
            itemsH.update({    
            Out:'นำออก '+itemvaleOut.export+'ชิ้น'
            })
          }// RFID
          if(itemvaleOut.export < itemvaleOut.piece && itemvaleOut.export != 0 && itemvaleOut.NumberPath > 100){
            const itemsH: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+itemvaleOut.NumberPath);
            itemsH.update({    
            Out:'นำออก '+itemvaleOut.export+'ชิ้น'
            })
          }//Barcode
          if(itemvaleOut.export == itemvaleOut.piece){
            console.log( itemvaleOut.export)
            console.log( itemvaleOut.Name)
            console.log( itemvaleOut.NumberPath)
            if(itemvaleOut.NumberPath < 100){
            const itemsH: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+itemvaleOut.NumberPath);
            itemsH.update({    
            Out:'(นำออกหมด)'
            })
            }
            if(itemvaleOut.NumberPath > 100){
              const itemsH: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+itemvaleOut.NumberPath);
              itemsH.update({    
              Out:'(นำออกหมด)'
              })
              }
           }
          })         
          });
      }

      expshow(){
        let alert = this.alertCtrl.create({
          subTitle: 'วันหมดอายุ',
          buttons: [
            {
              text: 'ok',
              handler: () => {
              }
            }
          ]
        });
        alert.present();
      }

      presentLoadingText() {
        let loading = this.loadingCtrl.create({
          spinner: 'hide',
          content: 'Loading Please Wait...'
        });
      
        loading.present();
      
        setTimeout(() => {
          this.navCtrl.push(ProgressPage);
        }, 1000);
      
        setTimeout(() => {
          loading.dismiss();
        }, 5000);

        
          
      }

      ExpiredShow(){
        firebase.database().ref('/DB-RFID/').orderByChild('expitem').on('value', function(snapshot) {
        let itemes = [];
        snapshot.forEach(function(child) {
        itemes.push(child.val());
        }.bind(this));
        console.log(itemes)
        
        //console.log("all", data.map(function(val) { return new Date(val.timestamp).toString(); }));
      });
      }

      alertTag(){
        var temp = 0
        
        const sendKey: firebase.database.Reference = firebase.database().ref('/SendKey/Key');
        sendKey.on('value', itemSnapshot1 => {
        this.sendKey = itemSnapshot1.val();
        //temp = this.sendKey
        console.log(this.sendKey)

        const Tag: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.sendKey+'/RFID/row');
        Tag.on('value', itemSnapshot2 => {
        this.Tag = itemSnapshot2.val();
        
        if(this.Tag > temp ){
                  //this.alertTag()
          console.log(this.Tag)
          this.loading = this.loadingCtrl.create({
              spinner: 'hide',
              content: "นำเข้าแล้ว "+this.Tag +" ชิ้น",
              duration: 2000
          });
          this.loading.present();
          temp = this.Tag
        }//end if  
        
        })
        
        })
        return false   
      }

  allList(){
    
    this.tabs = '';

    this.dbf.list<items>(`/DB-RFID`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      firebase.database().ref('/DB-RFID/').orderByChild('expitem').on('value', function(snapshot) {
       
        snapshot.forEach(function(child) {
        itemes.push(child.val());
        }.bind(this));
        console.log(itemes)
      });
      this.itemList = itemes;
      this.loadedItemList = itemes;
      
    });
    

   
    this.dbf.list<barcode>(`/DB-barcode/`).valueChanges().subscribe((res: barcode[]) => {
      let barcode = [];

      firebase.database().ref('/DB-barcode/').orderByChild('expitem').on('value', function(snapshot) {
       
        snapshot.forEach(function(child) {
          barcode.push(child.val());
        }.bind(this));
        console.log(barcode)
      });
      this.itemList2 = barcode;
      this.loadedItemList2 = barcode;
    });
  }

  pauseTag(){
    this.tabs = 'Barcode';
    this.itemList = []
    this.dbf.list<barcode>(`/DB-barcode/`).valueChanges().subscribe((res: barcode[]) => {
      let barcode = [];
      
      firebase.database().ref('/DB-barcode/').orderByChild('expitem').on('value', function(snapshot) {
       
        snapshot.forEach(function(child) {
          barcode.push(child.val());
        }.bind(this));
        console.log(barcode)
      });
      
      this.itemList2 = barcode;
      this.loadedItemList2 = barcode;
      
    });
  }
  
  pauseBarcode(){
    this.tabs = 'Tag';
    this.itemList2 = []
    this.dbf.list<items>(`/DB-RFID`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];
      firebase.database().ref('/DB-RFID/').orderByChild('expitem').on('value', function(snapshot) {
       
        snapshot.forEach(function(child) {
        itemes.push(child.val());
        }.bind(this));
        console.log(itemes)
      });

      this.itemList = itemes;
      this.loadedItemList = itemes;
      
    });
  }

  barcodeAdd()
  {
    this.navCtrl.push(AddbarcodePage);
  }
  
  initializeItemsRFID(): void {
    this.itemList = this.loadedItemList;
    //this.itemList2 = []
  }

  initializeItemsBarcode(): void {
    this.itemList2 = this.loadedItemList2;
    //this.itemList = []
  }

  
  getitems(searchbar) {

    // Reset items back to all of the items
    if(this.tabs == ''){
      this.initializeItemsRFID()
      this.initializeItemsBarcode()
    }
    if(this.tabs == 'Tag'){
      this.initializeItemsRFID()
    }
    if(this.tabs == 'Barcode'){
      this.initializeItemsBarcode()
    }
    // this.initializeItemsBarcode()
     this.q[1] = searchbar.srcElement.value;
     this.q[2] = searchbar.srcElement.value;

    //console.log(g);
    // if the value is an empty string don't filter the items
    if (!this.q[1]) {
      return
    }
    if (!this.q[2]) {
      return
    }
    

    //console.log(this.itemList)
    if(this.tabs == 'Tag'){
      this.itemList = this.itemList.filter((v) => {
        //console.log(v);
        if (v.Name && this.q[1]) {
          if (v.Name.toLowerCase().indexOf(this.q[1].toLowerCase()) > -1) {
            return true;
          }
          return false;
        }
        // if (v.CountDay && this.q[1]) {
        //   if (v.CountDay.toLowerCase().indexOf(this.q[1].toLowerCase()) > -1) {
        //     return true;
        //   }
        //   return false;
        // }
      });
      //this.pauseBarcode()
    }

    if(this.tabs == 'Barcode'){
      this.itemList2 = this.itemList2.filter((v) => {
        //console.log(v);
        if (v.Name && this.q[2]) {
          if (v.Name.toLowerCase().indexOf(this.q[2].toLowerCase()) > -1) {
            return true;
          }
          return false;
        }
      });
      //this.pauseTag();
    }

    if(this.tabs == ''){
    this.itemList = this.itemList.filter((v) => {
      //console.log(v);
      if (v.Name && this.q[1]) {
        if (v.Name.toLowerCase().indexOf(this.q[1].toLowerCase()) > -1) {
          return true;
        }
        return false;
      }
    });
    this.itemList2 = this.itemList2.filter((v) => {
      //console.log(v);
      if (v.Name &&  this.q[2]) {
        if (v.Name.toLowerCase().indexOf( this.q[2].toLowerCase()) > -1) {
          return true;
        }
        return false;
      }
    });
  }
  }

  CalculateDate2()
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
          // console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด")

        }else if(this.startMonth > this.expMonth && this.NumberYear == 0){
          this.NumberMonth = 0;
          // console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด")   //<--------------------------------

        }else if(this.startMonth < this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          // console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด")   //<--------------------------------

        }else if(this.startMonth > this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          // console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด")   //<--------------------------------

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
            // console.log("เดือนเท่ากัน แต่ ปีมากกว่า 0 ")

        }else if(this.startMonth == this.expMonth && this.startDate <= this.expDate)
        {
            this.NumberDate =  this.expDate - this.startDate;
            // console.log("เดือนเท่ากัน")
        }
        else if(this.startMonth == this.expMonth && this.startDate > this.expDate)
        {
            // this.NumberDate =  this.expDate - this.startDate;
            this.NumberDate = 0;
            // console.log("เดือนเท่ากัน แต่วันติดลบ")   //<--------------------------------

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
            console.log("")
           
        }
        console.log(this.NumberDate)

        if(this.itemvale.piece == 0){
          //this.dbf.list('/DB-barcode/'+this.itemvale.code).remove();
          //return false
        }

        if(this.NumberDate <= 3 && this.NumberDate > 0){
          this.localNotifications.schedule({
            
            title: 'มีวัตถุดิบใกล้หมดอายุ',
            text: this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน =>ชนิด '+'(Barcode)',
            trigger: { in: 2, unit: ELocalNotificationTriggerUnit.SECOND },
            foreground: true ,
           
         });
        }
        //this.color = '#ba1488';
     
        if(this.NumberDate == 0){
          console.log("หมดอายุ")
       
          this.itemvale.CountDay = "Expired";
          itemes.push(this.itemvale);
          this.itemlist$ = itemes;

          //เมื่อวัตถุดิบหมดอายุจะส่งไปตารางหมดอายุ
          const items2: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.itemvale.code);
          items2.update({
          CountDay:this.itemvale.CountDay ,
          })
          const items3: firebase.database.Reference = firebase.database().ref('/History/'+this.itemvale.code);
          items3.update({
          CountDay:this.itemvale.CountDay ,
          })
          //ตารางหมดอายุ
          //this.dbf.list('/DB-barcode/'+this.itemvale.code).remove();
          return false
        }
        
        if(this.itemvale.piece > 0 && this.NumberDate != 0 ){
        const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.itemvale.code);
        
        items1.update({    
        CountDay:this.NumberDate+" Days"
        })
        const items2: firebase.database.Reference = firebase.database().ref('/History/'+this.itemvale.code);
        
        items2.update({    
        CountDay:this.NumberDate+" Days",
        })
        }
 
        this.itemvale.CountDay = this.NumberDate+" Days" ;
        itemes.push(this.itemvale);
        this.itemlist$ = itemes;
        //console.log(this.itemlist$)

        // if(this.itemvale.piece == 0){
        //   this.dbf.list('/DB-barcode/').remove(this.itemvale.code);
        // }
        
        
        })
   
  

    });

    
  }
  
  selectBarcode(){
    let alert = this.alertCtrl.create({
      subTitle: 'เลือกการจัดการบาร์โค้ด',
      buttons: [
        {
          text: 'นำเข้าวัตถุดิบ',
          handler: () => {
            this.navCtrl.push(AddbarcodePage);
          }
        },
  
        {
          text: 'นำออก',
          handler: () => {
            this.navCtrl.push(ReadcodePage);
          }
        }
      ]
    });
    alert.present();
  }

  selectEdditItem2(editItems: any) {

    var itemthis10 
    this.dbf.list('/DB-barcode/', ref => ref.orderByChild('code').equalTo(editItems.code)).snapshotChanges()
      .subscribe(actions => {
        actions.forEach(action => {

          itemthis10 = action.key;
        });
      });
      

    this.actionSheetCtrl.create({
      title:`${editItems.Name}`,
      buttons: [
        {
          text: 'Scan',
          handler: () => {  
          this.navCtrl.push(ReadcodePage);

          }
        },
        {
          text: 'Edit',
          handler: () => {  
          this.navCtrl.push(BarcodeEditPage,{edititemskey:itemthis10});

          }
        },
        {
          text: 'Delete',
          role: 'destructive',
          handler: () => {
            this.dbf.list('/DB-barcode/').remove(itemthis10);
            //this.dbf.list('/History/').remove(itemthis10);
            console.log(itemthis10);
          }
        },
        {
          text: 'Cancel',
          role: 'cancel',
          handler: () => {
            console.log("The user has selected the cancel button");
          }
        }
      ]

    }).present();
  }

  ionViewDidLoad() {
    //console.log('ionViewDidLoad SpecialPage');
  }

  Notification(){
   
  }
  
  


  CalculateDate()
  {
   
    
 
    this.dbf.list<items>(`/DB-RFID/`).snapshotChanges().subscribe((res) => {
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
          console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด 1")

        }else if(this.startMonth > this.expMonth && this.NumberYear == 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด 2")   //<--------------------------------

        }else if(this.startMonth < this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด 3")   //<--------------------------------

        }else if(this.startMonth > this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด 4")   //<--------------------------------

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
            console.log("เดือนเท่ากัน แต่ ปีมากกว่า 0 ")

        }else if(this.startMonth == this.expMonth && this.startDate <= this.expDate)
        {
            this.NumberDate =  this.expDate - this.startDate;
            console.log("เดือนเท่ากัน")
            console.log(this.NumberDate)
            console.log(this.expDate)
            console.log(this.startDate)
            console.log(this.startMonth)
        }
        else if(this.startMonth == this.expMonth && this.startDate > this.expDate)
        {
            // this.NumberDate =  this.expDate - this.startDate;
            this.NumberDate = 0;
            console.log("เดือนเท่ากัน แต่วันติดลบ")   //<--------------------------------

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
            console.log("")
           
        }
        console.log(this.NumberDate)

        // if(this.itemvale.RFID.row == this.itemvale.piece){
        // const items1: firebase.database.Reference = firebase.database().ref('/items/'+this.itemkey+'/RFID');
        // items1.update({    
        // row:"done",
        // })
        // }
        
        if(this.itemvale.piece == 0){
          // this.dbf.list('/DB-RFID/').remove(this.itemkey);
          // return false
        }

        if(this.NumberDate <= 3 && this.NumberDate > 0){
          this.localNotifications.schedule({
            
            title: 'มีวัตถุดิบใกล้หมดอายุ',
            text: this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน =>ชนิด '+'(RFID)' ,
            trigger: { in: 2, unit: ELocalNotificationTriggerUnit.SECOND },
            foreground: true ,
           
         });//แจ้งเตือน
         
        //  let toast = this.toastCtrl.create({
        //   message: 'RFID => ' + this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน',
        //   closeButtonText: 'Ok',
        //   duration: 5000,
        //   position: 'top'
        // });
      
        // toast.onDidDismiss(() => {
        //   console.log('Dismissed toast');
        // });
      
        // toast.present();
         
        }
        


        if(this.NumberDate == 0){
          console.log("หมดอายุ")
          // const items1: firebase.database.Reference = firebase.database().ref('/items/'+this.itemkey);
          // items1.update({    
          // CountDay:"หมดอายุ"
          // })

          this.itemvale.CountDay = "Expired";
          itemes.push(this.itemvale);
          this.itemlist$ = itemes;

          //เมื่อวัตถุดิบหมดอายุจะส่งไปตารางหมดอายุ
          const items2: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.itemkey);
          items2.update({
          CountDay:this.itemvale.CountDay,
          })
          const items4: firebase.database.Reference = firebase.database().ref('/History/'+this.itemkey);
          items4.update({
          CountDay:this.itemvale.CountDay,
          })//หมดอายุ
          // this.dbf.list('/DB-RFID/').remove(this.itemkey);
          // return false
        }

        
        if(this.itemvale.piece > 0 && this.NumberDate != 0 ){
        const items1: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.itemkey);
        
        items1.update({    
        CountDay:this.NumberDate+" Days",
        })
        const items3: firebase.database.Reference = firebase.database().ref('/History/'+this.itemkey);
        
        items3.update({
        Name:this.itemvale.Name,
        addBy:this.itemvale.addBy,

        CountDay:this.NumberDate+" Days",
        })
        }//end if
        this.itemvale.CountDay = this.NumberDate+" Days";
        itemes.push(this.itemvale);
        this.itemlist$ = itemes;
        //console.log(this.itemlist$)
        
        })
        
     


    });

    console.log(this.startDate);
    console.log(this.startMonth);
    console.log(this.startYear);
    console.log(this.itemlist$);
    
  }

  readBarcode()
  {
     this.navCtrl.push(ReadcodePage);
  }
  gotoadd()
  {
    this.navCtrl.push(VeryPage);
  }
  gotologin()
  {
    this.navCtrl.setRoot(LoginPage);
    
    
  }
  delete(path2){
      console.log(path2)
      //parseInt(path2)
      this.removePath = path2.toString()
      this.AlertRemove()
  }
  AlertRemove()
  {
    let alert = this.alertCtrl.create({
      title: 'ลบข้อมูลวัตถุดิบ ?',
      buttons: [
        {
          text: 'ยกเลิก',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
  
        {
          text: 'ยืนยัน',
          handler: () => {
            //this.dbf.list('/DB-RFID/').remove(this.removePath);
          }
        }
      ]
    });
    alert.present();
  }

  showRFIDpeice(path){
    this.key = parseInt(path)
      console.log(path);
          const send: firebase.database.Reference = firebase.database().ref('/SendKey');
          send.update({
          Key: this.key,
          })
          
          const NameTag: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.key);
          NameTag.on('value', itemSnapshot => {
            this.nameTag = itemSnapshot.val();
          })
    let alert = this.alertCtrl.create({
      subTitle:"จำนวนคงเหลือในตู้แช่",
      message:"จำนวนวัตถุดิบ : "+this.nameTag.RFID.row,
      buttons: [
        {
          text: 'ok',
          handler: () => {
          }
        },
      ]
    });alert.present();
  }
  showBarcodepeice(path){
    this.key = parseInt(path)
      console.log(path);
          const send: firebase.database.Reference = firebase.database().ref('/SendKey');
          send.update({
          Key: this.key,
          })
          
          const NameTag: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.key);
          NameTag.on('value', itemSnapshot => {
            this.nameTag = itemSnapshot.val();
          })
    let alert = this.alertCtrl.create({
      subTitle:"จำนวนคงเหลือในตู้แช่",
      message:'จำนวนวัตถุดิบ : '+this.nameTag.temp,
      buttons: [
        {
          text: 'ok',
          handler: () => {
          }
        },
      ]
    });alert.present();
  }

  btnActivate(path) {
     
      this.key = parseInt(path)
      console.log(path);
          const send: firebase.database.Reference = firebase.database().ref('/SendKey');
          send.update({
          Key: this.key,
          })
          
          const NameTag: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.key);
          NameTag.on('value', itemSnapshot => {
            this.nameTag = itemSnapshot.val();
          })

          if(this.nameTag.RFID.row == this.nameTag.piece){
          let alert = this.alertCtrl.create({
            message:"จำนวนวัตถุดิบ : "+this.nameTag.RFID.row,
            title: "นำเข้าครบ!",
            buttons: [
              {
                text: 'ok',
                handler: () => {
                }
              },
            ]
          });
          alert.present();
          }
          if(this.nameTag.RFID.row < this.nameTag.piece && this.nameTag.Status =='w'){
            console.log(this.nameTag)
            let alert = this.alertCtrl.create({
              subTitle:"กรุณานำวัตถุดิบไปสแกนที่เครื่อง",
              message:"จำนวนที่ต้องสแกน : "+this.nameTag.piece+"<br>จำนวนวัตถุดิบ : "+this.nameTag.RFID.row,
              buttons: [
                {
                  text: 'ok',
                  handler: () => {
                  }
                },
              ]
            });
            alert.present();
            }

            if(this.nameTag.RFID.row < this.nameTag.piece && this.nameTag.Status =='D'){
              console.log(this.nameTag)
              let alert = this.alertCtrl.create({
                subTitle:"จำนวนคงเหลือในตู้แช่",
                message:"จำนวนวัตถุดิบ : "+this.nameTag.RFID.row,
                buttons: [
                  {
                    text: 'ok',
                    handler: () => {
                    }
                  },
                ]
              });
              alert.present();
              }

        
          
        //   setTimeout(()=>{
        //     // if(this.nameTag.RFID.row > 0){
        //     //   alert.dismiss();
        //     // }
        //     alert.dismiss();
        // }, 7000);
          
          
   }

   
   test(){
    console.log(this.sendKey)
   }

  selectEdditItem(editItems: any) {
    console.log(editItems)
    var itemthis5;
    
    this.dbf.list('/DB-RFID/', ref => ref.orderByChild('NumberPath').equalTo(editItems.NumberPath)).snapshotChanges()
      .subscribe(actions => {
        actions.forEach(action => {

          itemthis5 = action.key;
        });
       
        console.log(itemthis5)
       
      });
    this.actionSheetCtrl.create({
      title:`${editItems.Name}`,
      buttons: [
        {
          text: 'Edit',
          handler: () => {  
          this.navCtrl.push(EditPage,{edititemskey:itemthis5});

          }
        },
        {
          text: 'Delete',
          role: 'destructive',
          handler: () => {
            this.dbf.list('/DB-RFID/').remove(itemthis5);
            //this.dbf.list('/History/').remove(itemthis5);
            console.log(itemthis5);
          }
        },
        {
          text: 'Cancel',
          role: 'cancel',
          handler: () => {
            console.log("The user has selected the cancel button");
          }
        }
      ]

    }).present();

         
  }
 
}
