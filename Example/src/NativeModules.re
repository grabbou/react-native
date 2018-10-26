type headlessJsTaskSupport = {. [@bs.meth] "notifyTaskFinished": int => unit};

[@bs.deriving abstract]
type nativeModules = {
  [@bs.as "HeadlessJsTaskSupport"]
  headlessJsTaskSupport,
};

[@bs.module] external nativeModules: nativeModules = "NativeModules";
