type queue('a) = (array(int), array(int), array(array('a)), int);

[@bs.deriving abstract]
type messageQueue('a) = {
  enqueueNativeCall:
    (int, int, array({.}), option(unit => unit), option(unit => unit)) =>
    unit,
  registerCallableModule: (string, 'a) => unit,
  registerLazyCallableModule: (string, unit => 'a) => unit,
  setImmediatesCallback: (unit => unit) => unit,
  getEventLoopRunningTime: unit => float,
  getCallableModule: string => Js.Null.t('a),
  flushedQueue: unit => Js.Null.t(queue('a)),
  callFunctionReturnFlushedQueue:
    (string, string, array({.})) => Js.Null.t(queue('a)),
  callFunctionReturnResultAndFlushedQueue:
    (string, string, array({.})) => ({.}, Js.Null.t(queue('a))),
  invokeCallbackAndReturnFlushedQueue:
    (int, array({.})) => Js.Null.t(queue('a)),
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
    (
      switch (Belt.MutableMap.String.get(lazyCallableModules, name)) {
      | Some(getValue) => Some(getValue())
      | None => None
      }
    )
    |> Js.Null.fromOption;

  let registerCallableModule = (name, module_: 'a) =>
    Belt.MutableMap.String.set(lazyCallableModules, name, () => module_);

  let registerLazyCallableModule = (name, factory) =>
    Belt.MutableMap.String.set(lazyCallableModules, name, () => factory());

  let callFunction = (module_, method, args) => {
    lastFlush := Js.Date.now();
    eventLoopStartTime := lastFlush^;

    /* Should do some Systrace stuff */
    
    let moduleMethods = Js.Null.getExn(getCallableModule(module_));
    if ([%raw "!moduleMethods[method_]"]) {
      raise(Sys_error("can't call " ++ method ++ " on " ++ [%bs.raw "JSON.stringify(moduleMethods)"]));
    } else {
      let applyModuleMethods = [%raw {|
        function (mM, method, a,) {
          mM[method].apply(mM, a);
        }
      |}];
      applyModuleMethods(moduleMethods, method, args)
    }
  };

  let callImmediates = () =>
    switch (immediatesCallback^) {
    | Some(cb) => cb()
    | None => ()
    };

  let setImmediatesCallback = fn => immediatesCallback := Some(fn);

  let flushedQueue = () => {
    let () = guard(() => callImmediates());

    let q = queue^;
    let (a, _, _, _) = q;
    queue := ([||], [||], [||], callId^);
    (Belt.Array.length(a) > 0 ? Some(q) : None)->Js.Null.fromOption;
  };

  let callFunctionReturnFlushedQueue = (module_, method, args) => {
    let () = guard(() => callFunction(module_, method, args));

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
        onFail: option(unit => unit),
        onSuccess: option(unit => unit),
      ) => {
    switch (onFail, onSuccess) {
    | (Some(onFail), Some(onSuccess)) =>
      Js.Array.push([%raw "callId << 1"], params);
      Js.Array.push([%raw "params.push((callId << 1) | 1)"], params);
      Belt.MutableMap.Int.set(successCallbacks, callId^, onSuccess);
      Belt.MutableMap.Int.set(failureCallbacks, callId^, onFail);
    | (Some(onFail), None) =>
       Js.Array.push([%raw "callId << 1"], params);
      Belt.MutableMap.Int.set(failureCallbacks, callId^, onFail);
    | (None, Some(onSuccess)) =>
      Js.Array.push([%raw "params.push((callId << 1) | 1)"], params);
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
      let flushImmediate = [%raw "function (q) { global.nativeFlushQueueImmediate(q) }"];
      flushImmediate(.q);
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
