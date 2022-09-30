// Mahmoud Elnagar - 26/09/2022
import {
  CLASS_VAR,
  KW_CONST,
  makeNode,
  NODE_TYPE,
  OP,
  STATEMENTS,
  SUBS,
  UNARY_OP,
} from "./grammar.js";
import { tokenStream } from "./utils/tokenStream.js";

let ts = tokenStream([]); // token stream

/**
 * produces an Abstract Syntax Tree of the given `tokens`
 * @param {[import("./grammar.js").ASTNode]} tokens array of terminal AST nodes
 * @returns AST Object
 */
export function parse(tokens) {
  ts = tokenStream(tokens);
  return parseClass();
}

function expect(values = [], types = []) {
  if (values.length === 0 && types.length === 0) {
    throw Error("Call Error: values or types must be supplied.");
  }

  if (types.includes(ts.next().type)) return ts.get();
  if (values.includes(ts.next().value)) return ts.get();

  const vStr = values.length > 0 ? values.join(", ") : "";
  const tStr = types.length > 0 ? types.join(", ") : "";

  if (tStr === "") throw Error(`Syntax Error: one of <${vStr}> is expected.`);
  if (vStr === "") throw Error(`Syntax Error: one of <${tStr}> is expected.`);

  throw Error(`Syntax Error: one of <${vStr}> or <${tStr}> is expected.`);
}

const expectValue = (...values) => expect(values);
const expectType = (...types) => expect([], types);

function parseClass() {
  const value = [
    expectValue("class"),
    expectType(NODE_TYPE.IDENTIFIER),
    expectValue("{"),
  ];

  while (CLASS_VAR.includes(ts.next().value)) {
    value.push(parseClassVarDec());
  }

  while (SUBS.includes(ts.next().value)) {
    value.push(parseSubroutineDec());
  }

  value.push(expectValue("}"));

  return makeNode(value, NODE_TYPE.CLASS, false);
}

function parseClassVarDec() {
  const value = [
    expectValue(...CLASS_VAR),
    parseType(),
    expectType(NODE_TYPE.IDENTIFIER),
  ];

  while (ts.next().value === ",") {
    value.push(ts.get(), expectType(NODE_TYPE.IDENTIFIER));
  }

  value.push(expectValue(";"));

  return makeNode(value, NODE_TYPE.CLASS_VAR_DEC, false);
}

function parseType() {
  return expect(["int", "char", "boolean"], [NODE_TYPE.IDENTIFIER]);
}

function parseSubroutineDec() {
  const value = [
    expectValue(...SUBS),
    ts.next().value === "void" ? ts.get() : parseType(),
    expectType(NODE_TYPE.IDENTIFIER),
    expectValue("("),
    parseParameterList(),
    expectValue(")"),
    parseSubroutineBody(),
  ];

  return makeNode(value, NODE_TYPE.SUBROUTINE_DEC, false);
}

function parseParameterList() {
  const value = [];

  try {
    value.push(parseType(), expectType(NODE_TYPE.IDENTIFIER));
  } catch (error) {
    return makeNode([], NODE_TYPE.PARAM_LIST, false);
  }

  while (ts.next().value === ",") {
    value.push(ts.get(), parseType(), expectType(NODE_TYPE.IDENTIFIER));
  }

  return makeNode(value, NODE_TYPE.PARAM_LIST, false);
}

function parseSubroutineBody() {
  const value = [expectValue("{")];

  while (ts.next().value === "var") {
    value.push(parseVarDec());
  }

  value.push(parseStatements(), expectValue("}"));

  return makeNode(value, NODE_TYPE.SUBROUTINE_BODY, false);
}

function parseVarDec() {
  const value = [
    expectValue("var"),
    parseType(),
    expectType(NODE_TYPE.IDENTIFIER),
  ];

  while (ts.next().value === ",") {
    value.push(ts.get(), expectType(NODE_TYPE.IDENTIFIER));
  }

  value.push(expectValue(";"));

  return makeNode(value, NODE_TYPE.VAR_DEC, false);
}

