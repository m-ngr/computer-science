// Mahmoud Elnagar - 26/09/2022

export const KEYWORDS = Object.freeze([
  "class",
  "constructor",
  "function",
  "method",
  "field",
  "static",
  "var",
  "int",
  "char",
  "boolean",
  "void",
  "true",
  "false",
  "null",
  "this",
  "let",
  "do",
  "if",
  "else",
  "while",
  "return",
]);

export const SYMBOLS = Object.freeze([
  "{",
  "}",
  "(",
  ")",
  "[",
  "]",
  ".",
  ",",
  ";",
  "+",
  "-",
  "*",
  "/",
  "&",
  "|",
  ",",
  "<",
  ">",
  "=",
  "~",
]);

export const STATEMENTS = Object.freeze(["do", "while", "let", "if", "return"]);
export const UNARY_OP = Object.freeze(["-", "~"]);
export const OP = Object.freeze(["+", "-", "*", "/", "&", "|", "<", ">", "="]);
export const SUBS = Object.freeze(["method", "constructor", "function"]);
export const CLASS_VAR = Object.freeze(["static", "field"]);
export const KW_CONST = Object.freeze(["true", "false", "null", "this"]);

export const NODE_TYPE = Object.freeze({
  KEYWORD: "keyword",
  SYMBOL: "symbol",
  INT: "integerConstant",
  STRING: "stringConstant",
  IDENTIFIER: "identifier",
  CLASS: "class",
  CLASS_VAR_DEC: "classVarDec",
  SUBROUTINE_DEC: "subroutineDec",
  PARAM_LIST: "parameterList",
  SUBROUTINE_BODY: "subroutineBody",
  VAR_DEC: "varDec",
  STATEMENTS: "statements",
  LET: "letStatement",
  IF: "ifStatement",
  WHILE: "whileStatement",
  DO: "doStatement",
  RETURN: "returnStatement",
  EXP: "expression",
  TERM: "term",
  EXP_LIST: "expressionList",
});

/**
 * @typedef ASTNode
 * @type {object}
 * @property {string | [ASTNode]} value node body
 * @property {string} type node type (tag)
 * @property {boolean} terminal is terminal node
 *
 * @param {string | [ASTNode]} value node body
 * @param {string} type node type (tag)
 * @param {boolean} terminal is terminal node
 * @returns {ASTNode}
 */
export function makeNode(value, type, terminal) {
  return { value, type, terminal };
}
