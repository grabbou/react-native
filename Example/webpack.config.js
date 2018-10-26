const webpack = require("webpack");
const path = require('path');

const paths = require("./paths.js");

module.exports = {
  entry: './lib/es6/src/index.bs.js',
  output: {
    filename: 'bundle.js',
    path: path.resolve(__dirname, 'lib')
  },
  plugins: [
    new webpack.DefinePlugin({
      __DEV__: process.env.NODO_ENV === 'development',
      'process.env': {
        NODE_ENV: JSON.stringify('development'),
        PLATFORM_ENV: JSON.stringify('web')
      }
    })
  ],
  resolve: {
    alias: paths
  },
  module: {
    rules: [
      {
        test: /\.m?js$/,
        exclude: /(node_modules|bower_components)/,
        use: {
          loader: 'babel-loader',
          options: {
            presets: ['@babel/preset-env', "@babel/preset-flow", "@babel/preset-react"],
            plugins: ["@babel/plugin-proposal-class-properties"],
          }
        }
      }
    ]
  },
  
  mode: "development"
};
