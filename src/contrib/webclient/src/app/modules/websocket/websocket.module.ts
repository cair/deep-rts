import { NgModule, ModuleWithProviders } from '@angular/core';
import { CommonModule } from '@angular/common';

import { config } from './websocket.config';
import { WebSocketConfig } from './websocket.interfaces';


@NgModule({
    imports: [
        CommonModule
    ]
})
export class WebsocketModule {
  static forRoot(wsConfig: WebSocketConfig): ModuleWithProviders<WebsocketModule> {
    return {
      ngModule: WebsocketModule,
      providers: [{provide: config, useValue: wsConfig}]
    };
  }

}
