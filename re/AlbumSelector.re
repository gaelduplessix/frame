open BsReactNative;
open Js.Promise;

type state = {
  albums: list(Album.t),
  nextPageToken: option(string),
};

type action =
  | SetAlbums(list(Album.t), option(string));

let component = ReasonReact.reducerComponent("AlbumSelector");

let make =
    (
      ~accessToken: string,
      ~onSelected: string => unit,
      ~onError: unit => unit,
      _children,
    ) => {
  let loadMoreAlbums = (~self, ~force) =>
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
        |> catch(e => {
             Js.log2("error!", e);
             onError();
             resolve();
           })
      )
      |> ignore;
    };
  {
    ...component,
    initialState: () => {albums: [], nextPageToken: None},
    didMount: self => loadMoreAlbums(~self, ~force=true),
    reducer: (action, _state) =>
      switch (action) {
      | SetAlbums(albums, nextPageToken) =>
        ReasonReact.Update({albums, nextPageToken})
      },
    render: self =>
      <SafeAreaView>
        <FlatList
          style=Style.(style([height(Pct(100.))]))
          data=(self.state.albums |> Array.of_list)
          keyExtractor=((album, _i) => album.id)
          renderItem=(
            FlatList.renderItem(renderItem => {
              let album: Album.t = renderItem.item;
              <TouchableOpacity onPress=(() => onSelected(album.id))>
                <View style=Style.(style([padding(Pt(15.0))]))>
                  <Text> (ReasonReact.string(album.title)) </Text>
                </View>
              </TouchableOpacity>;
            })
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
          onEndReached=(_ => loadMoreAlbums(~self, ~force=false))
        />
      </SafeAreaView>,
  };
};