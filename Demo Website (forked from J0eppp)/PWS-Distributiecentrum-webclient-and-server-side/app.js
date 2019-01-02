const express = require('express');
const router = require('./routers/router');

const app = express();

const server = app.listen(80, function () {
  console.log("The server is listening on port 80!");
});

app.set("view engine", "ejs");

app.use("/js/", express.static("public/assets/js/"));
app.use("/css/", express.static("public/assets/css/"));
app.use(router);
