import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { AddbarcodePage } from './addbarcode';

@NgModule({
  declarations: [
    AddbarcodePage,
  ],
  imports: [
    IonicPageModule.forChild(AddbarcodePage),
  ],
})
export class AddbarcodePageModule {}
