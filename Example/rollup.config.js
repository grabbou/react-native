import resolve from 'rollup-plugin-node-resolve';

export default {
  input: "lib/es6/src/index.bs.js",
  output: {
    file: "lib/bundle.js",
    format: "iife",
    name: "bundle"
  },
  plugins: [
    resolve()
  ],
  watch: {
    include: "lib/es6/src/**"
  }
}
