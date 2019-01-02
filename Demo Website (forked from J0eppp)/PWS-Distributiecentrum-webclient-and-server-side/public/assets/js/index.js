function addProduct(product) {
  pushProduct(product);
}

function pushProduct(product) {
  if (localStorage.getItem("products") == null) {
    localStorage.setItem("products", product + ", ");
  } else {
    var localStorageProducts = localStorage.getItem("products");
    var localProducts = localStorageProducts.split(", ");
    if (localProducts.length > 4) {
      alert("Je kan niet meer dan 3 producten toevoegen aan je winkelwagen!");
    } else {
      localStorageProducts += product + ", ";
      localStorage.setItem("products", localStorageProducts);
    }
  }
  console.log("The product has successfully been added.. product: " + product);
  countProducts();
}

function countProducts() {
  var localStorageProducts = localStorage.getItem("products");
  if (localStorageProducts == null) {
    document.getElementById('amountOfItems').innerHTML = 0;
  } else {
    var localProducts = localStorageProducts.split(", ");
    localProducts.splice(localProducts.length - 1, 1);
    document.getElementById('amountOfItems').innerHTML = localProducts.length;
  }
}

function removeProduct(product) {
  var products = localStorage.getItem("products");
  products = products.split(", ");
  products.splice(products.length - 1, 1);
  for (var i = 0; i < products.length; i++) {
    if (products[i] == product) {
      products.splice(i, 1);
    }
  }
  localStorage.setItem("products", products);
  console.log(products);
}

function removeAllProducts() {
  localStorage.setItem("products", "");
}

function getProducts() {
  var localStorageProducts = localStorage.getItem("products");
  var localProducts = localStorageProducts.split(", ");
  localProducts.splice(localProducts.length - 1, 1);
  for (var i = 0; i < localProducts.length; i++) {
    document.getElementById('products').innerHTML += localProducts[i] + `<button onclick="removeProduct('` + localProducts[i] + `')">Remove</button>` + "<br>";
  }
  console.log(localProducts);
}

function checkout() {
  var localStorageProducts = localStorage.getItem("products");
  var localProducts = localStorageProducts.split(", ");
  localProducts.splice(localProducts.length - 1, 1);
  console.log(localProducts);
}

function getLocalStorage() {
  var localStorageProducts = localStorage.getItem("products");
  var localProducts = localStorageProducts.split(", ");
  localProducts.splice(localProducts.length - 1, 1);
  document.getElementById('input').value = localProducts;
}

function changeLink(link) {
  window.history.pushState("", "", link);
}

var hasRan = false;

function getOrders() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState = 4 && this.status == 200) {
      document.getElementById('orders').innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/api/getOrders", true);
  xhttp.send();
}

function getOrdersAgain() {
  if (hasRan == false) {
    hasRan = true;
    setInterval(getOrders, 1000 * 60);
  } else {
    console.log("I'm sorry, the function is already running!");
  }
}
