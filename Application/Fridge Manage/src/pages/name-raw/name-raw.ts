import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { items } from '../../providers/firebase/item.model';
import { AngularFireDatabase } from 'angularfire2/database';
import firebase from 'firebase';
/**
 * Generated class for the NameRawPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-name-raw',
  templateUrl: 'name-raw.html',
})

export class NameRawPage {
  public itemlist$: Array<items[]>;
  public itemList1: Array<any>;
  public itemList2: Array<any>;
  public itemList3: Array<any>;
  public itemList4: Array<any>;
  public loadedItemList: Array<any>;
  public select:any;
  public nameItem='';

  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase) {


    this.dbf.list<items>(`/Raw-Material/Meat`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item1) => {
            itemes.push(item1);
      })

      this.itemList1 = itemes;
      //console.log(this.itemList1.length)
      this.loadedItemList = itemes;
    });

    this.dbf.list<items>(`/Raw-Material/Vegetable`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item2) => {
            itemes.push(item2);
      })

      this.itemList2 = itemes;
      //console.log(this.itemList2)
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


  }// endconstuct
  
  refresh(){
    this.nameItem = this.nameItem = ''
    this.select = this.select=''
    //console.log(this.nameItem)
  }

  additem(){
    //console.log(this.select)
    if(this.select == 'เนื้อสัตว์'){
      this.itemList1.length = this.itemList1.length + 1 ;
      const items1: firebase.database.Reference = firebase.database().ref('/Raw-Material/Meat/'+this.itemList1.length);
        items1.update({    
        Name:this.nameItem,
        })
        this.nameItem = this.nameItem = ''
    }//end if

    if(this.select == 'ผัก'){
      this.itemList2.length = this.itemList2.length + 1 ;
      const items2: firebase.database.Reference = firebase.database().ref('/Raw-Material/Vegetable/'+this.itemList2.length);
        items2.update({    
        Name:this.nameItem
        })
        this.nameItem = this.nameItem = ''
    }//end if

    if(this.select == 'ทะเล'){
      this.itemList3.length = this.itemList3.length + 1 ;
      const items3: firebase.database.Reference = firebase.database().ref('/Raw-Material/Sea/'+this.itemList3.length);
        items3.update({    
        Name:this.nameItem
        })
        this.nameItem = this.nameItem = ''
    }//end if

    if(this.select == 'อื่นๆ'){
      this.itemList4.length = this.itemList4.length + 1 ;
      const items4: firebase.database.Reference = firebase.database().ref('/Raw-Material/Other/'+this.itemList4.length);
        items4.update({    
        Name:this.nameItem
        })
        this.nameItem = this.nameItem = ''
    }//end if

    

  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad NameRawPage');
  }

}
