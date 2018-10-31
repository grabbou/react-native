type moduleConfig('constants) = (
  string, /* name */
  option('constants), /* constants */
  array((string, int)), /* methods */
  array(int), /* promise method IDs */
  array(int) /* sync method IDs */
);

type methodType =
  | PromiseMethod
  | AsyncMethod
  | SyncMethod;

type methodWrapper('a, 'b) =
  | Promise(array('a) => Js.Promise.t('b))
  | Async(array('a) => unit)
  | Sync(array('a) => unit);
/*
 let genMethod = (moduleId, methodId, methodType) =>
   switch (methodType) {
   | PromiseMethod =>
     Promise(
       (
         args =>
           Js.Promise.make((~resolve, ~reject) =>
             BatchedBridge.enqueueNativeCall(
               moduleId,
               methodId,
               args,
               data => resolve(. data) |> ignore,
               errorData => reject(. errorData) |> ignore,
             )
           )
       ),
     )
   | AsyncMethod =>
     Async(
       (
         args =>
           BatchedBridge.enqueueNativeCall(
             moduleId,
             methodId,
             args,
             data => Js.Promise.resolve(. data)->ignore,
             errorData => Js.Promise.reject(. errorData)->ignore,
           )
       ),
     )
   | SyncMethod =>
     Sync((args => global.nativeCallSyncHook(moduleId, methodId, args)))
   };

 let genModule = (~config: option(moduleConfig('a))=?, moduleId: int) =>
   switch (config) {
   | None => Js.Nullable.null
   | Some(cfg) =>
     let (moduleName, constants, methods, promiseMethods, syncMethods) = cfg;
     switch (constants) {
     | None => Js.Nullable.return({"name": moduleName})
     | Some(consts) =>
       Belt.Array.map(methods, ((methodName, methodID)) =>
         switch (
           Js.Array.includes(methodID, promiseMethods),
           Js.Array.includes(methodID, syncMethods),
         ) {
         | (true, true) => raise(Not_found) /* Can't be same */
         | (true, false) => (
             methodName,
             genMethod(moduleId, methodID, Promise),
           )
         | (false, false) => (
             methodName,
             genMethod(moduleId, methodID, Async),
           )
         | (false, true) => (methodName, genMethod(moduleId, methodID, Sync))
         }
       )
       ->Js.Dict.fromArray
       ->Js.Nullable.return
     };
   };
 */
