open BsReactNative;
open Js.Promise;

type state = {
  albums: list(Album.t),
  nextPageToken: option(string),
};

type action =
  | SetAlbums(list(Album.t), option(string));

let component = ReasonReact.reducerComponent("AlbumSelector");

let loadMoreAlbums = (self, accessToken, force) =>
  if (ReasonReact.(self.state.nextPageToken) == None && ! force) {
    ();
  } else {
    let nextPageParam =
      switch (ReasonReact.(self.state.nextPageToken)) {
      | None => ""
      | Some(token) => "?pageToken=" ++ token
      };
    Fetch.(
      fetchWithInit(
        "https://photoslibrary.googleapis.com/v1/albums" ++ nextPageParam,
        RequestInit.make(
          ~headers=
            HeadersInit.makeWithDict(
              Js.Dict.fromList([
                ("Authorization", "Bearer " ++ accessToken),
              ]),
            ),
          (),
        ),
      )
      |> then_(Response.json)
      |> then_(res => {
           let albumsRes = Album.decodeAlbumsResult(res);
           ReasonReact.(
             self.send(
               SetAlbums(
                 List.concat([self.state.albums, albumsRes.albums]),
                 albumsRes.nextPageToken,
               ),
             )
           );
           resolve();
         })
    )
    |> ignore;
  };

let make = (~accessToken: string, _children) => {
  ...component,
  initialState: () => {albums: [], nextPageToken: None},
  didMount: self => loadMoreAlbums(self, accessToken, true),
  reducer: (action, state) =>
    switch (action) {
    | SetAlbums(albums, nextPageToken) =>
      ReasonReact.Update({...state, albums, nextPageToken})
    },
  render: self =>
    <SafeAreaView>
      <FlatList
        style=Style.(style([height(Pct(100.))]))
        data=(self.state.albums |> Array.of_list)
        keyExtractor=((album, _i) => album.id)
        renderItem=(
          FlatList.renderItem(({item}) =>
            <TouchableOpacity onPress=(() => ())>
              <View style=Style.(style([padding(Pt(15.0))]))>
                <Text> (ReasonReact.string(Album.(item.title))) </Text>
              </View>
            </TouchableOpacity>
          )
        )
        itemSeparatorComponent=(
          FlatList.separatorComponent(_ =>
            <View
              style=Style.(
                      style([
                        height(Pt(1.0)),
                        backgroundColor(String("#e1e8ee")),
                      ])
                    )
            />
          )
        )
        onEndReached=(_ => loadMoreAlbums(self, accessToken, false))
      />
    </SafeAreaView>,
};