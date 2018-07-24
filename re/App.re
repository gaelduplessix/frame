open BsReactNative;
open Js.Promise;

type state = {
  accessToken: option(string),
  albumId: option(string),
};

type action =
  | SetAccessToken(option(string))
  | SetAlbumId(option(string));

let component = ReasonReact.reducerComponent("App");

let logout = () =>
  BsReactNative.AsyncStorage.removeItem(Config.accessTokenStorageKey, ())
  |> ignore;

let make = _children => {
  ...component,
  initialState: () => {accessToken: None, albumId: None},
  didMount: self =>
    /* try to retrieve token from storage */
    BsReactNative.AsyncStorage.getItem(Config.accessTokenStorageKey, ())
    |> then_(fromStorage => {
         self.send(SetAccessToken(fromStorage));
         resolve();
       })
    |> ignore,
  reducer: (action, state) =>
    switch (action) {
    | SetAccessToken(accessToken) =>
      ReasonReact.Update({...state, accessToken})
    | SetAlbumId(albumId) => ReasonReact.Update({...state, albumId})
    },
  render: self => {
    let handleLogin = () =>
      Login.signinWithGoogle()
      |> then_(token => {
           self.send(SetAccessToken(Some(token)));
           /* save token in local storage */
           BsReactNative.AsyncStorage.setItem(
             Config.accessTokenStorageKey,
             token,
             (),
           );
         })
      |> ignore;
    let logout = () =>
      BsReactNative.AsyncStorage.removeItem(Config.accessTokenStorageKey, ())
      |> then_(() => {
           self.send(SetAccessToken(None));
           resolve();
         })
      |> ignore;
    <SafeAreaView style=Style.(style([padding(Pt(10.0))]))>
      (
        switch (self.state.accessToken) {
        | None => <Button title="Login with Google" onPress=handleLogin />
        | Some(accessToken) =>
          switch (self.state.albumId) {
          | None =>
            <AlbumSelector
              accessToken
              onSelected=(albumId => self.send(SetAlbumId(Some(albumId))))
              /* in case of error, logout (TODO: better handle expired token...) */
              onError=logout
            />
          | Some(albumId) =>
            <View>
              <Button title="Logout" onPress=logout />
              <Button
                title="Select another album"
                onPress=(() => self.send(SetAlbumId(None)))
              />
              <SlideShow accessToken albumId />
            </View>
          }
        }
      )
    </SafeAreaView>;
  },
};

/* expose ReactJS component for use by React Native */
let app = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));