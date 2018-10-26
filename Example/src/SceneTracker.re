[@bs.deriving abstract]
type scene = {name: string};

let listeners: ref(array(scene => unit)) = ref([||]);
let activeScene = ref(scene(~name="default"));

let setActiveScene = scene => {
  activeScene := scene;
  Belt.Array.forEach(listeners^, l => l(activeScene^));
};

let getActiveScene = () => activeScene^;

let addActiveSceneChangedListener = callback => {
  let _ = Js.Array.push(callback);
  {
    "remove": () =>
      listeners :=
        Belt.Array.keep(listeners^, listener => listener !== callback),
  };
};
