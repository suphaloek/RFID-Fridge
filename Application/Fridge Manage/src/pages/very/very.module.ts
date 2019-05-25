import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { VeryPage } from './very';

@NgModule({
  declarations: [
    VeryPage,
  ],
  imports: [
    IonicPageModule.forChild(VeryPage),
  ],
  entryComponents: [
    VeryPage,
  ]
})
export class VeryPageModule {}
