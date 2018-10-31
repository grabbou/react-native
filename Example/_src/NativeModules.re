type headlessJsTaskSupport = {. [@bs.meth] "notifyTaskFinished": int => unit};

type manager = Js.Dict.t(string);

[@bs.deriving abstract]
type viewConfig = {
  [@bs.as "Manager"]
  manager: option(manager),
};

type uIManager('a) = Js.Dict.t(viewConfig);

[@bs.deriving abstract]
type nativeModules('manager) = {
  [@bs.as "HeadlessJsTaskSupport"]
  headlessJsTaskSupport,
  [@bs.as "UIManager"]
  uIManager: uIManager('manager),
};

[@bs.module]
external nativeModules: nativeModules('manager) = "NativeModules";
