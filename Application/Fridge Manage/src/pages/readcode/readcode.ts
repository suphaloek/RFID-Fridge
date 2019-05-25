import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams , AlertController } from 'ionic-angular';
import { barcode } from '../../providers/firebase/barcode.model';
import { BarcodeScanner, BarcodeScannerOptions } from '@ionic-native/barcode-scanner';
import { AngularFireDatabase } from 'angularfire2/database';
import firebase from 'firebase';
/**
 * Generated class for the ReadcodePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-readcode',
  templateUrl: 'readcode.html',
})
export class ReadcodePage {
  public item = {};
  public item2 :any;
  data={};
  option:BarcodeScannerOptions;
  i = 1
  public code:string;
  public itemList: Array<barcode>;
  public loadedItemList: Array<any>;
  public itemlist$: Array<barcode[]>;
  public itemvale:any
  public itemvale2:any
  public tempCode:number
  public sum:any
  public sumpiece = 0
  public sumOut = 0
  public out = 0
  public piece = 0 ;

  constructor(public navCtrl: NavController, public navParams: NavParams, public barcodeScanner:BarcodeScanner, public dbf: AngularFireDatabase,private alertCtrl: AlertController) {
    if(this.i == 1){
      this.scan();
      this.i = 0;
      }
  }

  AlertSave(){
    let alert = this.alertCtrl.create({
      title: 'ทำรายการผิดพลาด',
      buttons: ['OK']
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

    this.dbf.list<barcode>(`/DB-barcode/`+this.code).snapshotChanges().subscribe((res) => {
      let itemes = [];
      res.forEach((barcode) => {
      this.itemvale = barcode.payload.val()
      itemes.push(this.itemvale);
      this.itemlist$ = itemes;
      //this.itemvale.piece
  
      })         
      });
      

      const itemRef: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.code);
      itemRef.on('value', itemSnapshot => {
        this.item = itemSnapshot.val();
        this.sumpiece = itemSnapshot.val().temp
      });
      // const itemRef5: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.code);
      // itemRef5.on('value', itemSnapshot2 => {
      //   this.out = itemSnapshot2.val();
      //   this.sumOut = itemSnapshot2.val().export
      // });
    }, (err) =>{
      console.log(err);
      

    });

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

  presentPrompt() {
    //var Export = 0
    let alert = this.alertCtrl.create({
      title: 'นำวัตถุดิบออก ?',
      // inputs: [
      //   {
      //     name: 'num',
      //     placeholder: 'กรอกจำนวน',
          
      //   },
      // ],
      buttons: [
        {
          text: 'ยกเลิก',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
  
        {
          text: 'ตกลง',
          handler: () => {
            //parseInt(data.num);
            
            // this.sum = this.sumpiece - data.num;
            this.sum = this.sumpiece - this.piece
            const itemRef2: firebase.database.Reference = firebase.database().ref('/History/'+this.code+'/export');
            itemRef2.on('value', itemSnapshot => {
              this.item2 = itemSnapshot.val();
            });

            if(this.sum >= 0 ){
              const items1: firebase.database.Reference = firebase.database().ref('/DB-barcode/'+this.code);
              items1.update({    
                temp:this.sum
                })
                const items2: firebase.database.Reference = firebase.database().ref('/History/'+this.code);
                items2.update({    
                export:parseInt(this.item2) + this.piece
                })
            }
            else{
              this.AlertSave();
              return false;
            }
            this.piece = 0
          }
        }
      ]
    });
    alert.present();
  }

  keyPiece() {
    //var Export = 0
    let alert = this.alertCtrl.create({
      title: 'ระบุจำนวน',
      inputs: [
        {
          name: 'keyPiece',
          // placeholder: 'กรอกจำนวน',
        },
      ],
      buttons: [
        {
          text: 'ยกเลิก',
          handler: () => {
            console.log('Cancel clicked');
          }
        },
  
        {
          text: 'ยืนยัน',
          handler: (data) => {
            this.piece = parseInt(data.keyPiece)
          }
        }
      ]
    });
    alert.present();
  }


  ionViewDidLoad() {
    console.log('ionViewDidLoad ReadcodePage');
  }

}
