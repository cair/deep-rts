'use strict';

angular.module('myApp.view1', ['ngRoute'])

    .config(['$routeProvider', function($routeProvider) {
        $routeProvider.when('/view1', {
            templateUrl: 'view1/view1.html',
            controller: 'View1Ctrl'
        });
    }])

    .controller('View1Ctrl', function($scope, API) {



        API.on('state', function(data){
            $scope.state_data = [];
            for(var idx in data) {
                var game_data = JSON.parse(data[idx]["game"]["data"]);
                $scope.state_data.push(game_data)
            }


        });
        setInterval(function () {
            console.log("sent")
            API.emit('state', {})

        }, 1000)

    });