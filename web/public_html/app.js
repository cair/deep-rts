'use strict';

// Declare app level module which depends on views, and components
angular.module('myApp', [
    'ngRoute',
    'ngWebSocket',
    'myApp.view1',
    'myApp.view2',
    'myApp.version'
]).
factory('API', function($websocket) {
    // Open a WebSocket connection
    var url = "ws:"+'//'+location.hostname+(location.port ? ':'+location.port: '') + "/websocket";
    console.log(url)
    var dataStream = $websocket(url);

    var callbacks = {
      none: []
    };

    dataStream.onMessage(function(msg) {
        var data = JSON.parse(msg.data);
        var type = data['type'];

        if(type in callbacks) {
          for(var i=0; i < callbacks[type].length; i++) {
            callbacks[type][i](data['data'])
          }
        } else {
            for(var i=0; i < callbacks[type].length; i++) {
                callbacks["none"][i](data['data'])
            }
        }
    });

    var methods = {
        callbacks: callbacks,
        on: function(name, func) {
          if (!(name in callbacks)) {
            callbacks[name] = [];
          }

          callbacks[name].push(func);
        },
        emit: function(type, data) {
            dataStream.send(JSON.stringify({ type: type, data: data }));
        }
    };

    return methods;
}).
config(['$locationProvider', '$routeProvider', function($locationProvider, $routeProvider) {
    $locationProvider.hashPrefix('!');

    $routeProvider.otherwise({redirectTo: '/view1'});
}]);
