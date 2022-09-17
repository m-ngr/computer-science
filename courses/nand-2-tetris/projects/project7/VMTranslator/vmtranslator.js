// Mahmoud Elnagar - 08/09/2022
import { setExtension, parsePath, parse, writeFile } from "./parse.js";
import { translate } from "./translate.js";

function translateFile(path) {
  writeFile(
    setExtension(path, "asm"),
    translate(parse(path), parsePath(path).name)
  );
}

process.argv.slice(2).forEach((file) => {
  translateFile(file);
});