function parseStatements() {
  const value = [];

  while (STATEMENTS.includes(ts.next().value)) {
    switch (ts.next().value) {
      case "let":
        value.push(parseLet());
        break;
      case "if":
        value.push(parseIf());
        break;
      case "while":
        value.push(parseWhile());
        break;
      case "do":
        value.push(parseDo());
        break;
      case "return":
        value.push(parseReturn());
        break;
      default:
        throw Error(
          `Implementation Error: ${ts.next().value} statement is missing.`
        );
    }
  }

  return makeNode(value, NODE_TYPE.STATEMENTS, false);
}

function parseLet() {
  const value = [expectValue("let"), expectType(NODE_TYPE.IDENTIFIER)];

  if (ts.next().value === "[") {
    value.push(ts.get(), parseExpression(), expectValue("]"));
  }

  value.push(expectValue("="), parseExpression(), expectValue(";"));

  return makeNode(value, NODE_TYPE.LET, false);
}

function parseIf() {
  const value = [
    expectValue("if"),
    expectValue("("),
    parseExpression(),
    expectValue(")"),
    expectValue("{"),
    parseStatements(),
    expectValue("}"),
  ];

  if (ts.next().value === "else") {
    value.push(ts.get(), expectValue("{"), parseStatements(), expectValue("}"));
  }

  return makeNode(value, NODE_TYPE.IF, false);
}

function parseWhile() {
  const value = [
    expectValue("while"),
    expectValue("("),
    parseExpression(),
    expectValue(")"),
    expectValue("{"),
    parseStatements(),
    expectValue("}"),
  ];

  return makeNode(value, NODE_TYPE.WHILE, false);
}

function parseDo() {
  const value = [expectValue("do"), ...parseSubroutineCall(), expectValue(";")];
  return makeNode(value, NODE_TYPE.DO, false);
}

function parseReturn() {
  const value = [expectValue("return")];

  if (ts.next().value !== ";") value.push(parseExpression());

  value.push(expectValue(";"));

  return makeNode(value, NODE_TYPE.RETURN, false);
}

function parseExpression() {
  const value = [parseTerm()];

  while (OP.includes(ts.next().value)) {
    value.push(ts.get(), parseTerm());
  }

  return makeNode(value, NODE_TYPE.EXP, false);
}

function parseTerm() {
  const value = [
    expect(
      [...KW_CONST, ...UNARY_OP, "("],
      [NODE_TYPE.INT, NODE_TYPE.STRING, NODE_TYPE.IDENTIFIER]
    ),
  ];

  switch (value[0].type) {
    case NODE_TYPE.IDENTIFIER:
      if (ts.next().value === "(" || ts.next().value === ".") {
        ts.put(value.pop());
        value.push(...parseSubroutineCall());
      } else if (ts.next().value === "[") {
        value.push(ts.get(), parseExpression(), expectValue("]"));
      }
      break;
    case NODE_TYPE.SYMBOL:
      if (value[0].value === "(") {
        value.push(parseExpression(), expectValue(")"));
      } else if (UNARY_OP.includes(value[0].value)) {
        value.push(parseTerm());
      }
      break;
    default:
      break;
  }

  return makeNode(value, NODE_TYPE.TERM, false);
}

function parseSubroutineCall() {
  const nodes = [expectType(NODE_TYPE.IDENTIFIER)];

  if (ts.next().value === ".") {
    nodes.push(ts.get(), expectType(NODE_TYPE.IDENTIFIER));
  }

  nodes.push(expectValue("("), parseExpList(), expectValue(")"));

  return nodes;
}

function parseExpList() {
  const value = [];

  try {
    value.push(parseExpression());
  } catch (error) {
    return makeNode([], NODE_TYPE.EXP_LIST, false);
  }

  while (ts.next().value === ",") {
    value.push(ts.get(), parseExpression());
  }

  return makeNode(value, NODE_TYPE.EXP_LIST, false);
}
