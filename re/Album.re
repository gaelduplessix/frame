type t = {
  id: string,
  title: string,
};

type albumsResult = {
  albums: list(t),
  nextPageToken: option(string),
};

let decodeAlbum = json =>
  Json.Decode.{
    id: field("id", string, json),
    title: field("title", string, json),
  };

let decodeAlbumsResult = json =>
  Json.Decode.{
    albums: field("albums", list(decodeAlbum), json),
    nextPageToken: optional(field("nextPageToken", string), json),
  };