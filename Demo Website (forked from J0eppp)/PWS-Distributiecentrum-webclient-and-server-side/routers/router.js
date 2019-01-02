const router = require('express').Router();
const net = require('net');
const bodyParser = require('body-parser');
const fs = require('fs');

var orders = "";

function readOrders() {
  orders = fs.readFileSync("orders.json", "UTF8");
}

readOrders();

const products = {
  "A1": "A1",
  "A2": "A2",
  "A3": "A3",
  "A3": "A3",
  "B1": "B1",
  "B2": "B2",
  "B3": "B3",
  "B3": "B3"
}

function createRandomID() {
  var text = "";
  var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

  for (var i = 0; i < 5; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));

  return text;
}

function sendTCPMessage(message) {
  const client = new net.Socket();
  client.connect(1337, "127.0.0.1", function() {
    console.log("The webserver is connected to the TCP server...");
  });

  client.on('data', function(data) {
    console.log('Received: ' + data);
  });

  client.write(message + "\n", "ASCII");

  client.on("error", function(err) {
    console.log(err);
  });

  client.on('close', function() {
    console.log('Connection closed');
  });

  setTimeout(function() {
    client.destroy();
  }, 100);
}

const jsonParser = bodyParser.json();
const urlencodedParser = bodyParser.urlencoded({
  extended: false
});

router.get("/", function(req, res) {
  res.render("home");
});

router.get("/winkelwagen", function(req, res) {
  res.render("winkelwagen");
});

router.post("/checkout", urlencodedParser, function(req, res) {
  if (!req.body) {
    res.sendStatus(400);
  } else {
    var input = req.body.input;
    input = JSON.stringify(input);
    var inputArray = input.split(",");
    inputArray = JSON.parse(inputArray);
    inputArray = inputArray.split(",");
    var order = "";
    for (var i = 0; i < inputArray.length; i++) {
      var product = products[inputArray[i]];
      order += product + ",";
    }
    order = order.slice(0, -1);
    if (order == "undefined") {
      res.status(400);
      res.redirect("/error?" + order);
    } else {
      sendTCPMessage("NOTG(" + order + ")");
      console.log("An order has been sent... order: " + order);
      fs.appendFile("orders.json", `"order":\n{"${order}"}\n`, function(err) {
        if (err) {
          console.log(err);
        } else {
          console.log("Order is saved!");
          readOrders();
        }
      });
      res.redirect("success");
    }
  }
});

router.get("/success", function(req, res) {
  res.render("success");
});

router.get("/orders", function(req, res) {
  res.render("orders");
});

router.get("/api/getOrders", function(req, res) {
  res.send(orders);
});

router.get("/error", function (req, res) {
  res.render("error");
});

module.exports = router;
