import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams, AlertController, App } from 'ionic-angular';
import firebase from 'firebase';
import { user } from '../../providers/firebase/LoginUser';
import { AngularFireDatabase } from 'angularfire2/database';
//import { LoginPage } from '../login/login';
/**
 * Generated class for the AdminPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-admin',
  templateUrl: 'admin.html',
})
export class AdminPage {
  public itemList: Array<any>;
  public itemlist$: Array<[user]>;
  public itemvale :any
  i = 0;
  constructor(public navCtrl: NavController, public navParams: NavParams,private alertCtrl: AlertController,public dbf: AngularFireDatabase,public appCtrl: App) {
    this.dbf.list<user>(`/User-list/`).snapshotChanges().subscribe((res) => {
      let itemes = [];

      res.forEach((item) => {
      var itemvale = item.payload.val()
      itemes.push(itemvale);
      this.itemlist$ = itemes;
      // console.log( this.itemlist$.length)

      })         
      });
  }

  register()
  {
    let alert = this.alertCtrl.create({
      title: 'เพิ่มผู้ใช้ในระบบ',
      inputs: [
        {
          name: 'ชื่อ',
          placeholder: 'ชื่อ'
        },
        {
          name: 'username',
          placeholder: 'Username',
        },
        {
          name: 'password',
          placeholder: 'Password',
          type: 'password'
        },
        {
          name: 'Retry_password',
          placeholder: 'Retry Password',
          type: 'password'
        },
      ],
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'cancel',
        },
        {
          text: 'ยืนยัน',
          handler: data => {
            if(data.ชื่อ =='' || data.username =='' || data.password =='' || data.Retry_password =='' ){
              let alert = this.alertCtrl.create({
              title: 'กรอกข้อมูลไม่ครบ',
              buttons: ['OK']
              });
              alert.present();
            }// end if

            this.dbf.list<user>(`/User-list/`).snapshotChanges().subscribe((res) => {

              let itemes = [];
              res.forEach((item) => {
              this.itemvale = item.payload.val()
              itemes.push(this.itemvale);
            console.log(this.itemvale.ID_login);

            if(data.username == this.itemvale.ID_login){
              let alert = this.alertCtrl.create({
                title: 'มีผู้ใช้ในระบบแล้ว',
                buttons: ['OK']
              });
              alert.present();
              this.i = 1
              return false
            }//end if
          });
          

          if(this.i != 1 ){

          if(data.ชื่อ !='' && data.username !='' && data.password !='' && data.Retry_password !=''){
            if(data.Retry_password == data.password){
              const send: firebase.database.Reference = firebase.database().ref('/User-list/'+data.username);
                send.update({
                ID_login: data.username,
                Name: data.ชื่อ,
                Password: data.password,
                })
                let alert = this.alertCtrl.create({
                  title: 'เพิ่มผู้ใช้สำเร็จ',
                  buttons: ['OK']
                });
                alert.present();
                data.username = '';
                this.navCtrl.setRoot(AdminPage);
              }// end if
           if(data.Retry_password != data.password){
              let alert = this.alertCtrl.create({
              title: 'รหัสยืนยันไม่ตรงกัน',
              buttons: ['OK']
              });
              alert.present();
            }// end if
            }//end if
          }//end if this.i != 1 
          
          });//end readID
          }
        }
      ]
    });
    alert.present();
  }


  editPass(id:any)
  {
    let alert = this.alertCtrl.create({
      title: 'กรอกรหัสผ่านใหม่',
      inputs: [
        {
          name: 'password',
          placeholder: 'Password',
          type: 'password'
        },
        {
          name: 'Retry_password',
          placeholder: 'Retry Password',
          type: 'password'
        },
      ],
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'cancel',
        },
        {
          text: 'ยืนยัน',
          handler: data => {
          //parseInt(id)
          if(data.Retry_password != data.password){
            let alert = this.alertCtrl.create({
              title: 'รหัสผ่านไม่ตรงกัน',
              buttons: ['OK']
            });
            alert.present();
          }//end if

          if(data.Retry_password == data.password){
            const send: firebase.database.Reference = firebase.database().ref('/User-list/'+id);
            send.update({
            Password: data.password,
          })
          console.log(id);
          let alert = this.alertCtrl.create({
            title: 'เปลี่ยนรหัสผ่านเรียบร้อย',
            buttons: ['OK']
          });
          alert.present();
          }//end if
          }
        }
      ]
      });
    alert.present();
  }

  deleteUser(id:any){
    let alert = this.alertCtrl.create({
      title: 'ต้องการลบผู้ใช้หรือไม่',
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'cancel',
        },
        {
          text: 'ยืนยัน',
          handler: () => {
          this.dbf.list('/User-list/').remove(id);
          }
        }
      ]
      });
    alert.present();
 
  }
  closeAdmin()
  {
    this.appCtrl.getRootNav().setRoot('LoginPage');
    //this.dbf.list('/ล็อกอิน/Admin').remove();
  }

  ionViewDidLoad() {
    //console.log('ionViewDidLoad AdminPage');
  }

}
