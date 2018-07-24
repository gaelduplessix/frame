open BsReactNative;
open Js.Promise;

type state = {
  photos: list(Photo.t),
  nextPageToken: option(string),
};

type action =
  | SetPhotos(list(Photo.t), option(string));

let component = ReasonReact.reducerComponent("PhotoSelector");

let make = (~accessToken: string, ~albumId: string, _children) => {
  let loadMorePhotos = (~self, ~force) =>
    if (ReasonReact.(self.state.nextPageToken) == None && ! force) {
      ();
    } else {
      let nextPageParam =
        switch (ReasonReact.(self.state.nextPageToken)) {
        | None => ""
        | Some(token) => "?pageToken=" ++ token
        };
      Js.log2(accessToken, albumId);
      Fetch.(
        fetchWithInit(
          "https://photoslibrary.googleapis.com/v1/mediaItems:search"
          ++ nextPageParam,
          RequestInit.make(
            ~method_=Post,
            ~headers=
              HeadersInit.makeWithDict(
                Js.Dict.fromList([
                  ("Authorization", "Bearer " ++ accessToken),
                ]),
              ),
            ~body=BodyInit.make({j| { "albumId": "$(albumId)" } |j}),
            (),
          ),
        )
        |> then_(Response.json)
        |> then_(res => {
             Js.log(res);
             let photosRes = Photo.decodePhotosResult(res);
             ReasonReact.(
               self.send(
                 SetPhotos(
                   List.concat([self.state.photos, photosRes.photos]),
                   photosRes.nextPageToken,
                 ),
               )
             );
             resolve();
           })
      )
      |> ignore;
    };
  {
    ...component,
    initialState: () => {photos: [], nextPageToken: None},
    didMount: self => loadMorePhotos(~self, ~force=true),
    reducer: (action, _state) =>
      switch (action) {
      | SetPhotos(photos, nextPageToken) =>
        ReasonReact.Update({photos, nextPageToken})
      },
    render: self =>
      <SafeAreaView>
        (
          switch (self.state.photos) {
          | [photo, ..._] =>
            <Image
              style=Style.(style([width(Pct(100.0)), height(Pct(100.0))]))
              source=(
                Image.URI(Image.imageURISource(~uri=photo.baseUrl, ()))
              )
            />
          | _ => <Text> (ReasonReact.string("Loading...")) </Text>
          }
        )
      </SafeAreaView>,
  };
};