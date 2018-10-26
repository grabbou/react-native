/*
 Wanted output:
 ```js
 import "error-guard";
 import * as AppRegistry from "AppRegistry";

 AppRegistry.registerConfig([{
   appKey: "RNTester",
   run: () => {
     alert("test");
   }
 }]);
 ```
 */

[@bs.val] external alert: string => unit = "alert";

let registerRunnable =
  AppRegistry.registerRunnableGet(AppRegistry.appRegistry);

let run = _ => alert("test");

registerRunnable("RNTester", run);
