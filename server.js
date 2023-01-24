var WebSocketServer = require('websocket').server;
var http = require('http');
const express =  require('express');
const app = express();

//glob variables...
var connections = [];


var server = app.listen(7000, function() {
    console.log((new Date()) + ' Server is listening on port 7000');
});



wsServer = new WebSocketServer({
    httpServer: server,
    // You should not use autoAcceptConnections for production
    // applications, as it defeats all standard cross-origin protection
    // facilities built into the protocol and the browser.  You should
    // *always* verify the connection's origin and decide whether or not
    // to accept it.
    autoAcceptConnections: false
});

function originIsAllowed(origin) {
    console.log(origin);
  // put logic here to detect whether the specified origin is allowed.
  return true;
}

wsServer.on('request', function(request) {
    console.log(request);
    //give everyone unique id!


    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }

    var connection = request.accept(null, request.origin);
    connections.push(connection);
    console.log((new Date()) + ' Connection accepted.', connection.user);

    connection.on('message', function(message) {


        if (message.type === 'utf8') {
            let data = message.utf8Data;
            let val
            try{
                 val = JSON.parse(data);
            }catch (e){
                console.log(e)
            }
            console.log(data);
            // console.log(val.client_1);
            //console.log(data.length);
            //connection.sendUTF(message.utf8Data); this resend the reseived message, instead of it i will send a custom message. hello from nodejs
            try{
                connections.forEach(function (destination) {
                    if (destination !== connection)
                        destination.send(data)
                });
            }catch (e) {
                console.log(e)
            }
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            connection.sendBytes(message.binaryData);
        }
    });



    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});


// var WebSocketServer = require('websocket').server;
// var http = require('http');
// const express =  require('express');
// const app = express();
//
// //glob variables...
// var connections = [];
//
//
// var server = app.listen(7000, function() {
//     console.log((new Date()) + ' Server is listening on port 7000');
// });
//
//
//
// wsServer = new WebSocketServer({
//     httpServer: server,
//     // You should not use autoAcceptConnections for production
//     // applications, as it defeats all standard cross-origin protection
//     // facilities built into the protocol and the browser.  You should
//     // *always* verify the connection's origin and decide whether or not
//     // to accept it.
//     autoAcceptConnections: false
// });
//
// function originIsAllowed(origin) {
//     console.log(origin);
//     // put logic here to detect whether the specified origin is allowed.
//     return true;
// }
//
// wsServer.on('request', function(request) {
//     console.log(request);
//     //give everyone unique id!
//
//
//     if (!originIsAllowed(request.origin)) {
//         // Make sure we only accept requests from an allowed origin
//         request.reject();
//         console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
//         return;
//     }
//
//     var connection = request.accept(null, request.origin);
//     connections.push(connection);
//     console.log((new Date()) + ' Connection accepted.', connection.user);
//
//     connection.on('message', function(message) {
//
//
//         if (message.type === 'utf8') {
//             var data = message.utf8Data;
//             console.log('Received Message: ' + data);
//             //connection.sendUTF(message.utf8Data); this resend the reseived message, instead of it i will send a custom message. hello from nodejs
//             connections.forEach(function(destination) {
//                 if(destination !== connection)
//                     destination.sendUTF(message.utf8Data);
//             });
//         }
//         else if (message.type === 'binary') {
//             console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
//             connection.sendBytes(message.binaryData);
//         }
//     });
//
//
//
//     connection.on('close', function(reasonCode, description) {
//         console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
//     });
// });
