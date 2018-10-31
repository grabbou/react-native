module Map = Belt.MutableMap.String;

[@bs.deriving abstract]
type runnable = {
  run:
    Belt.Map.String.t(string) =>
    ReasonReact.component(
      ReasonReact.stateless,
      ReasonReact.noRetainedProps,
      ReasonReact.actionless,
    ),
};

let runnables: Map.t(runnable) = Map.make();

let registerRunnable = (key, run) =>
  Map.set(runnables, key, runnable(~run));

let runApplication = (appKey, appParameters) => {
	let _component = runGet(Map.getExn(runnables, appKey), appParameters);
};
