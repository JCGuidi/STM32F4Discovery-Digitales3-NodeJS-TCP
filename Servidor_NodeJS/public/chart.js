var app = angular.module('myApp', []);
app.controller('myCtrl', function($scope,$http) {
  $scope.data = {};

  angular.element(document).ready(function () {
        $scope.getData();
    });

  $scope.getData = function(){
    var btn = $('#ejec');
    var btn2 = $('#carg');
    btn.attr("disabled", true);
    btn2.attr("disabled", true);
    $http({
        url: '/chartData',
        method: 'POST',
        data: {}
    }).then(function (httpResponse) {
        dataFromServer = httpResponse.data;
        drawChart();
        btn2.removeAttr("disabled");
    })
  }

  $scope.runSDR = function(){
    var btn = $('#ejec');
    var btn2 = $('#carg');
    var spanE = $('#errorSpan');
    btn.attr("disabled", true);
    btn2.attr("disabled", true);
    $http({
        url: '/ejecData',
        method: 'POST',
        data: $scope.data
    }).then(function (httpResponse) {
        var str = "" + httpResponse.data;
        console.log(str);

        if (str.indexOf("Error") != -1) {
          btn.removeAttr("disabled");
          spanE.show();
        } else if (str.indexOf("Ok") != -1) {
          $scope.getData();
          btn.removeAttr("disabled");
        }
    })
  }
});
