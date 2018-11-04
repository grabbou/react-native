module AppRegistry = AppRegistry.Make({
  type component = ReasonReact.component(
    ReasonReact.stateless,
    ReasonReact.noRetainedProps,
    ReasonReact.actionless
  );
});

let render = (component) => AppRegistry.registerRunnable("App", component);
