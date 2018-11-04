module Map = Belt.MutableMap.String;

module Make = (Config: { type component; }) => {
  [@bs.deriving abstract]
  type runnable = {
    run: Belt.Map.String.t(string) => Config.component
  };

  let runnables: Map.t(runnable) = Map.make();

  let registerRunnable = (key, run) =>
    Map.set(runnables, key, runnable(~run));

  let runApplication = (appKey, appParameters) => {
    let _component = runGet(Map.getExn(runnables, appKey), appParameters);
  };
};

