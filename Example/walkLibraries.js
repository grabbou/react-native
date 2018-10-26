const walk = require('walk');
const fs = require('fs');
const path = require('path');
const walker = walk.walk("../Libraries");

const files = {};

walker.on("file", (root, fileStats, next) => {
  if (fileStats.name.includes(".js")) {
    files[fileStats.name.replace(".js", "")] = path.join(root, fileStats.name);
  }
  next();
});

walker.on("errors", (root, nodeStatsArray, next) => {
  next();
});

walker.on("end", () => {
  console.log(files);
  fs.writeFile("./paths.json", JSON.stringify(files, null, 2), 'utf8', (err) => {
    console.error(err);
  });
});
