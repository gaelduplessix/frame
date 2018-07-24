let signinWithGoogle = () =>
  Google.login(
    ~iosClientId=Config.iosClientId,
    ~scopes=["https://www.googleapis.com/auth/photoslibrary.readonly"],
  )
  |> Js.Promise.then_(res => {
       let resType = res |> Google.type_Get;
       if (resType != "success") {
         Js.Promise.reject(Google.LoginError(resType));
       } else {
         Js.Promise.resolve(res |> Google.accessTokenGet);
       };
     });