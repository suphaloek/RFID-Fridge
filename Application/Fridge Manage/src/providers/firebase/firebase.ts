import { Injectable } from '@angular/core';
import { AngularFireDatabase } from 'angularfire2/database';
//import { Item } from '../../../node_modules/ionic-angular/umd';
import firebase from '../../../node_modules/firebase';

@Injectable()
export class FirebaseProvider {
 
 
  constructor(public afd: AngularFireDatabase) {

   }

 /* addItem(name,exp,status) {

    let toSave = {
      ID: status,
      ชื่อ: name,
      วันหมดอายุ: exp
    }
      return this.afd.list('/วัตถุดิบ/').push(toSave);
}*/


addItem(newitem,expitem,status,timestam,numrow,piece,CountDay,addBy,RFID): 
  void {

    const items: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+numrow);

    items.set({
      Status: status,
      Name: newitem,
      timeKey: timestam,
      piece: piece,
      expitem: expitem,
      CountDay: CountDay,
      addBy: addBy ,
      RFID: RFID,
      //numrow: numrow,
  })
  const items2: firebase.database.Reference = firebase.database().ref('/DB-RFID/'+numrow+'/RFID/');

    items2.set({
      row:0,
      //numrow: numrow,
  })
    
}



/*showItem() {
firebase.database().ref('/explore/').once('value', (snapshot) => {

});
  
}*/

/*
getaddingItems() {
  return this.afd.list('วัตถุดิบ/');
}*/
 
  removeItem(id) {
    this.afd.list('DB-RFID/').remove(id);
  }
  
}


