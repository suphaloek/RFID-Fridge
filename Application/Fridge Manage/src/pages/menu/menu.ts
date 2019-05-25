import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, App, AlertController } from 'ionic-angular';
//import { HomePage } from '../home/home';
import { MenuaddPage } from '../menuadd/menuadd';
import { BasketPage } from '../basket/basket';
import { LoginPage } from '../login/login';
import { user } from '../../providers/firebase/LoginUser';
import { AngularFireDatabase } from 'angularfire2/database';
import { NameRawPage } from '../name-raw/name-raw';
import { ReadcodePage } from '../readcode/readcode';
import { HistoryPage } from '../history/history';
import { ContactPage } from '../contact/contact';

/**
 * Generated class for the MenuPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-menu',
  templateUrl: 'menu.html',
})
export class MenuPage {
  public itemList: Array<any>;
  public itemlist$: Array<user[]>;
  public itemvale :any
  itemcsd: user = {

    Name: "",
    ID_login:"",
    Password:"",
    numLogin: 0,
  };

  constructor(public navCtrl: NavController, public navParams: NavParams,public appCtrl: App,private alertCtrl: AlertController,public dbf: AngularFireDatabase) {
  }
gotoitems()
{
  this.navCtrl.setRoot(NameRawPage);
}
additems()
{
  this.navCtrl.setRoot(MenuaddPage);
}
gotoBasket()
{
  this.navCtrl.setRoot(BasketPage);
}
gotoBarcode()
{
  this.navCtrl.setRoot(ReadcodePage);
}
gotoHitstory()
{
  this.navCtrl.setRoot(HistoryPage);
}
gotoContact()
{
  this.navCtrl.setRoot(ContactPage);
}


exit()
{
  // this.navCtrl.setRoot(LoginPage);
  // this.navCtrl.popToRoot();
  let alert = this.alertCtrl.create({
  title: 'ท่านต้องการออกจากระบบ?',
     
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'ยกเลิก',
        },
        {
        text: 'ออกจากระบบ',
          handler: () => {
            
            this.dbf.list<user>(`/ล็อกอิน/`).snapshotChanges().subscribe((res) => {
              let itemes = [];
        
              res.forEach((item) => {
              this.itemvale = item.payload.val()
              itemes.push(this.itemvale);
              //console.log(this.itemvale);
              })     
              console.log(this.itemvale);
              this.dbf.list('/ล็อกอิน/').remove();
              });
              this.appCtrl.getRootNav().setRoot(LoginPage);
              
          }
        }
      ]});alert.present();
  

}

  ionViewDidLoad() {
    //console.log('ionViewDidLoad MenuPage');
  }

}
