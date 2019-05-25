import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, ActionSheetController } from 'ionic-angular';
import { BarcodeScanner } from '@ionic-native/barcode-scanner';
//import { ReadcodePage } from '../readcode/readcode';
import { AngularFireDatabase } from 'angularfire2/database';
//import firebase from 'firebase';
//import Moment from 'moment'
import { barcode } from '../../providers/firebase/barcode.model';
import { ReadcodePage } from '../readcode/readcode';
import { AddbarcodePage } from '../addbarcode/addbarcode';
import firebase from 'firebase';
import { BarcodeEditPage } from '../barcode-edit/barcode-edit';
import { LocalNotifications, ELocalNotificationTriggerUnit } from '@ionic-native/local-notifications';

@IonicPage()
@Component({
  selector: 'page-barcode',
  templateUrl: 'barcode.html',
})
export class BarcodePage {
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
  public colorButton1 = '#BA55D3'
  public colorButton2 = '#7CFC00'

  
  constructor(public navCtrl: NavController, public navParams: NavParams, public barcodeScanner:BarcodeScanner, public dbf: AngularFireDatabase,
    private actionSheetCtrl: ActionSheetController,public localNotifications: LocalNotifications, ) {
    this.CalculateDate();
    this.dbf.list<barcode>(`/DB-barcode/`).valueChanges().subscribe((res: barcode[]) => {
      let barcode = [];

      res.forEach((item) => {

        barcode.push(item);

      })

      this.itemList = barcode;
      this.loadedItemList = barcode;
    });

  }

  
  toAdd()
  {
    this.navCtrl.push(AddbarcodePage);
  }
  toItem()
  {
     this.navCtrl.push(ReadcodePage);
  }

  CalculateDate()
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
          console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด")

        }else if(this.startMonth > this.expMonth && this.NumberYear == 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด")   //<--------------------------------

        }else if(this.startMonth < this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนน้อยกว่าเดือนหมด")   //<--------------------------------

        }else if(this.startMonth > this.expMonth && this.NumberYear < 0){
          this.NumberMonth = 0;
          console.log("ปีเท่ากัน แต่เดือนมากกว่าเดือนหมด")   //<--------------------------------

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

        if(this.itemvale.piece == 0){
          this.dbf.list('/DB-barcode/'+this.itemvale.code).remove();
          return false
        }

        if(this.NumberDate <= 3){
          this.localNotifications.schedule({
            
            title: 'มีวัตถุดิบใกล้หมดอายุ',
            text: 'Barcode => ' + this.itemvale.Name+' จะหมดอายุใน '+this.NumberDate+' วัน',
            trigger: { in: 2, unit: ELocalNotificationTriggerUnit.SECOND },
            foreground: true ,
           
         });
        }
        //this.color = '#ba1488';
     
        if(this.NumberDate == 0){
          console.log("หมดอายุ")
       
          this.itemvale.CountDay = "หมดอายุ";
          itemes.push(this.itemvale);
          this.itemlist$ = itemes;

          //เมื่อวัตถุดิบหมดอายุจะส่งไปตารางหมดอายุ
          // const items2: firebase.database.Reference = firebase.database().ref('/Expired/'+this.itemvale.code);
          // items2.update({
          // Name: this.itemvale.Name,
          // piece:this.itemvale.piece,
          // expitem: this.itemvale.expitem,
          // CountDay:"หมดอายุ" ,
          // category:'Barcode',
          // NumberPath:this.itemvale.code,

          // })//ตารางหมดอายุ
          
          return false
        }
        
        if(this.itemvale.piece > 0 && this.NumberDate != 0 ){
        const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.itemvale.code);
        
        items1.update({    
        CountDay:this.NumberDate.toString()
        })
        }
 
        this.itemvale.CountDay = this.NumberDate+" วัน" ;
        itemes.push(this.itemvale);
        this.itemlist$ = itemes;
        //console.log(this.itemlist$)

        // if(this.itemvale.piece == 0){
        //   this.dbf.list('/DB-barcode/').remove(this.itemvale.code);
        // }
        
        
        })
   
  

    });

    
  }
  delete(path2){
    console.log(path2)
    //parseInt(path2)
    this.removePath = path2.toString()
    //this.AlertRemove()
  }


  selectEdditItem(editItems: any) {

    var itemthis 
    this.dbf.list('/DB-barcode/', ref => ref.orderByChild('Name').equalTo(editItems.Name)).snapshotChanges()
      .subscribe(actions => {
        actions.forEach(action => {

          itemthis = action.key;
        });
      });

    this.actionSheetCtrl.create({
      title: `${editItems.Name}`,
      buttons: [
        {
          text: 'Edit',
          handler: () => {  
          this.navCtrl.push(BarcodeEditPage,{edititemskey:itemthis});

          }
        },
        {
          text: 'Delete',
          role: 'destructive',
          handler: () => {
            this.dbf.list('/DB-barcode/').remove(itemthis);
            console.log(itemthis);
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
    console.log('ionViewDidLoad BarcodePage');
  }

}
