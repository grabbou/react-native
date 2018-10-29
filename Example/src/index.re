let registerRunnable =
  AppRegistry.registerRunnableGet(AppRegistry.appRegistry);

let run = _ =>
  MessageQueueRe.enqueueNativeCallGet(
    BatchedBridge.batchedBridge,
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
    (),
  );

registerRunnable("RNTester", run);
