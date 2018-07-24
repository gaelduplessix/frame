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
    let handleLogout = () =>
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
        /* | Some(_token) => <Button title="Logout" onPress=handleLogout /> */
        | Some(accessToken) => <AlbumSelector accessToken />
        }
      )
    </SafeAreaView>;
  },
};

/* expose ReactJS component for use by React Native */
let app = ReasonReact.wrapReasonForJs(~component, _jsProps => make([||]));

/* <Image
     style=Style.(style([width(Pt(100.0)), height(Pt(100.0))]))
     source=(
       Image.URI(
         Image.imageURISource(
           ~uri=
             "https://lh3.googleusercontent.com/lr/AJ_cxPaJU12BYdHonAkn9K2xNju5JiwAI34f8yZl96EXDXnRM_qNE_cPtqBeqxjy4nMMfUofVT81cqPPSU7noC7WVlGWTJdVyUlpMgVeH9GvVelzCxDMzlyPl-tfdhctaOroKu2cUYzLU_SG5tSf7B0lbpfqiXXIMY7res91CVf82YNQWJY2UyuTZKO_rDOvYnNAg1Z5k1uKP0owRaGjG3rIJzAnCIX7SCJ096-x7pgvKrmplIjHB-6fITML1ty6xwakCLS4fD09NWKSdg8NzZqB427Fq4xzuBpnwOz3qecFEJh1FMAvrb7gDr77n6pa-ka81LAZgnY7IVvHsAKZLDYJY86aBsky56UdFCgdIRqWFD4ph7bjKtvhXf1xRDDcEmNGB4PWFnYHc7oRufeAsB9YxFR6bmEq3GZMziRxoHgVs3QhuXfK0P6kSBdVK7hr6vhBzNTH-UUOWQq0fPpvDFVz4DYyIZHQbkp4l5XDr0dg60gMVUqcxBi5-yCJI2mbCUy9ErTu-4Q8c0GFZxdHXA8oHY7FbJjNMhIkocAWb5yScnCTY0vrZx_WwFiFCwcsZHTv0Qx5u4TJK_97X4HkFU0fR2phOO9HunWvojEj22eEWrT-vVBRavHyTouN9PL1pNKlhMjOWzkf6MQ_zSIea3sNQvNkarcSRsDj4q1qX01Cq7qp8tb8Bq0C-pencesj5OhRcXNNccZcW63hpaNLh5vYHeqswgMtN595LfrXpIgKZZYTVZYZb29Weo3vTTE2TakGhA3lnJ3USpSzhQwU7Nr-4ZIOzKf4iOwvhG2Qsy2p3aBRLimtgGvCUHvFtyyKBjZbUmg",
           (),
         ),
       )
     )
   /> */