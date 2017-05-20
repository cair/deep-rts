/**
 * Created by perar on 20.05.2017.
 */

var express = require('express');
var path = require('path');
var app = express();
var port = 3000;

app.listen(port, function () {
    console.log('DeepRTS Frontend listening on port ' + port + '!')
});

app.use('/', express.static(path.join(__dirname, 'public_html')));


/*
var WebSocketServer = require('uws').Server;
var wss = new WebSocketServer({ port: 3001 });

function onMessage(message) {
    console.log('received: ' + message);
}

wss.on('connection', function(ws) {
    ws.on('message', onMessage);
    ws.send('something');
});
*/
