import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { BarcodeEditPage } from './barcode-edit';

@NgModule({
  declarations: [
    BarcodeEditPage,
  ],
  imports: [
    IonicPageModule.forChild(BarcodeEditPage),
  ],
})
export class BarcodeEditPageModule {}
