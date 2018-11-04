module AppRegistry =
  AppRegistry.Make({
    type component =
      ReasonReact.component(
        ReasonReact.stateless,
        ReasonReact.noRetainedProps,
        ReasonReact.actionless,
      );
  });

module Bridge =
  Bridge.Make({
    let handleModuleCall = (method, _args) =>
      switch (method) {
      | "AppRegistry.runApplication" => ()
      | _ => ()
      };
  });

let render = component => AppRegistry.registerRunnable("App", component);
