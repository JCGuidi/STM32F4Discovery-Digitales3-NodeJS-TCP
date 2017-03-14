var net = require('net');

var HOST = '127.0.0.1';
var PORT = 3000;

var client = new net.Socket();

client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    // Write a message to the socket as soon as the client is connected, the server will receive it as message from the client
    var buf = "Arr"; //new Buffer([0x12, 0xa4, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78]);
    client.write(buf);

});

// Add a 'data' event handler for the client socket
// data is what the server sent to this socket
client.on('data', function(data) {
    console.log('DATA: ' + data);
    // Close the client socket completely
    client.destroy();
});

// Add a 'close' event handler for the client socket
client.on('close', function() {
    console.log('Connection closed');
});
