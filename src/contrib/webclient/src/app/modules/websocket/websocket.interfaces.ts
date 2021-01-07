import { Options } from 'reconnecting-websocket';
import { Observable, Subject, ObjectUnsubscribedError } from 'rxjs';

export class MessageSubject<T> extends Subject<T> {

    constructor(
        private listeners: IListeners,
        private topic: string,
        private id: string
    ) {
        super();
    }

    public next(value?: T): void {
        if (this.closed) {
            throw new ObjectUnsubscribedError();
        }

        if (!this.isStopped) {
            const {observers} = this;
            const len = observers.length;
            const copy = observers.slice();

            for (let i = 0; i < len; i++) {
                copy[i].next(value);
            }

            if (!len) {
                this.garbageCollect();
            }
        }
    }

    /*
    * garbage collector
    * */
    private garbageCollect(): void {
        delete this.listeners[this.topic][this.id];

        if (!Object.keys(this.listeners[this.topic]).length) { // if not subjects
            delete this.listeners[this.topic];
        }
    }

}

export interface IWebsocketService {
    addEventListener<T>(topics: string[], id?: number): Observable<T>;
    runtimeIgnore(topics: string[]): void;
    runtimeRemoveIgnore(topics: string[]): void;
    sendMessage(event: string, data: any): void;
}

export interface WebSocketConfig {
    url: string;
    ignore?: string[];
    garbageCollectInterval?: number;
    options?: Options;
    pingInterval?: number;
}

export interface ITopic<T> {
    [hash: string]: MessageSubject<T>;
}

export interface IListeners {
    [topic: string]: ITopic<any>;
}

export interface IBuffer {
    type: string;
    data: number[];
}

export interface IWsMessage {
    event: string;
    buffer: IBuffer;
}

export interface IMessage {
    id: number;
    text: string;
}

export type ITopicDataType = IMessage[] | number | string[];
