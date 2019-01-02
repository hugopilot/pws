const net = require('net');

function sendTCPMessage(message) {
  client.write(message, "ASCII");
}

const client = new net.Socket();
client.connect(1337, "127.0.0.1", function () {
  console.log("The webserver is connected to the TCP server...");
  sendTCPMessage("Hi, I'm a webserver and I'm connected to you!");
});

client.on('data', function(data) {
	console.log('Received: ' + data);
});

client.on("error", function (err) {
  console.log(err);
});

client.on('close', function() {
	console.log('Connection closed');
});

function one() {
  sendTCPMessage("one");
}

module.exports = {
  one: one
}
