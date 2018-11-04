
let handleCall = (moduleName, method, args) =>
  switch (moduleName, method) {
  | ("AppRegistry", "runApplication") => {
      Js.log(args[0]);
      ();
    };
  | _ => ();
  };
