// Mahmoud Elnagar - 27/08/2022
import { readFileSync, writeFileSync } from "fs";

export const parseFile = (path, symbols) => {
  const data = readFileSync(path, "utf8")
    .split("\n")
    .map((line) => line.trim().split("//")[0].trim())
    .filter((line) => line.length > 0);
  return extractSymbols(data, symbols);
};

export const writeFile = (path, commands) => {
  writeFileSync(
    path,
    commands.reduce((p, c) => p + c + "\n", "")
  );
};

const extractSymbols = (data, symbols) => {
  let n = 0;
  return data.filter((line, index) => {
    if (line.startsWith("(") && line.endsWith(")")) {
      symbols.set(line.slice(1, -1), index - n);
      n += 1;
      return false;
    }
    return true;
  });
};
