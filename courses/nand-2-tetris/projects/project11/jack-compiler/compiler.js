// Mahmoud Elnagar - 30/09/2022
import { parse } from "./parse.js";
import { tokenize } from "./tokenize.js";
import { writeFileSync, lstatSync, readdirSync } from "fs";
import { join, parse as parsePath } from "path";
import { compile } from "./compile.js";
import { astToXml } from "./utils/astToXml.js";

function compileFile(filePath) {
  const ast = parse(tokenize(filePath));
  if (produceXML) {
    const xml = astToXml(ast);
    writeFileSync(setExtension(filePath, "xml"), xml);
  }
  const vm = compile(ast);
  writeFileSync(setExtension(filePath, "vm"), vm);
}

function setExtension(path, newExt) {
  const { dir, name } = parsePath(path);
  if (!newExt.startsWith(".")) newExt = "." + newExt;
  if (lstatSync(path).isFile()) return join(dir, name + newExt);
  return join(path, name + newExt);
}

function compilePath(path) {
  if (lstatSync(path).isFile()) return compileFile(path);

  readdirSync(path)
    .filter((file) => file.endsWith(".jack"))
    .map((file) => join(path, file))
    .forEach((file) => compileFile(file));
}

const produceXML = process.argv.includes("-xml");

process.argv
  .slice(2)
  .filter((token) => token !== "-xml")
  .forEach((path) => compilePath(path));
