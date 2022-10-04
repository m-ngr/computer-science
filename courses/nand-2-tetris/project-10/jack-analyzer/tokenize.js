// Mahmoud Elnagar - 26/09/2022

import { readFileSync } from "fs";
import { makeNode, NODE_TYPE, KEYWORDS, SYMBOLS } from "./grammar.js";
import { tokenStream } from "./utils/tokenStream.js";

/**
 * produces array of AST terminal nodes
 * @param {string} filePath jack source file
 * @return {[import("./grammar.js").ASTNode]} array of terminal nodes
 */
export function tokenize(filePath) {
  return getTokens(readFileSync(filePath, "utf8"));
}

function getTokens(content) {
  const charStream = tokenStream(content.split(""));
  const tokens = [];
  let buffer = "";

  while (charStream.hasNext()) {
    ignoreComments(charStream);
    const ch = charStream.get();

    if (ch === `"`) {
      if (buffer) tokens.push(makeTerminalNode(buffer, undefined));
      buffer = `"${charStream.getUntil('"').join("")}`;
      if (buffer.includes("\n"))
        throw Error("Syntax Error: String shouldn't be spanned across lines");
      tokens.push(makeTerminalNode(buffer, NODE_TYPE.STRING));
      buffer = "";
    } else if (SYMBOLS.includes(ch)) {
      if (buffer) tokens.push(makeTerminalNode(buffer, undefined));
      tokens.push(makeTerminalNode(ch, NODE_TYPE.SYMBOL));
      buffer = "";
    } else if (isWhiteSpace(ch)) {
      if (buffer) tokens.push(makeTerminalNode(buffer, undefined));
      buffer = "";
    } else {
      buffer += ch;
    }
  }

  return tokens;
}

function isWhiteSpace(ch) {
  return ch === " " || ch === "\n" || ch === "\r" || ch === "\t";
}

function ignoreComments(charStream) {
  if (charStream.next() !== "/") return;
  const ch = charStream.get();

  if (charStream.next() === "/") {
    charStream.getUntil("\n");
    ignoreComments(charStream);
  } else if (charStream.next() === "*") {
    charStream.get();
    do {
      charStream.getUntil("*");
    } while (charStream.next() !== "/");
    charStream.get();
    ignoreComments(charStream);
  } else {
    charStream.put(ch);
  }
}

function makeTerminalNode(value, type) {
  if (type == undefined) type = getNodeType(value);
  if (type === NODE_TYPE.STRING) value = value.slice(1, -1);
  return makeNode(value, type, true);
}

function getNodeType(value) {
  if (KEYWORDS.includes(value)) return NODE_TYPE.KEYWORD;
  if (SYMBOLS.includes(value)) return NODE_TYPE.SYMBOL;
  if (value.startsWith('"') && value.endsWith('"')) return NODE_TYPE.STRING;
  if (isNaN(Number(value)) === false) {
    value = Number(value);
    if (0 <= value && value <= 32767) return NODE_TYPE.INT;
    throw Error(`Value Error: Number ${value} is out of range [0:32767].`);
  }
  if (isNaN(Number(value.at(0)))) return NODE_TYPE.IDENTIFIER;
  throw Error(`Syntax Error: Invalid Token ${value}.`);
}
