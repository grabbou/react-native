[@bs.deriving abstract]
type descriptor('a) = {
  get: unit => 'a,
  [@bs.optional]
  enumerable: bool,
  [@bs.optional]
  writeable: bool,
};

[@bs.module]
external defineLazyObjectProperty: ('a, string, descriptor('b)) => unit =
  "defineLazyObjectProperty";
