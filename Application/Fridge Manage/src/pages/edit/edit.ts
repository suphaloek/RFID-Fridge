import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, AlertController } from 'ionic-angular';
//import { FirebaseListObservable } from '../../../node_modules/angularfire2/database-deprecated';
import { AngularFireDatabase, AngularFireObject } from 'angularfire2/database';
import { items } from '../../providers/firebase/item.model';
//import { query } from '../../../node_modules/@angular/core/src/render3/instructions';
import { Observable } from 'rxjs/observable'
//import { FirebaseObjectObservable, FirebaseListObservable } from 'angularfire2/database-deprecated';
//import { FirebaseObjectObservable } from 'a'

@IonicPage()
@Component({
  selector: 'page-edit',
  templateUrl: 'edit.html',
})

export class EditPage {
  public itemList: Array<any>;
  public loadedItemList: Array<any>;
  //public iece = 0 ;

  itemedit$: AngularFireObject<items>;
  itemLists: items = {

    Name: "",
    Status: "",
    expitem: "",
    piece: 1,
    timeKey: "",
    NumberPath: 1,
    
  };
  
  
  upPiece(){this.itemLists.piece = this.itemLists.piece + 1 }
  downPiece(){
    this.itemLists.piece = this.itemLists.piece - 1
    if(this.itemLists.piece < 0)
    {
      this.itemLists.piece = 0;
    }
   }

  shoppingItems: Observable<any[]>;


  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase,private alertCtrl: AlertController) 
  {
    const edititemkey = this.navParams.get('edititemskey')
    console.log(edititemkey);


    this.dbf.object<items>(`/DB-RFID/${edititemkey}`).valueChanges().subscribe((data: items) => {
      this.itemLists.Name = data.Name;
      this.itemLists.Status = data.Status;
      this.itemLists.expitem = data.expitem;
      this.itemLists.piece = data.piece;
      this.itemLists.timeKey = data.timeKey;
      this.itemLists.NumberPath = data.NumberPath;
      
    })
    console.log(this.itemLists);
    /*
    this.dbf.list<items>(`/items/`+edititemkey).valueChanges().subscribe((res: items[]) => {
      let itemes = [];

      res.forEach((item) => {

        itemes.push(item);

      })

      

      this.itemList = itemes;
      this.loadedItemList = itemes;
      console.log(this.itemList);
      
    });
    */


  }
  doneEdit(){
    let alert = this.alertCtrl.create({
      subTitle: 'แก้ไขสำเร็จ',
      buttons: ['OK']
    });
    alert.present();
  }
  onclickAdd(item: any) {

    //var itemthis;
    this.dbf.list('/DB-RFID/', ref => ref.orderByChild('NumberPath').equalTo(item.NumberPath)).snapshotChanges()
      .subscribe(actions => {
        actions.forEach(action => {
          this.dbf.list(`/DB-RFID/`).update(action.key, item)
          this.dbf.list(`/History/`).update(action.key, item)
        });
      });

      this.doneEdit();
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad EditPage');
  }

}
