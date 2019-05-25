import { FormsModule } from '@angular/forms';
import { MbscModule } from '@mobiscroll/angular';
import { BrowserModule } from '@angular/platform-browser';
import { ErrorHandler, NgModule } from '@angular/core';
import { IonicApp, IonicErrorHandler, IonicModule } from 'ionic-angular';
import { SplashScreen } from '@ionic-native/splash-screen';
import { StatusBar } from '@ionic-native/status-bar';

import { BasketPage } from '../pages/basket/basket' ;
import { MyApp } from './app.component';
import { HomePage } from '../pages/home/home';
import { VeryPage } from '../pages/very/very';
import { MenuaddPage } from '../pages/menuadd/menuadd';
import { ReadcodePage } from '../pages/readcode/readcode';
import { SpecialPage  } from '../pages/special/special';
//import { LoginPage } from '../pages/login/login' ;

import { HttpModule } from '@angular/http';
import { AngularFireDatabaseModule } from 'angularfire2/database';
import { AngularFireModule } from 'angularfire2';
import { FirebaseProvider } from '../providers/firebase/firebase';
import { EditPage } from '../pages/edit/edit';
import { BarcodePage } from '../pages/barcode/barcode';

import { BarcodeScanner } from '@ionic-native/barcode-scanner';
import { Calendar } from '@ionic-native/calendar';
import { LocalNotifications } from '@ionic-native/local-notifications';
import { TabsPage } from '../pages/tabs/tabs';
import { MenuPage } from '../pages/menu/menu';
//import { Camera } from '@ionic-native/camera';
import {RoundProgressModule} from 'angular-svg-round-progressbar';
import { AddbarcodePage } from '../pages/addbarcode/addbarcode';
import { BarcodeEditPage } from '../pages/barcode-edit/barcode-edit';
import { AdminPage } from '../pages/admin/admin';
import { NameRawPage } from '../pages/name-raw/name-raw';
import { HistoryPage } from '../pages/history/history';
import { ProgressPage } from '../pages/progress/progress';
import { IonicSimpleProgressBarModule, SimpleProgressBarProvider } from 'ionic-progress-bar';
import { ReportPage } from '../pages/report/report';
import { ContactPage } from '../pages/contact/contact';

const firebaseConfig = {
  apiKey: "AIzaSyBWAO2z5A22wAiAXHtg7o8k7v6XYm7DIN4",
  authDomain: "testdata-79638.firebaseapp.com",
  databaseURL: "https://testdata-79638.firebaseio.com",
  projectId: "testdata-79638",
  storageBucket: "testdata-79638.appspot.com",
  messagingSenderId: "404714504707"
};



@NgModule({
  
  declarations: [
    MyApp,
    HomePage,
    VeryPage,
    EditPage,
    MenuaddPage,
    BasketPage,
    ReadcodePage,
    SpecialPage,
    BarcodePage,
    TabsPage,
    MenuPage,
    AddbarcodePage,
    BarcodeEditPage,
    AdminPage,
    NameRawPage,
    HistoryPage,
    ProgressPage,
    ReportPage,
    ContactPage
  
 
   
  ],
  imports: [ 
    FormsModule, 
    MbscModule,
    BrowserModule,
    HttpModule,
    AngularFireDatabaseModule,
    AngularFireModule.initializeApp(firebaseConfig),
    IonicModule.forRoot(MyApp,{tabsPlacement: 'bottom'}),
    RoundProgressModule,
    IonicSimpleProgressBarModule.forRoot()
      
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    HomePage,
    VeryPage,
    EditPage,
    MenuaddPage,
    BasketPage,
    ReadcodePage,
    SpecialPage,
    BarcodePage,
    TabsPage,
    MenuPage,
    AddbarcodePage,
    BarcodeEditPage,
    AdminPage,
    NameRawPage,
    HistoryPage,
    ProgressPage,
    ReportPage,
    ContactPage
  

  ],
  providers: [
    StatusBar,
    SplashScreen,
    FirebaseProvider,
    { provide: ErrorHandler, useClass: IonicErrorHandler },
    BarcodeScanner,
    Calendar,
    LocalNotifications,
    SimpleProgressBarProvider
    
  ]
})
export class AppModule { }
