import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams,AlertController } from 'ionic-angular';
import { items } from '../../providers/firebase/item.model';
import { AngularFireDatabase  } from 'angularfire2/database';
import firebase from 'firebase';
import Moment from 'moment'
/**
 * Generated class for the HistoryPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-history',
  templateUrl: 'history.html',
})
export class HistoryPage {
  public itemList: Array<any>;
  public itemlist$: Array<items[]>;
  public loadedItemList: Array<any>;
  public colorBG = '#DCDCDC'
  public qMonth:any
  public qYear:any
  public gMonth =''
  public gYear = ''
  public key:any
  public detail:any
  public year
  public month
  public day
  public expyear
  public expmonth
  public expday
  public itemvale :any
  public listMonth = ["January","February","March","April","May","June","July","August","September","October","November","December"]
  public listYear = ["2019","2020","2021","2022","2023","2024"]
  public showMonth:any
  public showYear:any
  public timestam ='';
  
  constructor(public navCtrl: NavController, public navParams: NavParams, public dbf: AngularFireDatabase,private alertCtrl: AlertController) {

        // this.dbf.list<items>(`/History`).valueChanges().subscribe((res: items[]) => {
        //   let itemes = [];
    
        //   res.forEach((item) => {
        //         itemes.push(item);
        //   })
    
        //   this.itemlist$ = itemes;
        //   this.loadedItemList = itemes;
        // });
        this.dbf.list<items>(`History`).valueChanges().subscribe((res: items[]) => {
          let itemes = [];
    
          firebase.database().ref('/History/').orderByChild('timeKey').on('value', function(snapshot) {
       
            snapshot.forEach(function(child) {
            itemes.push(child.val());
            }.bind(this));
            console.log(itemes)
          });
          this.itemList = itemes;
          this.loadedItemList = itemes;
        });
        
        this.showItem()
  }

  refresh(){
    this.gMonth = ''
    this.gYear = ''
    this.timestam = ''
    this.showMonth = ''
    this.showYear = ''
    this.showItem()
  }

  showItem(){
    if(this.gMonth == "01"){this.showMonth = this.listMonth[0]}
    if(this.gMonth == "02"){this.showMonth = this.listMonth[1]}
    if(this.gMonth == "03"){this.showMonth = this.listMonth[2]}
    if(this.gMonth == "04"){this.showMonth = this.listMonth[3]}
    if(this.gMonth == "05"){this.showMonth = this.listMonth[4]}
    if(this.gMonth == "06"){this.showMonth = this.listMonth[5]}
    if(this.gMonth == "07"){this.showMonth = this.listMonth[6]}
    if(this.gMonth == "08"){this.showMonth = this.listMonth[7]}
    if(this.gMonth == "09"){this.showMonth = this.listMonth[8]}
    if(this.gMonth == "10"){this.showMonth = this.listMonth[9]}
    if(this.gMonth == "11"){this.showMonth = this.listMonth[10]}
    if(this.gMonth == "12"){this.showMonth = this.listMonth[11]}
    
    if(this.gYear == "19"){this.showYear = this.listYear[0]}
    if(this.gYear == "20"){this.showYear = this.listYear[1]}
    if(this.gYear == "21"){this.showYear = this.listYear[2]}
    if(this.gYear == "22"){this.showYear = this.listYear[3]}
    if(this.gYear == "23"){this.showYear = this.listYear[4]}
    if(this.gYear == "24"){this.showYear = this.listYear[5]}
    
    //console.log(this.gMonth)

    if(this.gYear != '' && this.gMonth == ''){
    this.timestam = Moment().format("DD/MM/YY");
    this.dbf.list<items>(`/History/`).snapshotChanges().subscribe((res) => {
     let itemes = [];
     
      firebase.database().ref('/History/').orderByChild('timeKey').on('value', function(snapshot) {
       
        snapshot.forEach(function(child) {
        itemes.push(child.val());
        }.bind(this));
        console.log(itemes)
      });
     
     this.qMonth = this.itemvale.timeKey[5]+this.itemvale.timeKey[6]
     this.qYear = this.itemvale.timeKey[2]+this.itemvale.timeKey[3]
     //console.log(this.qMonth)
      //console.log(this.itemvale.export)
      if(this.gYear == this.qYear){
        //itemes.push(this.itemvale);
        this.itemList = itemes;
        }
    
    console.log(this.qYear)
    console.log(this.qMonth)
    });
   
    }

    if(this.gMonth != ''){
      this.timestam = Moment().format("DD/MM/YY");
      this.dbf.list<items>(`/History/`).snapshotChanges().subscribe((res) => {
        let itemes = [];
        firebase.database().ref('/History/').orderByChild('timeKey').on('value', function(snapshot) {
          snapshot.forEach(function(child) {
          itemes.push(child.val());
          }.bind(this));
          console.log(itemes)
        });
        this.qMonth = this.itemvale.timeKey[5]+this.itemvale.timeKey[6]
        this.qYear = this.itemvale.timeKey[2]+this.itemvale.timeKey[3]
        //console.log(this.qMonth)
         //console.log(this.itemvale.export)
         
           if(this.gMonth != this.qMonth){
           this.itemList = [];
           }
           
      
       });
      }

      
      this.dbf.list<items>(`/History/`).snapshotChanges().subscribe((res) => {
       let itemes = [];
      

       res.forEach((user) => {
       this.itemvale = user.payload.val()
      
       
       this.qMonth = this.itemvale.timeKey[5]+this.itemvale.timeKey[6]
       this.qYear = this.itemvale.timeKey[2]+this.itemvale.timeKey[3]
       //console.log(this.qMonth)
        //console.log(this.itemvale.export)
        if(this.gYear == '19'){
          if(this.gMonth == this.qMonth){
              itemes.push(this.itemvale)
              this.itemList = itemes;
          }
        }
        if(this.gYear != '19'){
          this.itemList = [];
          }
      
      })
      
      });
      
      //}

      //this.refresh()

  }

  initializeItems(): void {
    this.itemList = this.loadedItemList;
  }

  getMonth(datePicker) {
    // Reset items back to all of the items
    console.log(datePicker)
    this.initializeItems();
    
    var q = datePicker.srcElement.value;
    console.log(q)
    if (!q) {
      return;
    }

    this.itemList = this.itemList.filter((v) => {
      //console.log(v);
      if (v.timeKey && q) {
        if (v.timeKey.toLowerCase().indexOf(q.toLowerCase()) > -1) {
          return true;
        }
        return false;
      }
      
    });
    //this.itemList = this.


    // console.log(q, this.itemList.length);

  }  

  getitems(searchbar) {
    // Reset items back to all of the items
    this.initializeItems();
    var q = searchbar.srcElement.value;
    console.log(q)
    if (!q) {
      return;
    }
    this.itemList = this.itemList.filter((v) => {
      
      //console.log(v);
      if (v.Name && q) {
        console.log(v)
        if (v.Name.toLowerCase().indexOf(q.toLowerCase()) > -1) {
          return true;
        }
        return false;
      }
      
    });
    


    // console.log(q, this.itemList.length);

  }

  Alert(path){

    this.key = parseInt(path)
    const detail: firebase.database.Reference = firebase.database().ref('/History/'+this.key);
          detail.on('value', itemSnapshot => {
            this.detail = itemSnapshot.val();
          })
          this.year = this.detail.timeKey[0]+this.detail.timeKey[1]+this.detail.timeKey[2]+this.detail.timeKey[3]; 
          this.month = this.detail.timeKey[5]+this.detail.timeKey[6];
          this.day =  this.detail.timeKey[8]+ this.detail.timeKey[9];

          this.expyear = this.detail.expitem[0]+this.detail.expitem[1]+this.detail.expitem[2]+this.detail.expitem[3]; 
          this.expmonth = this.detail.expitem[5]+this.detail.expitem[6];
          this.expday = this.detail.expitem[8]+ this.detail.expitem[9];

    let alert = this.alertCtrl.create({
      subTitle: 'Detail',
      message:"ชื่อ : "+this.detail.Name+"<br>นำเข้าโดย : "+this.detail.addBy+"<br>วันที่นำเข้า : "+this.day+"/"+this.month+"/"+this.year+"<br>วันหมดอายุ : "+this.expday+"/"+this.expmonth+"/"+this.expyear+"<br>นำเข้าแบบ : "+this.detail.category,
      buttons: [
        {
          text: 'ลบ',
          handler: () => {
            let alert = this.alertCtrl.create({
              title: 'ต้องการลบวัตถุดิบ ?',
              buttons: [
                {
                  text: 'ยกเลิก',
                  role: 'cancel',
                },
                {
                  text: 'ลบ',
                  handler: () => {
                  this.dbf.list('/History/'+path).remove();
                  this.dbf.list('/DB-RFID/'+path).remove();
                  this.dbf.list('/DB-barcode/'+path).remove();
                  }
                }
              ]
              });
            alert.present();
            
          }
        },
        {
          text: 'ปิด',
          handler: () => {
          }
        },
      ]
    });
    alert.present();
  }

  clearAll(){
    let alert = this.alertCtrl.create({
      title: 'ล้างประวัติทั้งหมด ?',
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'cancel',
        },
        {
          text: 'ยืนยัน',
          handler: () => {
          
          }
        }
      ]
      });
    alert.present();
  }
  
  deleteHistory(id)
  {
    let alert = this.alertCtrl.create({
      title: 'ลบประวัติ ?',
      buttons: [
        {
          text: 'ยกเลิก',
          role: 'cancel',
        },
        {
          text: 'ยืนยัน',
          handler: () => {
          console.log(id)
            this.dbf.list('/History/'+id).remove();
          }
        }
      ]
      });
    alert.present();
  }

  

  ionViewDidLoad() {
    //console.log(this.Date)
  }

}
