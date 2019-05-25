import { Component , ViewChild} from '@angular/core';
import { IonicPage, NavController, NavParams, PopoverController, AlertController , Select} from 'ionic-angular';
import { FirebaseProvider } from './../../providers/firebase/firebase';
//import { FirebaseListObservable  } from "angularfire2/database-deprecated";
//import { FirebaseListObservable } from 'angularfire2/database';
import Moment from 'moment'
import firebase from 'firebase';
import { AngularFireDatabase } from 'angularfire2/database';
//import { user } from '../../providers/firebase/LoginUser';
import { user } from '../../providers/firebase/LoginUser';
import { items } from '../../providers/firebase/item.model';

@IonicPage()

@Component({
  selector: 'page-very',
  templateUrl: 'very.html',
})
export class VeryPage {
  
  public itemlist$: Array<items[]>;
  public itemList1: Array<any>;
  public itemList2: Array<any>;
  public itemList3: Array<any>;
  public itemList4: Array<any>;
  public loadedItemList: Array<any>;
  public expitem = '' ;
  public newItem = '';
  public firebase
  //lists: FirebaseListObservable<any[]>;
  public status = 'w';
  public numrow :any;
  public nub:any ;
  public timestam :any;
  public item = {};
  public countPiece  ;
  public piece = 0 ;
  public CountDay = 0;
  public text:string
  public addBy:string
  public login:any
  public numberLogin:number
  public RFID = 0;
  selected:any
  public colorPiece = '#F8F8FF'
  public colorButton = '#33CCFF'
  public colorBG = '#DCDCDC'
  public numHis:any

  @ViewChild('mySelect1') selectRef1: Select;
  @ViewChild('mySelect2') selectRef2: Select;
  @ViewChild('mySelect3') selectRef3: Select;
  @ViewChild('mySelect4') selectRef4: Select;

  @ViewChild('mySelect5') selectRef5: Select;

  constructor(public navCtrl: NavController, public navParams: NavParams,public firebaseProvider: FirebaseProvider,
              public popoverCtrl: PopoverController,public popoverCtrl2: PopoverController,private alertCtrl: AlertController, public dbf: AngularFireDatabase,) {
    //this.text = this.navParams.get('text');
    //this.piece = this.countPiece ;
    //this.newItem = ' ';
  
    const nuw: firebase.database.Reference = firebase.database().ref('/numrow/num');
    nuw.on('value', itemSnapshot => {
      this.numrow = itemSnapshot.val();
      this.numrow = this.numrow+1;
    })
    const nuw2: firebase.database.Reference = firebase.database().ref('/numrowhis/num');
    nuw2.on('value', itemSnapshot => {
      this.numHis = itemSnapshot.val();
      this.numHis = this.numHis+1;
    })

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
    
    this.dbf.list<user>('/ล็อกอิน/').valueChanges().subscribe((res: user[]) => {
      let user = [];

      res.forEach((item) => {

        user.push(item);
        this.login = item.ID_login
      })
    });

          this.addBy = this.login;

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

  openSelect5()
  {
      this.selectRef5.open();
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

  refresh(){
   this.piece = 0 ;
   this.newItem ='';
   this.expitem = '';
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

  done(){
    let alert = this.alertCtrl.create({
      subTitle: 'เพิ่มวัตถุดิบสำเร็จ',
      buttons: ['OK']
    });
    alert.present();
  }
   
  additem() {
    
    this.nub = this.numrow;
    console.log(this.addBy)
    
    if(this.expitem == '' && this.newItem == '' && this.piece == 0 )
    {
      this.falseAlert();
      return false;
    }
    // if(this.newitem == ''){ this.inputItemAlert(); 
    //     return false;
    //   }
    if(this.expitem == ''){ this.inputExpAlert(); 
        return false;
      }
    if(this.piece == 0){ this.inputPieceAlert(); 
      return false;
      }
      
    const itemRef: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.numrow);
    itemRef.on('value', itemSnapshot => {
      this.item = itemSnapshot.val();
    });


    this.addBy = this.login;
    //this.addBy = 'Boo432'
    this.timestam = Moment().format("YYYY-MM-DD HH:mm");
    this.done(); //alert message
    const items2: firebase.database.Reference = firebase.database().ref('/History/'+this.numrow);
    items2.update({    
    Name: this.newItem,
    piece: this.piece,
    timeKey: this.timestam =Moment().format("YYYY-MM-DD HH:mm"),
    addBy: this.addBy,
    category:"RFID",
    NumberPath:this.numrow,
    export:0,
    expitem:this.expitem,
    
    
    })
    const items3: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.numrow);
    items3.update({    
     
        Status: this.status,
        Name:   this.newItem,
        timeKey: this.timestam,
        piece: this.piece,
        expitem: this.expitem,
        addBy: this.addBy ,
        RFID:this.RFID,
        NumberPath:this.numrow,
        history:this.numHis
    })
    const items5: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+this.numrow+'/RFID/');
    items5.set({
      row:0,
    })
   // this.expitem = new Date().toLocaleDateString();
    // this.firebaseProvider.addItem(this.newItem,this.expitem,this.status,this.timestam,this.numrow,this.piece,this.CountDay,this.addBy,this.RFID);   //<-------------------------------------
    // this.firebaseProvider.expItem(this.expitem);
    this.newItem = '';
    this.piece = 0;
    this.expitem = '';
    this.CountDay = 0;
    


   

   const items1: firebase.database.Reference = firebase.database().ref('/numrow/');
  items1.update({    
    num:this.numrow
    })
    const itemsH: firebase.database.Reference = firebase.database().ref('/numrowhis/');
    itemsH.update({    
    num:this.numHis
    })
   //this.numrow ++ 

  }
  
 

  ionViewDidLoad() {
    
  }

  removeItem(id) {
    this.firebaseProvider.removeItem(id);
  }
  

  // doadd() {
  //   let alert = this.alertCtrl.create();
  //   alert.setTitle('เลือกวัตถุดิบ');

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'เนื้อหมู',
  //     value: 'เนื้อหมู',
  //     checked: true
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'เนื้อวัว',
  //     value: 'เนื้อวัว'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'เนื้อไก่',
  //     value: 'เนื้อไก่'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'ปลากระพง',
  //     value: 'ปลากระพง'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'ผักกาด',
  //     value: 'ผักกาด'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'กะหล่ำปลี',
  //     value: 'กะหล่ำปลี'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'มันฝรั่ง',
  //     value: 'มันฝรั่ง'
  //   });

  //   alert.addInput({
  //     type: 'radio',
  //     label: 'มะเขือเทศ',
  //     value: 'มะเขือเทศ'
  //   });

  //   alert.addButton('Cancel');
  //   alert.addButton({
  //     text: 'Ok',
  //     handler: (data: any) => {
  //       //console.log('Radio data:', data);
  //       this.newItem = data;
  //     }
  //   });

  //   alert.present();
  // }

}

