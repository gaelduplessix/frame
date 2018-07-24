[@bs.deriving abstract]
type login_params = {
  iosClientId: string,
  scopes: array(string),
};

[@bs.deriving abstract]
type login_res = {
  accessToken: string,
  refreshToken: string,
  serverAuthCode: string,
  [@bs.as "type"]
  type_: string,
};

[@bs.scope "Google"] [@bs.module "expo"]
external logInAsync : login_params => Js.Promise.t(login_res) = "";

let login = (~iosClientId: string, ~scopes: list(string)) =>
  logInAsync(login_params(~iosClientId, ~scopes=scopes |> Array.of_list));

exception LoginError(string);