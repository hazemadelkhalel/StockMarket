// utils/cookie.js
import Cookies from "js-cookie";

const TOKEN_COOKIE_NAME = "stock_token";

export const setSessionToken = (token) => {
  Cookies.set(TOKEN_COOKIE_NAME, token);
};

export const getSessionToken = () => {
  const token = Cookies.get(TOKEN_COOKIE_NAME);

  if (token == null || token == undefined || !token) {
    return "";
  }
  return token;
};

export const removeSessionToken = () => {
  Cookies.remove(TOKEN_COOKIE_NAME);
};
