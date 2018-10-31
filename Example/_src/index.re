let run = _ =>
  BatchedBridge.batchedBridge->MessageQueueRe.enqueueNativeCallGet(
    4,
    0,
    [|
      Obj.magic({
        "title": "Test",
        "message": "Hello world!",
        "buttons": [||],
        "type": "default",
      }),
    |],
    Some(Js.log),
    None,
  );

AppRegistry.appRegistry->AppRegistry.registerRunnableGet("RNTester", run);

