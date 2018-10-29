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
/*
 [%raw
   {|
 const BatchedBridge = require('BatchedBridge');
 BatchedBridge.registerLazyCallableModule('Systrace', () => require('Systrace'));
 BatchedBridge.registerLazyCallableModule('JSTimers', () => require('JSTimers'));
 BatchedBridge.registerLazyCallableModule('HeapCapture', () =>
   require('HeapCapture'),
 );
 BatchedBridge.registerLazyCallableModule('SamplingProfiler', () =>
   require('SamplingProfiler'),
 );
 BatchedBridge.registerLazyCallableModule('RCTLog', () => require('RCTLog'));
 BatchedBridge.registerLazyCallableModule('RCTDeviceEventEmitter', () =>
   require('RCTDeviceEventEmitter'),
 );
 BatchedBridge.registerLazyCallableModule('RCTNativeAppEventEmitter', () =>
   require('RCTNativeAppEventEmitter'),
 );
 BatchedBridge.registerLazyCallableModule('PerformanceLogger', () =>
   require('PerformanceLogger'),
 );
 BatchedBridge.registerLazyCallableModule('JSDevSupportModule', () =>
   require('JSDevSupportModule'),
 );
 |}
 ];
 */
