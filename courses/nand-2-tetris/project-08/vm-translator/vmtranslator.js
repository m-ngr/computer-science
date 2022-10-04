// Mahmoud Elnagar - 18/09/2022
import { setExtension, parse, writeFile } from "./parse.js";
import { initProgram, endProgram, translate } from "./translate.js";

function translateFile(parsed) {
  writeFile(
    setExtension(parsed.path, "asm"),
    translate(parsed.code, parsed.name).concat(endProgram())
  );
}

function translateDir(parsed) {
  writeFile(
    setExtension(parsed.path, "asm"),
    [
      initProgram(),
      parsed.code.map((f) => translate(f.code, f.name)),
      endProgram(),
    ].flat(10)
  );
}

process.argv.slice(2).forEach((path) => {
  const parseResult = parse(path);
  if (parseResult.type === "file") {
    translateFile(parseResult);
  } else {
    translateDir(parseResult);
  }
});
