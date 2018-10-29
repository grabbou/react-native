let uIManager = NativeModules.nativeModules->NativeModules.uIManagerGet;

[@bs.module "UIManager"]
external removeRootView: int => unit = "removeRootView";

/*
 open DefineLazyObjectProperty;

  Js.Dict.values(uIManager)
  ->Belt.Array.forEach(viewConfig =>
      switch (NativeModules.managerGet(viewConfig)) {
      | Some(manager) =>
        defineLazyObjectProperty(
          viewConfig,
          "Constants",
          descriptor(
            ~get=
              () => {
                let viewManager =
                  Js.Dict.get(
                    Obj.magic(NativeModules.nativeModules),
                    Obj.magic(manager),
                  );
                switch (viewManager) {
                | Some(vM) =>
                  Js.Dict.entries(Obj.magic(vM))
                  ->Belt.Array.map(((key, value)) =>
                      switch (Js.typeof(value)) {
                      | "function" => ("remove", "remove")
                      | _ => (key, value)
                      }
                    )
                  ->Belt.Array.keep(x => x !== ("remove", "remove"))
                  ->Js.Dict.fromArray
                | None => Js.Dict.empty()
                };
              },
            (),
          ),
        );
        defineLazyObjectProperty(
          viewConfig,
          "Commands",
          descriptor(
            ~get=
              () => {
                let idx = ref(0);
                let viewManager =
                  Js.Dict.get(
                    Obj.magic(NativeModules.nativeModules),
                    Obj.magic(manager),
                  );
                switch (viewManager) {
                | Some(vM) =>
                  Js.Dict.entries(vM)
                  ->Belt.Array.map(((key, value)) =>
                      switch (Js.typeof(value)) {
                      | "function" =>
                        idx := idx^ + 1;
                        (key, idx^);
                      | _ => ("remove", (-1))
                      }
                    )
                  ->Belt.Array.keep(x => x !== ("remove", (-1)))
                  ->Js.Dict.fromArray
                | None => Js.Dict.empty()
                };
              },
            (),
          ),
        );
      | None => ()
      }
    );

  let default = uIManager;
  */
