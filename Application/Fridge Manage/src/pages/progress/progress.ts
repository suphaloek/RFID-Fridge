import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';

/**
 * Generated class for the ProgressPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-progress',
  templateUrl: 'progress.html',
  template: `
    <ion-header>
      <ion-navbar>
        <ion-title>Page 2</ion-title>
      </ion-navbar>
    </ion-header>
    <ion-content padding>This is another page!</ion-content>
  `
})
export class ProgressPage {

  constructor(public navCtrl: NavController, public navParams: NavParams) {
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad ProgressPage');
  }

}
