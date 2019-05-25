import { Component } from '@angular/core';
import { IonicPage } from 'ionic-angular';
import { HomePage } from '../home/home';
import { BasketPage } from '../basket/basket';
import { BarcodePage } from '../barcode/barcode';
import { MenuPage } from '../menu/menu';
import { user } from '../../providers/firebase/LoginUser';
import { AngularFireDatabase } from 'angularfire2/database';
import { HistoryPage } from '../history/history';
import { ReadcodePage } from '../readcode/readcode';

@IonicPage()
@Component({
  selector: 'page-tabs',
  templateUrl: 'tabs.html',
})

export class TabsPage {
  
  homePage = HomePage
  loadPage = BasketPage
  reportPage = HistoryPage
  barcodePage = BarcodePage
  menuPage = MenuPage
  scanPage = ReadcodePage
  login:any

  constructor(public dbf: AngularFireDatabase) {

  this.dbf.list<user>('/ล็อกอิน/').valueChanges().subscribe((res: user[]) => {
    let user = [];

    res.forEach((item) => {

      user.push(item);
      this.login = item.ID_login
    })
  });
}


}
