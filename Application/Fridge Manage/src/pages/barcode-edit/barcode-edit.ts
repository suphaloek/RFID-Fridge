import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams,AlertController } from 'ionic-angular';
//import { FirebaseListObservable } from '../../../node_modules/angularfire2/database-deprecated';
import { AngularFireDatabase, AngularFireObject } from 'angularfire2/database';
import { Observable } from 'rxjs/observable'
import { barcode } from '../../providers/firebase/barcode.model';

@IonicPage()
@Component({
  selector: 'page-barcode-edit',
  templateUrl: 'barcode-edit.html',
})

export class BarcodeEditPage {
  public itemList: Array<any>;
  public loadedItemList: Array<any>;
  //public iece = 0 ;

  itemedit$: AngularFireObject<barcode>;
  itemLists: barcode = {

    Name: "",
    expitem: "",
    piece: 1,
    timeKey: "",
    code:"",
  };
  

  shoppingItems: Observable<any[]>;


  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase,private alertCtrl: AlertController) {
    const edititemkey = this.navParams.get('edititemskey')
    console.log(edititemkey);


    this.dbf.object<barcode>(`/DB-barcode/${edititemkey}`).valueChanges().subscribe((data: barcode) => {
      this.itemLists.Name = data.Name;
      this.itemLists.expitem = data.expitem;
      this.itemLists.piece = data.piece;
      this.itemLists.timeKey = data.timeKey;
      this.itemLists.code = data.code;
      
    })
    console.log(this.itemLists);
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
    this.dbf.list('/DB-barcode/', ref => ref.orderByChild('code').equalTo(item.code)).snapshotChanges()
      .subscribe(actions => {
        actions.forEach(action => {
          this.dbf.list(`/DB-barcode/`).update(action.key, item)
          this.dbf.list(`/History/`).update(action.key, item)
        });
      });
    //this.navCtrl.setRoot(BarcodePage);
      this.doneEdit();
  }

  upPiece(){this.itemLists.piece = this.itemLists.piece + 1 }
  downPiece(){
    this.itemLists.piece = this.itemLists.piece - 1
    if(this.itemLists.piece < 0)
    {
      this.itemLists.piece = 0;
    }
   }
  ionViewDidLoad() {
    console.log('ionViewDidLoad BarcodeEditPage');
  }

}
