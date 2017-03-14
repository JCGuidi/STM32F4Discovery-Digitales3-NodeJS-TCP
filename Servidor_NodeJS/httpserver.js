var net = require('net');
var express = require('express');
var bodyParser = require('body-parser');
var app = express();

app.use(bodyParser.json({limit: '50mb'}));
app.use(express.static(__dirname + '/public'));

function pad(n) { return ("0" + n).slice(-2); }

app.get('/', function(req, res){
  res.redirect('/index.html');
});

app.post('/temperatura', function(req, res, next){

  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent TEMP');
    client.write('TEMP');
  });
  var dataToSend = {temp: "temp", tMin: 'tMin', tMax: 'tMax', isOn: '1'};

  client.on('data', function(data) {
    var datos = String(data);
    var result = datos.split("/");

    dataToSend.temp = result[0];
    dataToSend.tMin = result[1];
    dataToSend.tMax = result[2];
    dataToSend.isOn = result[3];

    res.send(dataToSend);

    client.destroy(); // kill client after server's response
  });
});

app.post('/chartData', function(req, res, next){
  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent Arr');
    client.write('Arr');
  });

  var array = [['Tiempo', 'Temperatura']];
  var dataToSend = {array: array};

  client.on('data', function(data) {
    var datos = String(data);
    var result = datos.split(",");
    var length = result.length;

    for (var i = 0; i < length; i++) {
      var fila = [i , parseFloat(result[i])];
      array.push(fila);
    }

    dataToSend.array = array;

    res.send(dataToSend);

    client.destroy(); // kill client after server's response
  });
});

app.post('/setMIN', function(req, res, next){

  var cope = req.body;
  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent tMin');
    client.write('m'+cope.temp);
  });

  client.on('data', function(data) {
    res.send(data);
    client.destroy(); // kill client after server's response
  });

});

app.post('/setMAX', function(req, res, next){

  var cope = req.body;
  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent tMax');
    client.write('M'+cope.temp);
  });

  client.on('data', function(data) {
    res.send(data);
    client.destroy(); // kill client after server's response
  });

});

app.post('/TurnON', function(req, res, next){

  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent ON');
    client.write('ON');
  });

  client.on('data', function(data) {
    res.send(data);
    client.destroy(); // kill client after server's response
  });

});

app.post('/TurnOFF', function(req, res, next){

  var client = new net.Socket();
  client.connect(3000, '127.0.0.1', function() {
    console.log('Sent OFF');
    client.write('OFF');
  });

  client.on('data', function(data) {
    res.send(data);
    client.destroy(); // kill client after server's response
  });

});

app.listen(8080);
console.log("Server running at http://localhost:3000/");
