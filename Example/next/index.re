open Belt;

module App = {
  let component = ReasonReact.statelessComponent("App");
  let make = (~locale, _children) => {
    ...component,
    render: _self => <View> ...(ReasonReact.string(locale)) </View>,
  };
};

ReactNative.render(props =>
  Map.String.(
    App.make(~locale=getWithDefault(props, "locale", "en"), [||])
  )
);
