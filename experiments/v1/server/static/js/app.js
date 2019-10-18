var deeprts = angular.module('deeprts',["ngRoute", "ngWebSocket"]);

deeprts.config(function($routeProvider, $locationProvider) {
    $routeProvider
        .when("/", {
            templateUrl : "static/templates/index.html",
            controller: "index",
            controllerAs: 'vm'
        })
        .when("/red", {
            templateUrl : "red.htm"
        })
        .when("/green", {
            templateUrl : "green.htm"
        })
        .when("/blue", {
            templateUrl : "blue.htm"
        })
        .otherwise({
            redirectTo: '/'
        });

    // use the HTML5 History API
    $locationProvider.html5Mode({
        enabled: true,
        requireBase: false
    });
});

deeprts.filter('reverse', function() {
    return function(items) {
        return items.slice().reverse();
    };
});

deeprts.factory('WebData', function() {


    var socket = window.io("ws://localhost:8080");

    socket.on('connect', function (socket) {

    });

    return {
        subscribe: function(action, cb) {
            socket.on(action, cb)
        },
        send: function(action, data) {
            socket.emit(action, JSON.stringify(data));
        }
    };




    // Open a WebSocket connection
    //var dataStream = $websocket('ws://localhost:8080');
    /*var actions = {};

    dataStream.onMessage(function(message) {

        console.log(message);

        if (!"action" in message) {
            throw new Error("Missing key 'action' in message");
        }
        if (!"data" in message) {
            throw new Error("Missing key 'data' in message")
        }
        if (!message.action in actions) {
            throw new Error("No subscribed endpoints for action " + message.action + " could not handle message!");
        }

        for(var k in actions[message.action]) {
            actions[message.action][k](message.data);
        }


    });*/

});


deeprts.controller('index',
    function($scope, WebData){
        var vm = this;

        vm.generation = "N/A";
        vm.gresults = [];

        WebData.subscribe("weight_status", function(data){
            data = JSON.parse(data);
            vm.generation = data["generation"];
            $scope.$apply()
        });

        WebData.subscribe("web_game_result", function(data){
            data = JSON.parse(data);
            vm.gresults.push(data);
            $scope.$apply()
        });

        WebData.subscribe("web_game_results", function(data){
            data = JSON.parse(data);
            for(var i in data){
                var v = data[i];
                vm.gresults.push(v);
            }
            $scope.$apply()
        });

        WebData.send("game_results", {})

    });
