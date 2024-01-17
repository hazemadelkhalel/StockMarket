// clear-cookies.js
Cookies = require("js-cookie");

const clearCookies = () => {
  Cookies.remove("stock_token");
  console.log("Cookies cleared successfully.");
};

clearCookies();
