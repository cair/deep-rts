import { Injectable, OnDestroy, Inject } from '@angular/core';
import { Observable } from 'rxjs';
import ReconnectingWebSocket from 'reconnecting-websocket';
import SHA256 from 'sha256-es';

import { IListeners, ITopic, ITopicDataType, IWebsocketService, MessageSubject, WebSocketConfig } from './websocket.interfaces';
import { config } from './websocket.config';




@Injectable({
    providedIn: 'root'
})
export class WebsocketService implements IWebsocketService, OnDestroy {

    private listeners: IListeners;
    private uniqueId: number;
    private websocket: ReconnectingWebSocket;

    constructor(@Inject(config) private wsConfig: WebSocketConfig) {
        this.uniqueId = -1;
        this.listeners = {};
        this.wsConfig.ignore = wsConfig.ignore ? wsConfig.ignore : [];

        // run connection
        this.connect();
    }

    ngOnDestroy() {
        this.websocket.close();
    }


    /*
    * connect to WebSocked
    * */
    private connect(): void {
        // ReconnectingWebSocket config
        const options = {
            connectionTimeout: 1000,
            maxRetries: 10,
            ...this.wsConfig.options
        };

        // connect to WebSocked
        this.websocket = new ReconnectingWebSocket(this.wsConfig.url, [], options);

        this.websocket.addEventListener('open', (event: Event) => {
            console.log(`[${Date()}] WebSocket connected!`);
        });

        this.websocket.addEventListener('close', (event: CloseEvent) => {
            console.log(`[${Date()}] WebSocket close!`);
        });

        this.websocket.addEventListener('error', (event: ErrorEvent) => {
            console.error(`[${Date()}] WebSocket error!`);
        });

        this.websocket.addEventListener('message', (event: MessageEvent) => {
            // dispatch message to subscribers
            this.onMessage(event);
        });

        setInterval(() => {
            if(this.websocket.readyState == 1){
                this.websocket.send("ping");
            }

        }, (this.wsConfig.pingInterval || 1000));

        setInterval(() => {
            this.garbageCollect(); // remove subjects without subscribe
        }, (this.wsConfig.garbageCollectInterval || 10000));
    }


    /*
    * garbage collector
    * */
    private garbageCollect(): void {
        for (const event in this.listeners) {
            if (this.listeners.hasOwnProperty(event)) {
                const topic = this.listeners[event];

                for (const key in topic) {
                    if (topic.hasOwnProperty(key)) {
                        const subject = topic[key];

                        if (!subject.observers.length) { // if not subscribes
                            delete topic[key];
                        }
                    }
                }

                if (!Object.keys(topic).length) { // if not subjects
                    delete this.listeners[event];
                }
            }
        }
    }


    /*
    * call messages to Subject
    * */
    private callMessage<T>(topic: ITopic<T>, data: T): void {
        for (const key in topic) {
            if (topic.hasOwnProperty(key)) {
                const subject = topic[key];

                if (subject) {
                    // dispatch message to subscriber
                    subject.next(data);
                } else {
                    console.log(`[${Date()}] Topic Subject is "undefined"`);
                }
            }
        }
    }


    /*
    * dispatch messages to subscribers
    * */
    private onMessage(event: MessageEvent): void {
        const message = JSON.parse(event.data);

        for (const name in this.listeners) {

            if (this.listeners.hasOwnProperty(name) && !this.wsConfig.ignore.includes(name)) {
                const topic = this.listeners[name];
                const keys = name.split('/'); // if multiple events
                const isMessage = keys.includes(message.event);
                const hasData = message.hasOwnProperty("data");
                const data = message.data;

                if(isMessage && hasData){
                    this.callMessage<ITopicDataType>(topic, data);
                }
            }
        }
    }


    /*
    * add topic for subscribers
    * */
    private addTopic<T>(topic: string, id?: number): MessageSubject<T> {
        const token = (++this.uniqueId).toString(); // token for personal subject
        const key = id ? token + id : token; // id for more personal subject
        const hash = SHA256.hash(key); // set hash for personal

        if (!this.listeners[topic]) {
            this.listeners[topic] = <any>{};
        }

        return this.listeners[topic][hash] = new MessageSubject<T>(this.listeners, topic, hash);
    }


    /*
    * subscribe method
    * */
    public addEventListener<T>(topics: string | string[], id?: number): Observable<T> {
        if (topics) {
            const topicsKey = typeof topics === 'string' ? topics : topics.join('/'); // one or multiple

            return this.addTopic<T>(topicsKey, id).asObservable();
        } else {
            console.log(`[${Date()}] Can't add EventListener. Type of event is "undefined".`);
        }
    }


    /*
    * on message to server
    * */
    public sendMessage(event: string, data: any = {}): void {
        if (event && this.websocket.readyState === 1) {
            this.websocket.send(JSON.stringify({
                "event": event,
                "data": data
            }));
        } else {
            console.log('Send error!');
        }
    }


    /*
    * runtime add ignore list
    * */
    public runtimeIgnore(topics: string[]): void {
        if (topics && topics.length) {
            this.wsConfig.ignore.push(...topics);
        }
    }


    /*
    * runtime remove from ignore list
    * */
    public runtimeRemoveIgnore(topics: string[]): void {
        if (topics && topics.length) {
            topics.forEach((topic: string) => {
                const topicIndex = this.wsConfig.ignore.findIndex(t => t === topic); // find topic in ignore list

                if (topicIndex > -1) {
                    this.wsConfig.ignore.splice(topicIndex, 1);
                }
            });
        }
    }

}
