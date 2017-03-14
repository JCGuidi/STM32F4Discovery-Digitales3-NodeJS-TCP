var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope,$http) {
  $scope.data = {};

  var dataFromServer;
  function myFunction() {
      setInterval(function(){
        $scope.getTemp();
      }, 1000);
  }

  function getData() {
    $http({
        url: '/chartData',
        method: 'POST',
        data: {}
    }).then(function (httpResponse) {

        dataFromServerChart = httpResponse.data;
        longitud = dataFromServerChart.array.length - 1;
        drawChart();
    })
  }

  angular.element(document).ready(function () {
        myFunction()
        getData();
    });

  $scope.getTemp = function(){
    var tempText = $('#temp');
    var tempMin = $('#tempMin');
    var tempMax = $('#tempMax');

    $http({
        url: '/temperatura',
        method: 'POST',
        data: {}
    }).then(function (httpResponse) {
        console.log('response:', httpResponse.data);
        dataFromServer = httpResponse.data;
        tempText.text(dataFromServer.temp);
        tempMin.text(dataFromServer.tMin);
        tempMax.text(dataFromServer.tMax);

        longitud += 1;

        var fila = [longitud , parseFloat(dataFromServer.temp)];
        dataFromServerChart.array.push(fila);
        console.log(dataFromServerChart.array);
        drawChart();
        $("#fan").removeAttr('style');
        var css = '';
        if (dataFromServer.isOn == "1") {
          css = 'animation: 1s rotate360 infinite linear;';
        } else {
          css = 'animation: none !important;';
        }
        $("#fan").attr("style", css);
    })
  }

  $scope.setMin = function(){

     var btn = $('#tMin');
     btn.attr("disabled", true);
     var btn2 = $('#tMax');
     btn2.attr("disabled", true);

     $http({
         url: '/setMIN',
         method: 'POST',
         data: $scope.data
     }).then(function (httpResponse) {
         console.log('response:', httpResponse.data);

         var str = "" + httpResponse.data;
         console.log(str);
         if (str.indexOf("Error") != -1) {
           console.log('error');
         } else if (str.indexOf("OK") != -1){
           btn.removeAttr("disabled");
           btn2.removeAttr("disabled");
         }
     })
  }

  $scope.setMax = function(){

     var btn = $('#tMin');
     btn.attr("disabled", true);
     var btn2 = $('#tMax');
     btn2.attr("disabled", true);

     $http({
         url: '/setMAX',
         method: 'POST',
         data: $scope.data
     }).then(function (httpResponse) {
         console.log('response:', httpResponse.data);

         var str = "" + httpResponse.data;
         console.log(str);
         if (str.indexOf("Error") != -1) {
           console.log('error');
         } else if (str.indexOf("OK") != -1){
           btn.removeAttr("disabled");
           btn2.removeAttr("disabled");
         }
     })
  }

  $scope.turnOn = function(){

     var btn = $('#ejec');
     btn.attr("disabled", true);
     $("#fan").removeAttr('style');
     var css = 'animation: 1s rotate360 infinite linear;';
     $("#fan").attr("style", css);
     $http({
         url: '/TurnON',
         method: 'POST',
         data: {}
     }).then(function (httpResponse) {
         console.log('response:', httpResponse.data);

         var str = "" + httpResponse.data;
         console.log(str);
         if (str.indexOf("Error") != -1) {
           console.log('error');
         } else if (str.indexOf("OK") != -1){
           btn.removeAttr("disabled");
         }
     })
  }

  $scope.turnOff = function(){

     var btn = $('#ejec2');
     btn.attr("disabled", true);
     $("#fan").removeAttr('style');
     var css = 'animation: none !important;';
     $("#fan").attr("style", css);

     $http({
         url: '/TurnOFF',
         method: 'POST',
         data: {}
     }).then(function (httpResponse) {
         console.log('response:', httpResponse.data);

         var str = "" + httpResponse.data;
         console.log(str);
         if (str.indexOf("Error") != -1) {
           console.log('error');
         } else if (str.indexOf("OK") != -1){
           btn.removeAttr("disabled");
         }
     })
  }

});
