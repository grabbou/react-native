let batchedBridge = MessageQueueRe.make();

%raw
{|
Object.defineProperty(global, '__fbBatchedBridge', {
  configurable: true,
  value: batchedBridge,
})
|};

/*
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "Systrace", () =>
  [%raw "require('Systrace')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "JSTimers", () =>
  [%raw "require('JSTimers')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "HeapCapture", () =>
  [%raw "require('HeapCapture')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "SamplingProfiler", () =>
  [%raw "require('SamplingProfiler')"]
);
*/
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "RCTLog", () =>
  [%raw "require('RCTLog')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "RCTDeviceEventEmitter", () =>
  [%raw "require('RCTDeviceEventEmitter')"]
);
/*
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "RCTNativeAppEventEmitter", () =>
  [%raw "require('RCTNativeAppEventEmitter')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "PerformanceLogger", () =>
  [%raw "require('PerformanceLogger')"]
);
MessageQueueRe.registerLazyCallableModuleGet(batchedBridge, "JSDevSupportModule", () =>
  [%raw "require('JSDevSupportModule')"]
);
*/
