/**
 * Statically defined list of all routines that native side of React Native can call during
 * its lifetime.
 *
 * Due to different architecture of Reason code, we decided to explicitly map these calls.
 */
let map = {
  "AppRegistry.runApplication": appKey => AppRegistry.runApplication(appKey),
};
