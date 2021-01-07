import { Component, OnInit } from '@angular/core';
import { VERSION} from '../../../environments/version';
import { faCircle } from '@fortawesome/free-solid-svg-icons';
import {environment} from "../../../environments/environment";
import {WebsocketService} from "../../modules/websocket";
@Component({
  selector: 'app-header',
  templateUrl: './header.component.html',
  styleUrls: ['./header.component.scss']
})
export class HeaderComponent implements OnInit {
  title = environment.applicationName;
  version = `v${VERSION.version}.${VERSION.hash}`;
  faCircle = faCircle;
  currentMS = undefined;

  constructor(private socket: WebsocketService) {


  }

  ngOnInit(): void {
    this.pingLoop();

  }

  pingLoop(){
    let lastPing = undefined;


    this.socket.addEventListener("ping-me").subscribe((x) => {
      let now = new Date().getTime();
      let dy = now - lastPing;
      lastPing = undefined;
      this.currentMS = dy;

    })
    let connFn = () => {
      this.socket.sendMessage("ping-me", "")
      if(lastPing != undefined){
        // Did not get response in time. means we are disconnected.
        this.currentMS = undefined;
      }
      lastPing = new Date().getTime();

    };
    setInterval(connFn, 5000)
    setTimeout(connFn, 100);

  }
}
