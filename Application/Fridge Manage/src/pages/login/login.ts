import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, AlertController, App } from 'ionic-angular';
import { user } from '../../providers/firebase/LoginUser';
import { AngularFireDatabase } from 'angularfire2/database';
import firebase from 'firebase';
//import { HomePage } from '../home/home';
import { TabsPage } from '../tabs/tabs';
import { AdminPage } from '../admin/admin';


@IonicPage()
@Component({
  selector: 'page-login',
  templateUrl: 'login.html',
})
export class LoginPage {
  calendars = [];
  id :any = ""
  pass:any = ""
  public itemList: Array<any>;
  public itemlist$: Array<user[]>;
  public itemvale :any
  public i = 0;
  public numberLogin:any
  public bgcolor = '#DCDCDC';
  public buttoncolor = '#0033FF';

  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase,private alertCtrl: AlertController,public appCtrl: App) {
    const nuw: firebase.database.Reference = firebase.database().ref('/ล็อกอิน/numLogin');
    nuw.on('value', itemSnapshot => {
      this.numberLogin = itemSnapshot.val();
    })
    //this.dbf.list('/ล็อกอิน/'+this.id).remove(this.id);
    this.readID();
    }

  Login(){
      let alert = this.alertCtrl.create({
        subTitle: 'ล็อคอินสำเร็จ',
        buttons: ['OK']
      });
      alert.present();
    }
  
  AlertLogin(){
    let alert = this.alertCtrl.create({
      title: 'ล็อคอินไม่สำเร็จ',
      subTitle: 'กรุณาลองใหม่อีกครั้ง',
      buttons: ['OK']
    });
    alert.present();
  }
  failLogin(){
    let alert = this.alertCtrl.create({
      title: 'ล็อคอินไม่สำเร็จ',
      subTitle: 'รหัสผ่านไม่ถูกต้อง',
      buttons: ['OK']
    });
    alert.present();
  }

  readID(){

    this.dbf.list<user>(`/User-list/`).snapshotChanges().subscribe((res) => {
      let itemes = [];
      res.forEach((user) => {
      this.itemvale = user.payload.val()
      itemes.push(this.itemvale);
      this.itemlist$ = itemes;
  
      })         
      });
  }
   
  presentPrompt() {
    let alert = this.alertCtrl.create({
      title: 'Login Admin',
      inputs: [
        {
          name: 'Admin_ID',
          placeholder: 'Admin_ID'
        },
        {
          name: 'password',
          placeholder: 'Password',
          type: 'password'
        }
      ],
      buttons: [
        {
          text: 'Cancel',
          role: 'cancel',
          handler: data => {
            console.log('Cancel clicked');
          }
        },
        {
          text: 'Login',
          handler: data => {
            this.dbf.list<user>(`/Admin_ID/`).snapshotChanges().subscribe((res) => {
              if(data.Admin_ID == "" || data.password == "" ){
                this.AlertLogin();
                //return false;
                this.i++;
              }
             
             
             res.forEach((user) => {
              
              this.itemvale = user.payload.val()
              
              //console.log(this.itemvale)
               
                if(this.itemvale.ID_login == data.Admin_ID && this.itemvale.Password == data.password ){
                  this.i = this.i+1;
                  this.itemvale.ID_login
                  //ส่งId กับ ชื่อเข้า firebase
                  // const send: firebase.database.Reference = firebase.database().ref('/ล็อกอิน/'+this.itemvale.Name);
                  // send.update({
                  // ID_login: this.itemvale.ID_login,
                  // Name: this.itemvale.Name,
                  // })
                  this.navCtrl.setRoot(AdminPage);
                  
                   
                }
                
        
                console.log(this.itemvale.ID_login)
                console.log(this.itemvale.Password)
        

                })
                
                if(this.i == 0){
               
                  if(this.itemvale.ID_login != this.id || this.itemvale.Password != this.pass){
                    this.failLogin();
                    this.i = this.i+1;
                  }
                  }
                
              
              });
          



          }
        }
      ]
    });
    alert.present();
  }


  doLogin() {

        this.dbf.list<user>(`/User-list/`).snapshotChanges().subscribe((res) => {
        if(this.id == "" || this.pass == "" ){
          this.AlertLogin();
          //return false;
          this.i++;
        }
       
      
       res.forEach((user) => {
        
        this.itemvale = user.payload.val()
        
        //console.log(this.itemvale)
         
          if(this.itemvale.ID_login == this.id && this.itemvale.Password == this.pass ){
            this.i = this.i+1;
            this.itemvale.ID_login
            //ส่งId กับ ชื่อเข้า firebase
            

            const send: firebase.database.Reference = firebase.database().ref('/ล็อกอิน/'+this.itemvale.ID_login);
            send.update({
            ID_login: this.itemvale.ID_login,
            Name: this.itemvale.Name,
            
            })
            // const login: firebase.database.Reference = firebase.database().ref('/ล็อกอิน/');
            // login.update({    
            //   Login:this.itemvale.ID_login
            //   })
            this.Login();
            this.appCtrl.getRootNav().setRoot(TabsPage);
             
          }
          
       
          console.log(this.itemvale.ID_login)
          
         
          })
          
          if(this.i == 0){
         
            if(this.itemvale.ID_login != this.id || this.itemvale.Password != this.pass){
              this.failLogin();
              this.i = this.i+1;
            }
            }
          
        
        });
       
    
    }


}