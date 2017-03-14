//var mongoose = require('mongoose');
var net = require('net');

var HOST = '127.0.0.1';
var PORT = 3000;
var tMin = 20;
var tMax = 40;
var isOn = 0;
var tempArray = [20, 20.1];
var forcedOn = 0;
// var Temperatura = mongoose.model('Temperatura', {
//     time_sec: { type: Number },
//     temp: { type: Number }
// });

//Date.now();

function myFunction() {
    setInterval(function(){
      var cliTemp = new net.Socket();

      cliTemp.connect(7, '192.168.2.10', function() {
        console.log('Sent TEM');
        cliTemp.write('TEM');
      });

      cliTemp.on('data', function(data) {
        console.log('Received: ' + data);

        if (data[0] == 84) {

          var datos = String(data);
          var temp = parseFloat(datos.substring(1));
          temp = (temp/17.5).toFixed(2);

          tempArray.push(temp);
          if (temp > tMax) {
            cliTemp.write('ON');
            isOn = 1;
          } else if (temp < tMin) {
            if (forcedOn != 1) {
              cliTemp.write('OFF');
              isOn = 0;
            }
          } else if (isOn == 1) {
            cliTemp.write('ON');
            isOn = 1;
          }
        }

        cliTemp.destroy(); // kill client after server's response

      });
    }, 1000);
}

myFunction();

net.createServer(function(sock) {
    // We have a connection - a socket object is assigned to the connection automatically
    console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
    // Add a 'data' event handler to this instance of socket
    sock.on('data', function(data) {
        //console.log('DATA ' + sock.remoteAddress + ': ' + data.readUInt32BE(0).toString(16));
        // Write the data back to the socket, the client will receive it as data from the server
        if (sock.remoteAddress == HOST && sock.remotePort == PORT) {
            sock.write('Gracias Amigo');
        } else {
            var client = new net.Socket();

            //console.log(data[0]);
            if (data == "ON") {
              client.connect(7, '192.168.2.10', function() {
              	console.log('Sent ON');
              	client.write('ON');
                isOn = 1;
                forcedOn = 1;
              });
            } else if (data == "OFF") {
              client.connect(7, '192.168.2.10', function() {
              	console.log('Sent OFF');
              	client.write('OFF');
                isOn = 0;
                forcedOn = 0;
              });
            } else if (data == "TEMP") {
              var tempToSend = tempArray[tempArray.length - 1];
              sock.write(tempToSend + "/" + tMin + "/" + tMax + "/" + isOn);
            } else if (data == "Arr") {
              sock.write(tempArray.toString());
            } else if (data[0] == 84) {

              var datos = String(data);
              var temp = parseFloat(datos.substring(1));
              temp = (temp/17.5).toFixed(2);
              tempArray.push(temp);
              if (temp > tMax) {
                client.connect(7, '192.168.2.10', function() {
                	console.log('Sent ON');
                	client.write('ON');
                  isOn = 1;
                });
              } else if (temp < tMin) {
                if (forcedOn != 1) {
                  client.connect(7, '192.168.2.10', function() {
                    console.log('Sent OFF');
                    client.write('OFF');
                    isOn = 0;
                  });
                }
              } else if (isOn == 1) {
                cliTemp.write('ON');
                isOn = 1;
              }

              /*
              mongoose.connect('mongodb://localhost/temperaturas');

              var instance = new Temperatura();

              instance.time_sec = Date.now();
              instance.temp = temp;

              instance.save(function(err) {
                  if (err) { console.log(err); }
                  console.log("Saved!");
                  mongoose.connection.close();
              });
              */

            } else if (data[0] == 109) { //m
              var datos = String(data);
              tMin = parseFloat(datos.substring(1));
              console.log(tMin);
              sock.write("OK");
            } else if (data[0] == 77) { //M
              var datos = String(data);
              tMax = parseFloat(datos.substring(1));
              console.log(tMax);
              sock.write("OK");
            } else {
              sock.write(data);
            }

            client.on('data', function(data) {
            	console.log('Received: ' + data);
              if (data == 'OK') {
                sock.write(data);
              } else {
                sock.write(data);
              }
            	client.destroy(); // kill client after server's response
            });
        }
    });
    // Add a 'close' event handler to this instance of socket
    sock.on('close', function(data) {
        console.log('CLOSED: ' + sock.remoteAddress +' '+ sock.remotePort);
    });

}).listen(PORT);

console.log('Server listening on ' + HOST +':'+ PORT);
