var biggestInt = 9007199254740992;
var smallestInt = -9007199254740992;


function createRandomInt() {
  var text = "";
  var possible = "0123456789";

  for (var i = 0; i < 10; i++)
    text += possible.charAt(Math.floor(Math.random() * possible.length));

  if (text > biggestInt) {
    console.log("Int was too big!");
    createRandomInt();
  } else {
    return text;
  }
}

while (true) {
  var firstInt = createRandomInt();
  var secondInt = createRandomInt();
  var times = firstInt * secondInt;
  if (times > biggestInt) {
    console.log("Too big!");
  } else {
    console.log(times);
    return false;
  }
}
