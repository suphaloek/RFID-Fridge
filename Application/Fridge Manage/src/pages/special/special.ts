import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, ActionSheetController } from 'ionic-angular';
import { FirebaseListObservable } from '../../../node_modules/angularfire2/database-deprecated';
//import firebase from '../../../node_modules/firebase';
import { AngularFireDatabase, AngularFireList } from 'angularfire2/database'
import { items } from '../../providers/firebase/item.model';
//import { text } from '../../../node_modules/@angular/core/src/render3/instructions';
import { EditPage } from '../edit/edit' ;

AngularFireDatabase
@IonicPage()
@Component({
  selector: 'page-special',
  templateUrl: 'special.html',
})
export class SpecialPage {

  public itemList: Array<any>;
  public loadedItemList: Array<any>;
  //
  public itemlistA: AngularFireList<any[]>;
  shoppingItems: FirebaseListObservable<any[]>;
  public itemssss;
  itemcsd: items = {

    Name: "",
    Status: "",
    expitem: "",
    piece: 1,
    timeKey: "",
    NumberPath: 1,
  };


  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase, private actionSheetCtrl: ActionSheetController) {
    /*
       const nuw: firebase.database.Reference = firebase.database().ref('/items');
   
       nuw.on('value', itemList => {
         let itemes = [];
         itemList.forEach(itemry => {
           itemes.push(itemry.val());
           return false;
         });
   
         this.itemList = itemes;
         this.loadedItemList = itemes;
       });
   */

    this.dbf.list<items>(`/items`).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item) => {

        itemes.push(item);
        
      })

      this.itemList = itemes;
      this.loadedItemList = itemes;
    });

  }





  initializeItems(): void {
    this.itemList = this.loadedItemList;
  }

  getItems(searchbar) {
    // Reset items back to all of the items
    this.initializeItems();
    var q = searchbar.srcElement.value;

    // set q to the value of the searchbar
    //var q = searchbar.srcElement.value;

    //console.log(q);
    // if the value is an empty string don't filter the items
    if (!q) {
      return;
    }

    this.itemList = this.itemList.filter((v) => {
      console.log(v);
      if (v.Name && q) {
        if (v.Name.toLowerCase().indexOf(q.toLowerCase()) > -1) {
          return true;
        }
        return false;
      }
    });
    //this.itemList = this.


    // console.log(q, this.itemList.length);

  }
  selectEdditItem(editItems: any) {

    var itemthis;
    this.dbf.list('/items/', ref => ref.orderByChild('timeKey').equalTo(editItems.timeKey)).snapshotChanges()
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
          this.navCtrl.push(EditPage,{edititemskey:itemthis});

          }
        },
        {
          text: 'Delete',
          role: 'destructive',
          handler: () => {
            this.dbf.list('/items/').remove(itemthis);
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
    console.log('ionViewDidLoad SpecialPage');
  }

}
