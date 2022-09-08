// Mahmoud Elnagar - 08/09/2022
import { readFileSync, writeFileSync } from "fs";

export const parseFile = (path) =>
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

export const writeFile = (path, lines) => {
  writeFileSync(
    path,
    lines.reduce((p, c) => p + c + "\n", "")
  );
};
