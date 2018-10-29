[@bs.deriving abstract]
type messageQueue('a) = {
  enqueueNativeCall:
    (
      int,
      int,
      array({.}),
      ~onFail: unit => unit=?,
      ~onSuccess: unit => unit=?,
      unit
    ) =>
    unit,
  registerCallableModule: (string, 'a) => unit,
  registerLazyCallableModule: (string, unit => 'a) => unit,
  setImmediatesCallback: (unit => unit) => unit,
  getEventLoopRunningTime: unit => float,
  getCallableModule: string => option('a),
  flushedQueue:
    unit => option((array(int), array(int), array(array('a)), int)),
  callFunctionReturnFlushedQueue:
    (string, string, array({.})) =>
    option((array(int), array(int), array(array('a)), int)),
  callFunctionReturnResultAndFlushedQueue:
    (string, string, array({.})) =>
    ({.}, option((array(int), array(int), array(array('a)), int))),
  invokeCallbackAndReturnFlushedQueue:
    (int, array({.})) =>
    option((array(int), array(int), array(array('a)), int)),
};

/*
 type spyData('a) = {
   type_: int,
   module_: option(string),
   method: string,
   args: array('a),
 };
 */

let make = () => {
  let lazyCallableModules: Belt.MutableMap.String.t(unit => 'a) =
    Belt.MutableMap.String.make();
  let queue: ref((array(int), array(int), array(array('a)), int)) =
    ref(([||], [||], [||], 0));
  let successCallbacks: Belt.MutableMap.Int.t(unit => unit) =
    Belt.MutableMap.Int.make();
  let failureCallbacks: Belt.MutableMap.Int.t(unit => unit) =
    Belt.MutableMap.Int.make();

  let callId = ref(0);
  let lastFlush = ref(0.);
  let eventLoopStartTime = ref(Js.Date.now());
  let immediatesCallback: ref(option(unit => unit)) = ref(None);

  /*
     let debugInfo = Belt.MutableMap.Int.make();
     let remoteModuleTable = Belt.MutableMap.Int.make();
     let remoteMethodTable = Belt.MutableMap.Int.make();
   */

  /* Should do some error checking here */
  let guard = fn => fn();

  let getCallableModule = name =>
    switch (Belt.MutableMap.String.get(lazyCallableModules, name)) {
    | Some(getValue) => Some(getValue())
    | None => None
    };

  let registerCallableModule = (name, module_: 'a) =>
    Belt.MutableMap.String.set(lazyCallableModules, name, () => module_);

  let registerLazyCallableModule = (name, factory) =>
    Belt.MutableMap.String.set(lazyCallableModules, name, () => factory());

  let callFunction = (module_, method, args) => {
    lastFlush := Js.Date.now();
    eventLoopStartTime := lastFlush^;

    /* Should do some Systrace stuff */

    switch (getCallableModule(module_)) {
    | Some(moduleMethods) =>
      %raw
      "moduleMethods[method].apply(moduleMethods, args)"
    | None => raise(Not_found) /* TODO: use better error */
    };
  };

  let callImmediates = () =>
    switch (immediatesCallback^) {
    | Some(cb) => cb()
    | None => ()
    };

  let setImmediatesCallback = fn => immediatesCallback := Some(fn);

  let flushedQueue = () => {
    guard(() => callImmediates());

    let q = queue^;
    let (a, _, _, _) = q;
    queue := ([||], [||], [||], callId^);
    Belt.Array.length(a) > 0 ? Some(q) : None;
  };

  let callFunctionReturnFlushedQueue = (module_, method, args) => {
    guard(() => callFunction(module_, method, args));

    flushedQueue();
  };

  let callFunctionReturnResultAndFlushedQueue = (module_, method, args) => (
    callFunction(module_, method, args),
    flushedQueue(),
  );

  let invokeCallback = (cbId: int, args: array({.})) => {
    lastFlush := Js.Date.now();
    eventLoopStartTime := lastFlush^;

    let callID: int = [%raw "cbId >>> 1"];
    let isSuccess: bool = [%raw "cbId & 1"];

    let callback =
      isSuccess ?
        Belt.MutableMap.Int.get(successCallbacks, callID) :
        Belt.MutableMap.Int.get(failureCallbacks, callID);

    switch (callback) {
    | Some(cb) => cb(Obj.magic(args))
    | None => ()
    };
  };

  let invokeCallbackAndReturnFlushedQueue = (cbId: int, args: array({.})) => {
    invokeCallback(cbId, args);

    flushedQueue();
  };

  let spy__ = data => ();

  let enqueueNativeCall =
      (
        moduleId: int,
        methodId: int,
        params: array('a),
        ~onFail: option(unit => unit)=?,
        ~onSuccess: option(unit => unit)=?,
        (),
      ) => {
    switch (onFail, onSuccess) {
    | (Some(onFail), Some(onSuccess)) =>
      %raw
      {|params.push(callId << 1)|};
      %raw
      {|params.push((callId << 1) | 1)|};
      Belt.MutableMap.Int.set(successCallbacks, callId^, onSuccess);
      Belt.MutableMap.Int.set(failureCallbacks, callId^, onFail);
    | (Some(onFail), None) =>
      %raw
      {|params.push(callId << 1)|};
      Belt.MutableMap.Int.set(failureCallbacks, callId^, onFail);
    | (None, Some(onSuccess)) =>
      %raw
      {|params.push((callId << 1) | 1)|};
      Belt.MutableMap.Int.set(successCallbacks, callId^, onSuccess);
    | _ => ()
    };

    callId := callId^ + 1;

    let (qModules, qMethods, qParams, _) = queue^;
    Js.Array.push(moduleId, qModules);
    Js.Array.push(methodId, qMethods);
    Js.Array.push(params, qParams);

    let now = Js.Date.now();

    if ([%raw "global.nativeFlushQueueImmediate"] && now -. lastFlush^ >= 5.) {
      let q = queue^;
      queue := ([||], [||], [||], callId^);
      lastFlush := now;
      %raw
      "global.nativeFlushQueueImmediate(q)";
    };
    ();
  };

  let getEventLoopRunningTime = () => Js.Date.now() -. eventLoopStartTime^;

  messageQueue(
    ~enqueueNativeCall,
    ~registerCallableModule,
    ~getCallableModule,
    ~registerLazyCallableModule,
    ~setImmediatesCallback,
    ~getEventLoopRunningTime,
    ~flushedQueue,
    ~callFunctionReturnFlushedQueue,
    ~callFunctionReturnResultAndFlushedQueue,
    ~invokeCallbackAndReturnFlushedQueue,
  );
};
