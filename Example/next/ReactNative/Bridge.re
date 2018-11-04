module Make = (Config: { let handleModuleCall: (string, array(string)) => unit }) => {
  let enqueueNativeCall = () => ();
  let handleNativeRequest = () => ();
};
