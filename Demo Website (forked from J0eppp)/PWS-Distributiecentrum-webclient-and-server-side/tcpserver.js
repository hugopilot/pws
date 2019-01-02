var net = require('net');

var server = net.createServer(function(socket) {
    console.log("A new connection is established!");

  socket.on('data', function(data) {
    console.log("" + data);
  });

  socket.on("error", function (err) {
    console.log(err);
  });
});

server.listen(1337, '127.0.0.1', function () {
  console.log("The server is listening on port 8383!");
});
