import { Component, ViewChild } from '@angular/core';
import { IonicPage, NavController, NavParams, Select ,AlertController} from 'ionic-angular';
import { BarcodeScannerOptions, BarcodeScanner } from '@ionic-native/barcode-scanner';
import { AngularFireDatabase } from 'angularfire2/database';
import firebase from 'firebase';
import Moment from 'moment'
import { items } from '../../providers/firebase/item.model';
import { user } from '../../providers/firebase/LoginUser';
/**
 * Generated class for the AddbarcodePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-addbarcode',
  templateUrl: 'addbarcode.html',
})
export class AddbarcodePage {

  data={};
  option:BarcodeScannerOptions;
  public newItem = '';
  public expitem = '' ;
  public piece = 0 ;
  public addBy:string
  public timestam :any;
  public code = '';
  public itemlist$: Array<items[]>;
  public itemList1: Array<any>;
  public itemList2: Array<any>;
  public itemList3: Array<any>;
  public itemList4: Array<any>;
  public loadedItemList: Array<any>;
  public colorPiece = '#F8F8FF'
  public colorButton = '#33CCFF'
  public colorBG = '#DCDCDC'
  public login:any
  
  @ViewChild('mySelect1') selectRef1: Select;
  @ViewChild('mySelect2') selectRef2: Select;
  @ViewChild('mySelect3') selectRef3: Select;
  @ViewChild('mySelect4') selectRef4: Select;

  constructor(public navCtrl: NavController, public navParams: NavParams, public barcodeScanner:BarcodeScanner, public dbf: AngularFireDatabase,private alertCtrl: AlertController) {
    /// เลือกวัตถุดิบ
    this.dbf.list<items>(`/Raw-Material/Meat`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item1) => {
            itemes.push(item1);
      })

      this.itemList1 = itemes;
      this.loadedItemList = itemes;
    });

    this.dbf.list<items>(`/Raw-Material/Vegetable`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item2) => {
            itemes.push(item2);
      })

      this.itemList2 = itemes;
      this.loadedItemList = itemes;
    });

    this.dbf.list<items>(`/Raw-Material/Sea`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item3) => {
            itemes.push(item3);
      })

      this.itemList3 = itemes;
      this.loadedItemList = itemes;
    });

    this.dbf.list<items>(`/Raw-Material/Other`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item4) => {
            itemes.push(item4);
      })

      this.itemList4 = itemes;
      this.loadedItemList = itemes;
    });
    //// End เลือกวัตถุดิบ 

 

          
  }
  done(){
    let alert = this.alertCtrl.create({
      subTitle: 'เพิ่มวัตถุดิบสำเร็จ',
      buttons: ['OK']
    });
    alert.present();
  }

  openSelect1()
  {
      this.selectRef1.open();
  }
  openSelect2()
  {
      this.selectRef2.open();
  }
  openSelect3()
  {
      this.selectRef3.open();
  }
  openSelect4()
  {
      this.selectRef4.open();
  }

  Code_piece() {
    let alert = this.alertCtrl.create({
      title: 'รหัสของวัตถุดิบ',
      inputs: [
        {
          name: 'code',
        },
      ],
      buttons: [
        {
          text: 'Cancel',
          handler: () => {
            //console.log('Cancel clicked');
          }
        },
  
        {
          text: 'Save',
          handler: (data) => {
            
            this.code = data.code ;
          }
        }
      ]
    });
    alert.present();
  }

  Name_Raw() {
    let alert = this.alertCtrl.create({
      title: 'กรอกชื่อวัตถุดิบ',
      inputs: [
        {
          name: 'Name',
        },
      ],
      buttons: [
        {
          text: 'Cancel',
          handler: () => {
            //console.log('Cancel clicked');
          }
        },
  
        {
          text: 'Save',
          handler: (data) => {
            this.newItem = data.Name ;
          }
        }
      ]
    });
    alert.present();
  }

  Num_piece() {
    let alert = this.alertCtrl.create({
      title: 'จำนวนของวัตถุดิบ',
      inputs: [
        {
          name: 'piece',
        },
      ],
      buttons: [
        {
          text: 'Cancel',
          handler: () => {
            //console.log('Cancel clicked');
          }
        },
  
        {
          text: 'Save',
          handler: (data) => {
            
            this.piece = parseInt(data.piece) ;
          }
        }
      ]
    });
    alert.present();
  }

  scan()
  {
    this.option = {
    
      prompt: "Please scan you barcode"
    }
    this.barcodeScanner.scan(this.option).then((barcodeData)=>{
    this.data = barcodeData;
    this.code = barcodeData.text;

    }, (err) =>{
      console.log(err);
 

    });

  }
  


  refresh(){
    this.piece = 0 ;
    this.newItem ='';
    this.expitem = '';
    this.code = '';
   }


  upPiece(){this.piece = this.piece + 1}
  upPiece10(){this.piece = this.piece + 10}
  upPiece20(){this.piece = this.piece + 20}
  upPiece30(){this.piece = this.piece + 30}
  upPiece40(){this.piece = this.piece + 40}
  upPiece50(){this.piece = this.piece + 50}
  upPiece100(){this.piece = this.piece + 100}
  downPiece(){
    this.piece = this.piece - 1
    if(this.piece < 0)
    {
    this.piece = 0;
    }
  }


  Add()
  {
    this.dbf.list<user>('/ล็อกอิน/').valueChanges().subscribe((res: user[]) => {
      let user = [];

      res.forEach((item) => {

        user.push(item);
        this.login = item.ID_login
      })
      this.addBy = this.login;
      //this.addBy = 'Aoo235';
      
    console.log(this.login)

    if(this.expitem == '' && this.newItem == '' && this.piece == 0 )
    {
      this.falseAlert();
      return false;
    }
    if(this.code == ''){ this.codeAlert(); 
        return false;
      }
    if(this.expitem == ''){ this.inputExpAlert(); 
        return false;
      }
    if(this.piece == 0){ this.inputPieceAlert(); 
      return false;
      }

    this.timestam = Moment().format("YYYY-MM-DD HH:mm");
     const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.code);
    this.done();
    items1.update({    
    code: parseInt(this.code),
    Name: this.newItem,
    piece: this.piece,
    timeKey: this.timestam,
    expitem: this.expitem ,
    addBy: this.addBy,
    temp:this.piece,
    Out:'',
    })

    const items2: firebase.database.Reference = firebase.database().ref('/History/'+this.code);
    items2.update({    
    Name: this.newItem,
    piece: this.piece,
    timeKey: this.timestam =Moment().format("YYYY-MM-DD HH:mm"),
    addBy: this.addBy,
    category:"Barcode",
    NumberPath:this.code,
    export:0,
    expitem:this.expitem,
    
    })

    this.refresh();
  });
  //this.done(); //alert message
  }
  
  inputItemAlert() {
    let alert = this.alertCtrl.create({
      title: 'ทำรายการผิดพลาด',
      subTitle: 'ท่านไม่ได้กรอกชื่อวัตถุดิบ กรุณาทำรายการใหม่อีกครั้ง',
      buttons: ['OK']
    });
    alert.present();
  }
  inputExpAlert() {
    let alert = this.alertCtrl.create({
      title: 'ทำรายการผิดพลาด',
      subTitle: 'ท่านไม่ได้กรอกวันหมดอายุ กรุณาทำรายการใหม่อีกครั้ง',
      buttons: ['OK']
    });
    alert.present();
  }
  inputPieceAlert() {
    let alert = this.alertCtrl.create({
      title: 'คำเตือน!!',
      subTitle: 'กรุณากรอกจำนวน',
      buttons: ['OK']
    });
    alert.present();
  }
  falseAlert() {
    let alert = this.alertCtrl.create({
      title: 'ทำรายการใหม่อีกครั้ง',
      subTitle: 'กรุณากรอกข้อมูลวัตถุดิบ',
      buttons: ['OK']
    });
    alert.present();
  }
  codeAlert() {
    let alert = this.alertCtrl.create({
      title: 'ทำรายการใหม่อีกครั้ง',
      subTitle: 'กรุณาสแกนบาร์โค้ด',
      buttons: ['OK']
    });
    alert.present();
  }


  ionViewDidLoad() {
    //console.log('ionViewDidLoad AddbarcodePage');
  }

}
