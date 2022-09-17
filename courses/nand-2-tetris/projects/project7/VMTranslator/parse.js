// Mahmoud Elnagar - 17/09/2022
import { readFileSync, writeFileSync, lstatSync, readdirSync } from "fs";
import { join, parse as parsePath } from "path";

const parseFile = (path) =>
  readFileSync(path, "utf8")
    .split("\n")
    .map((line) => line.trim().split("//")[0].trim())
    .filter((line) => line.length > 0)
    .map((line) =>
      line
        .split(" ")
        .map((word) => word.trim())
        .filter((word) => word.length > 0)
    );

export const writeFile = (path, lines) => writeFileSync(path, lines.join("\n"));

export function parse(path) {
  if (lstatSync(path).isFile()) return parseFile(path);
  const parsedText = [];

  readdirSync(path)
    .filter((file) => file.endsWith(".vm"))
    .map((file) => join(path, file))
    .forEach((file) => parsedText.push(...parseFile(file)));

  return parsedText;
}

export function setExtension(path, newExt) {
  const { dir, name } = parsePath(path);
  if (!newExt.startsWith(".")) newExt = "." + newExt;
  return join(dir, name + newExt);
}

export { parsePath };
