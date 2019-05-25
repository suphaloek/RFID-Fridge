import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { VeryPage } from '../very/very';
//import { BarcodePage } from '../barcode/barcode';
import { AddbarcodePage } from '../addbarcode/addbarcode';

@IonicPage()
@Component({
  selector: 'page-menuadd',
  templateUrl: 'menuadd.html',
})
export class MenuaddPage {
  public colorPiece = '#F8F8FF'
  public colorButton = '#FF3366'
  public colorBG = '#DCDCDC'
  
  constructor(public navCtrl: NavController, public navParams: NavParams) {
  }

  toRFID() {
    this.navCtrl.push(VeryPage);
  }

  toBarcode() {
    this.navCtrl.push(AddbarcodePage);
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad MenuaddPage');
  }

}
