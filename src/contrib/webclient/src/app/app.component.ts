
import {Component, OnInit} from '@angular/core';
import { VERSION} from '../environments/version';
import { environment} from '../environments/environment';
import {ActivatedRoute, NavigationCancel, NavigationStart, Router} from "@angular/router";

@Component({
  selector: 'app-root',
  templateUrl: './app.component.html',
  styleUrls: ['./app.component.scss']
})
export class AppComponent implements OnInit{
  title = environment.applicationName;
  version = `v${VERSION.version}.${VERSION.hash}`;
  standalone = true;

  constructor(public router: Router) {
    router.events.subscribe(s => {
      if (s instanceof NavigationStart) {
        let url = s.url.split("?")[1]

        if(url === undefined){
          this.standalone = false;
          return;
        }

        let params = this.parseQuery(s.url.split("?")[1])

        if(params.hasOwnProperty("standalone") && params["standalone"] == '0'){
          this.standalone = false;
        }
      }
    });
  }

  parseQuery(queryString) {
    var query = {};
    var pairs = (queryString[0] === '?' ? queryString.substr(1) : queryString).split('&');
    for (var i = 0; i < pairs.length; i++) {
      var pair = pairs[i].split('=');
      query[decodeURIComponent(pair[0])] = decodeURIComponent(pair[1] || '');
    }
    return query;
  }

  ngOnInit(): void {

  }
}