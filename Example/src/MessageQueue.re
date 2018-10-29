type messageQueue;

[@bs.new] [@bs.module] external make: unit => messageQueue = "MessageQueue";

[@bs.send]
external enqueueNativeCall:
  (messageQueue, int, int, array('a), 'b => unit) => unit =
  "enqueueNativeCall";

[@bs.send]
external registerCallableModule: (messageQueue, string, 'a) => unit =
  "registerCallableModule";

[@bs.send]
external registerLazyCallableModule:
  (messageQueue, string, unit => unit) => unit =
  "registerLazyCallableModule";

[@bs.send]
external setImmediatesCallback: (messageQueue, unit => unit) => unit =
  "setImmediatesCallback";

[@bs.send]
external getEventLoopRunningTime: (messageQueue, unit) => float =
  "getEventLoopRunningTime";
