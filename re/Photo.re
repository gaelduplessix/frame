type t = {
  id: string,
  baseUrl: string,
};

type photosResult = {
  photos: list(t),
  nextPageToken: option(string),
};

let decodePhoto = json =>
  Json.Decode.{
    id: field("id", string, json),
    baseUrl: field("baseUrl", string, json),
  };

let decodePhotosResult = json =>
  Json.Decode.{
    photos: field("mediaItems", list(decodePhoto), json),
    nextPageToken: optional(field("nextPageToken", string), json),
  };