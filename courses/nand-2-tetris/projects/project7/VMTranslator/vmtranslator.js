// Mahmoud Elnagar - 08/09/2022
import { parseFile, writeFile } from "./parse.js";
import { translate } from "./translate.js";

function translateFile(path) {
  const pos = path.lastIndexOf(".");
  const outputPath = path.substr(0, pos < 0 ? path.length : pos) + ".asm";
  writeFile(outputPath, translate(parseFile(path), getFileName(path)));
}

function getFileName(path) {
  path = path.replace(/^.*[\\\/]/, "");
  const pos = path.lastIndexOf(".");
  const fileName = path.substr(0, pos < 0 ? path.length : pos);
  return fileName;
}

process.argv.slice(2).forEach((file) => {
  translateFile(file);
});
