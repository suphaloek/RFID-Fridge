import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { NameRawPage } from './name-raw';

@NgModule({
  declarations: [
    NameRawPage,
  ],
  imports: [
    IonicPageModule.forChild(NameRawPage),
  ],
})
export class NameRawPageModule {}
