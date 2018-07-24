import { app } from './lib/js/re/App.js';
export default app;

// import { AsyncStorage } from 'react-native';
// import Expo from 'expo';

// const IOS_CLIENT_ID = '';

// async function signInWithGoogleAsync() {
//   const result = await Expo.Google.logInAsync({
//     iosClientId: IOS_CLIENT_ID,
//     scopes: ['https://www.googleapis.com/auth/photoslibrary.readonly'],
//   });

//   if (result.type !== 'success') {
//     throw result;
//   }
//   return result;
// }

// async function getAccessToken() {
//   const accessToken = await AsyncStorage.getItem('googleAccessToken');
//   if (accessToken !== null) {
//     return JSON.parse(accessToken);
//   }
//   const result = await signInWithGoogleAsync();
//   await AsyncStorage.setItem('googleAccessToken', JSON.stringify(result));
//   return result;
// }

// const photosRepository = accessToken => ({
//   getAlbums() {
//     return fetch('https://photoslibrary.googleapis.com/v1/albums', {
//       headers: {
//         Authorization: `Bearer ${accessToken}`,
//       },
//     }).then(res => res.json());
//   },

//   getPhotos() {
//     return fetch('https://photoslibrary.googleapis.com/v1/mediaItems:search', {
//       headers: {
//         Authorization: `Bearer ${accessToken}`,
//       },
//       method: 'POST',
//       body: JSON.stringify({
//         albumId: '',
//       }),
//     }).then(res => res.json());
//   },
// });

// (async () => {
//   const { accessToken } = await getAccessToken();
//   const photosRepo = photosRepository(accessToken);
//   const albums = await photosRepo.getAlbums();
//   const photos = await photosRepo.getPhotos();
//   console.log({ albums, photos });
// })();
