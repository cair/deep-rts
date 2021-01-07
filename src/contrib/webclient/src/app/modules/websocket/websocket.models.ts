import Dexie from 'dexie';

import { IMessage, IWsMessage } from './websocket.interfaces';
import { WS_API } from './websocket.events';

class MessagesDatabase extends Dexie {
    public messages!: Dexie.Table<IMessage, number>; // id is number in this case

    constructor() {
        super('MessagesDatabase');

        this.version(1).stores({
            messages: '++id,text'
        });
    }
}


export const modelParser = (message: IWsMessage) => {
    if (message && message.buffer) {
        /* binary parse */
        const encodeUint8Array = String.fromCharCode
            .apply(String, new Uint8Array(message.buffer.data));

        const parseData = JSON.parse(encodeUint8Array);

        let MessagesDB: MessagesDatabase;

        if (message.event === WS_API.EVENTS.MESSAGES) { // if messages set IMessage[]
            if (!MessagesDB) {
                MessagesDB = new MessagesDatabase();
            }

            parseData.forEach((messageData: IMessage) => {
                /* create transaction */
                MessagesDB.transaction('rw', MessagesDB.messages, async () => {

                    /* if not item */
                    if ((await MessagesDB.messages
                        .where({id: messageData.id}).count()) === 0) {

                        const id = await MessagesDB.messages
                            .add({id: messageData.id, text: messageData.text});

                        console.log(`Addded message with id ${id}`);
                    }

                }).catch(e => {
                    console.error(e.stack || e);
                });
            });

            return MessagesDB.messages.toArray();
        }

        if (message.event === WS_API.EVENTS.COUNTER) { // if counter set number
            return new Promise(r => r(parseData));
        }

        if (message.event === WS_API.EVENTS.UPDATE_TEXTS) { // if text set string
            const texts = [];

            parseData.forEach((textData: string) => {
                texts.push(textData);
            });

            return new Promise(r => r(texts));
        }

    } else {
        console.log(`[${Date()}] Buffer is "undefined"`);
    }
};
