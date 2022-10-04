// Mahmoud Elnagar - 27/08/2022
import { encode } from "./encode.js";
import { parseFile, writeFile } from "./parse.js";
import { SymbolMap } from "./SymbolMap.js";

function assemble(path) {
  const symbols = new SymbolMap();
  const pos = path.lastIndexOf(".");
  const outputPath = path.substr(0, pos < 0 ? path.length : pos) + ".hack";
  writeFile(outputPath, encode(parseFile(path, symbols), symbols));
}

process.argv.slice(2).forEach((file) => {
  assemble(file);
});
