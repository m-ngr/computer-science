// Mahmoud Elnagar - 26/09/2022
import { parse } from "./parse.js";
import { tokenize } from "./tokenize.js";
import { astToXml } from "./utils/astToXml.js";
import { writeFileSync, lstatSync, readdirSync } from "fs";
import { join, parse as parsePath } from "path";

function analyzeFile(filePath) {
  const xml = astToXml(parse(tokenize(filePath)));
  writeFileSync(setExtension(filePath, "xml"), xml);
}

function setExtension(path, newExt) {
  const { dir, name } = parsePath(path);
  if (!newExt.startsWith(".")) newExt = "." + newExt;
  if (lstatSync(path).isFile()) return join(dir, name + newExt);
  return join(path, name + newExt);
}

function analyze(path) {
  if (lstatSync(path).isFile()) return analyzeFile(path);

  readdirSync(path)
    .filter((file) => file.endsWith(".jack"))
    .map((file) => join(path, file))
    .forEach((file) => analyzeFile(file));
}

process.argv.slice(2).forEach((path) => analyze(path));
