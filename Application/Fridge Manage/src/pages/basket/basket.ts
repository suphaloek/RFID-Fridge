import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, AlertController } from 'ionic-angular';
import { AngularFireDatabase } from 'angularfire2/database';
import { basket } from '../../providers/firebase/basket.model';
import firebase from 'firebase';
/**
 * Generated class for the BasketPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-basket',
  templateUrl: 'basket.html',
})
export class BasketPage {
  public itemList: Array<any>;
  public itemlist$: Array<basket[]>;
  public itemvale :any
  public selectedRange1= 0;
  public selectedRange2= 0;
  public itemName1:string
  public itemName2:string
  public amount1 = 0 ; 
  public amount2 = 0 ;
  public temp1:any;
  public temp2:any;
 
  public bgcolor2 = '#9966FF'
  itemcsd: basket = {
    
    Name:"",
    Weight_per_piece:0,
    piece:1,
    
  };
  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase,private alertCtrl: AlertController) {
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
        Weight_per_piece: Math.abs(itemes[0].Weight_per_piece),
        piece:Math.round(this.amount1),
    
        })
      const items2: firebase.database.Reference = firebase.database().ref('/Loadcell/2');
        items2.update({    
        Weight_per_piece: Math.abs(itemes[1].Weight_per_piece),
        piece:Math.round(this.amount2),

        })
    });
    
  }

  edit_1Name(){
    let alert = this.alertCtrl.create({
      title: 'แก้ไขชื่อ',
      inputs: [
        {
          name: 'Name',
          placeholder: 'ชื่อ'
        },
      ],
      buttons: [
        {
          text: 'Cancel',
          role: 'cancel',
        },
        {
          text: 'Save',
          handler: data => {
          if(data.Name != ''){
            
            const items1: firebase.database.Reference = firebase.database().ref('/Loadcell/1');
            items1.update({    
            Name:data.Name,
            
        })
            
        }
        }
        }
      ]
    });
    alert.present();
  }

  edit_1Weight(){
    let alert = this.alertCtrl.create({
      title: 'ระบุน้ำหนักต่อชิ้น',
      inputs: [
      {
        name: 'Weight',
        placeholder: 'Weight',
      }
      ],
      buttons: [
        {
          text: 'Cancel',
          role: 'cancel',
        },
        {
          text: 'Save',
          
        handler: data => {
        this.amount1 = (this.selectedRange1 / (data.Weight/1000))  ;
        const items2: firebase.database.Reference = firebase.database().ref('/Loadcell/1');
        items2.update({    
        piece:this.amount1,
        weight:parseInt(data.Weight),
        })
      }
      }
    ]
    });
    alert.present();
  }


  edit_2Name(){
    let alert = this.alertCtrl.create({
      title: 'แก้ไขชื่อ',
      inputs: [
        {
          name: 'Name',
          placeholder: 'ชื่อ'
        },
      ],
      buttons: [
        {
          text: 'Cancel',
          role: 'cancel',
        },
        {
          text: 'Save',
          handler: data => {
          if(data.Name != ''){
            
            const items1: firebase.database.Reference = firebase.database().ref('/Loadcell/2');
            items1.update({    
            Name:data.Name,
            
        })
            
        }
        }
        }
      ]
    });
    alert.present();
  }



  edit_2Weight(){
    let alert = this.alertCtrl.create({
      title: 'ระบุน้ำหนักต่อชิ้น',
      inputs: [
      {
        name: 'Weight',
        placeholder: 'Weight',
      }
      ],
      buttons: [
        {
          text: 'Cancel',
          role: 'cancel',
        },
        {
          text: 'Save',
          
        handler: data => {
        this.amount2 = (this.selectedRange2 / (data.Weight/1000))  ;
        const items2: firebase.database.Reference = firebase.database().ref('/Loadcell/2');
        items2.update({    
        piece:this.amount2,
        weight:parseInt(data.Weight),
        })
      }
      }
    ]
    });
    alert.present();
  }

  // close(){
  //   alert(this.selectedRange1)
  // }

  // save(){
  //   alert(this.selectedRange1)
  // }


  ionViewDidLoad() {
    //console.log('ionViewDidLoad BasketPage');
  }

}
