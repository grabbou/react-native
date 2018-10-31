let component = ReasonReact.statelessComponent("ReactNative_View");
let make = (children) => {
	...component,
	render: (_self) =>children,
};
