type data;

type run = data => unit;
type task = data => Js.Promise.t(unit);

type taskProvider = unit => task;

[@bs.deriving abstract]
type runnable = {run};

[@bs.deriving abstract]
type registry = {
  registerRunnable: (string, run) => string,
  getAppKeys: unit => array(string),
  getRunnable: string => Js.Nullable.t(runnable),
  runApplication: (string, data) => unit,
  unmountApplicationComponentAtRootTag: int => unit,
  registerHeadlessTask: (string, taskProvider) => unit,
  startHeadlessTask: (int, string, data) => unit,
};

let runnables: Belt.MutableMap.String.t(runnable) =
  Belt.MutableMap.String.make();
let tasks: Belt.MutableMap.String.t(taskProvider) =
  Belt.MutableMap.String.make();

let registerRunnable = (appKey: string, run: run) => {
  Belt.MutableMap.String.set(runnables, appKey, runnable(~run));
  appKey;
};

let getAppKeys = () => Belt.MutableMap.String.keysToArray(runnables);

let getRunnable = (appKey: string) =>
  Belt.MutableMap.String.get(runnables, appKey)->Js.Nullable.fromOption;

let runApplication = (appKey, appParameters) => {
  SceneTracker.setActiveScene(SceneTracker.scene(~name=appKey));
  let runnable = Belt.MutableMap.String.getExn(runnables, appKey);
  let run = runGet(runnable);
  run(appParameters);
};

let unmountApplicationComponentAtRootTag = rootTag =>
  ReactNative.unmountComponentAtNodeAndRemoveContainer(rootTag);

let registerHeadlessTask = (taskKey: string, taskProvider: taskProvider) =>
  Belt.MutableMap.String.set(tasks, taskKey, taskProvider);

let startHeadlessTask = (taskId: int, taskKey: string, data: 'a) => {
  let taskProvider = Belt.MutableMap.String.getExn(tasks, taskKey);
  let task = taskProvider();
  data
  |> task
  |> Js.Promise.then_(() =>
       Js.Promise.resolve(
         NativeModules.headlessJsTaskSupportGet(NativeModules.nativeModules)##notifyTaskFinished(
           taskId,
         ),
       )
     )
  |> Js.Promise.catch(_ =>
       Js.Promise.resolve(
         NativeModules.headlessJsTaskSupportGet(NativeModules.nativeModules)##notifyTaskFinished(
           taskId,
         ),
       )
     )
  |> ignore;
};

let appRegistry: registry =
  registry(
    ~registerRunnable,
    ~getAppKeys,
    ~getRunnable,
    ~runApplication,
    ~unmountApplicationComponentAtRootTag,
    ~registerHeadlessTask,
    ~startHeadlessTask,
  );

BatchedBridge.registerCallableModule("AppRegistry", appRegistry);
