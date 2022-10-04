// Mahmoud Elnagar - 30/09/2022
import { NODE_TYPE } from "./grammar.js";
import { labelGenerator } from "./utils/labelGenerator.js";
import { symbolTable } from "./utils/symbolTable.js";

const env = {
  className: "",
  classEnv: symbolTable(),
  subEnv: symbolTable(),
  has: (varName) => env.subEnv.has(varName) || env.classEnv.has(varName),
  get: (varName) => env.subEnv.get(varName) ?? env.classEnv.get(varName),
};

const labMan = labelGenerator();

/**
 * convert the given AST to VM Code.
 * @param {import("./grammar.js").ASTNode} ast Abstract Syntax Tree
 * @returns {string} VM Code
 */
export function compile(ast) {
  labMan.reset();
  return compileNode(ast).flat(10).join("\n");
}

/**
 * @param {import("./grammar.js").ASTNode} ast Abstract Syntax Tree
 * @returns {[string]} array of VM Code
 */
function compileNode(ast) {
  switch (ast.type) {
    case NODE_TYPE.CLASS:
      return compileClass(ast);
    case NODE_TYPE.CLASS_VAR_DEC:
      return compileClassVarDec(ast);
    case NODE_TYPE.SUBROUTINE_DEC:
      return compileSubroutineDec(ast);
    case NODE_TYPE.PARAM_LIST:
      return compileParameterList(ast);
    case NODE_TYPE.SUBROUTINE_BODY:
      return compileSubroutineBody(ast);
    case NODE_TYPE.VAR_DEC:
      return compileVarDec(ast);
    case NODE_TYPE.STATEMENTS:
      return compileStatements(ast);
    case NODE_TYPE.LET:
      return compileLet(ast);
    case NODE_TYPE.IF:
      return compileIf(ast);
    case NODE_TYPE.WHILE:
      return compileWhile(ast);
    case NODE_TYPE.DO:
      return compileDo(ast);
    case NODE_TYPE.RETURN:
      return compileReturn(ast);
    case NODE_TYPE.EXP:
      return compileExpression(ast);
    case NODE_TYPE.TERM:
      return compileTerm(ast);
    case NODE_TYPE.EXP_LIST:
      return compileExpList(ast);
    case NODE_TYPE.STRING:
      return compileString(ast);
    case NODE_TYPE.SYMBOL:
      return compileOperator(ast);
    default:
      throw Error(`Compilation Error: Invalid node type (${ast.type}).`);
  }
}

function pushVar(varName) {
  if (env.has(varName)) {
    const { kind, index } = env.get(varName);
    return `push ${kind} ${index}`;
  }

  throw Error(`Compilation Error: Unknown variable name (${varName}).`);
}

function popVar(varName) {
  if (env.has(varName)) {
    const { kind, index } = env.get(varName);
    return `pop ${kind} ${index}`;
  }

  throw Error(`Compilation Error: Unknown variable name (${varName}).`);
}

function pushArray(varAst, expAst) {
  return [
    compileExpression(expAst),
    pushVar(varAst.value),
    "add",
    "pop pointer 1",
    "push that 0",
  ];
}

function popArray(varAst, expAst) {
  return [
    compileExpression(expAst),
    pushVar(varAst.value),
    "add",
    "pop pointer 1",
    "pop that 0",
  ];
}

function typeError(ast) {
  throw Error(`Compilation Error: Invalid node type (${ast.type}).`);
}

function compileClass(ast) {
  if (ast.type !== NODE_TYPE.CLASS) return typeError(ast);
  env.className = ast.value[1].value;
  env.classEnv.clear();

  return ast.value
    .slice(3, -1)
    .map((node) => compileNode(node))
    .flat(10);
}

function compileClassVarDec(ast) {
  if (ast.type !== NODE_TYPE.CLASS_VAR_DEC) return typeError(ast);

  let kind = ast.value[0].value;
  if (kind === "field") kind = "this";
  const type = ast.value[1].value;

  ast.value.slice(2, -1).forEach((node) => {
    if (node.type === NODE_TYPE.IDENTIFIER) {
      env.classEnv.set(node.value, type, kind);
    }
  });

  return [];
}

function compileSubroutineDec(ast) {
  if (ast.type !== NODE_TYPE.SUBROUTINE_DEC) return typeError(ast);

  env.subEnv.clear();

  const subType = ast.value[0].value;
  const subName = ast.value[2].value;
  //define arguments: to compile body.
  if (subType === "method") env.subEnv.set("this", env.className, "argument");
  compileParameterList(ast.value[4]);
  //compile body: to get number of locals
  const bodyCode = compileSubroutineBody(ast.value[6]);

  const code = [
    `function ${env.className}.${subName} ${env.subEnv.numberOf("local")}`,
  ];

  if (subType === "constructor") {
    code.push(
      `push constant ${env.classEnv.numberOf("this")}`,
      "call Memory.alloc 1",
      "pop pointer 0"
    );
  } else if (subType === "method") {
    code.push("push argument 0", "pop pointer 0");
  }

  code.push(bodyCode);

  return code.flat(10);
}

function compileParameterList(ast) {
  if (ast.type !== NODE_TYPE.PARAM_LIST) return typeError(ast);

  const pList = ast.value.filter((node) => node.type !== NODE_TYPE.SYMBOL);

  for (let i = 1; i < pList.length; i += 2) {
    env.subEnv.set(pList[i].value, pList[i - 1].value, "argument");
  }

  return [];
}

function compileSubroutineBody(ast) {
  if (ast.type !== NODE_TYPE.SUBROUTINE_BODY) return typeError(ast);

  return ast.value
    .slice(1, -1)
    .map((node) => compileNode(node))
    .flat(10);
}

function compileVarDec(ast) {
  if (ast.type !== NODE_TYPE.VAR_DEC) return typeError(ast);
  const type = ast.value[1].value;

  ast.value.slice(2, -1).forEach((node) => {
    if (node.type === NODE_TYPE.IDENTIFIER) {
      env.subEnv.set(node.value, type, "local");
    }
  });

  return [];
}

function compileStatements(ast) {
  if (ast.type !== NODE_TYPE.STATEMENTS) return typeError(ast);

  return ast.value.map((node) => compileNode(node)).flat(10);
}

function compileLet(ast) {
  if (ast.type !== NODE_TYPE.LET) return typeError(ast);
  const code = compileExpression(ast.value.at(-2));

  if (ast.value[2].value === "=") {
    code.push(popVar(ast.value[1].value));
  } else {
    code.push(popArray(ast.value[1], ast.value[3]));
  }

  return code.flat(10);
}

function compileIf(ast) {
  if (ast.type !== NODE_TYPE.IF) return typeError(ast);
  const code = [];
  const trueLabel = labMan.generate("TRUE_BRANCH");
  const endLabel = labMan.generate("END_IF");
  const hasElse = ast.value.length > 9;

  code.push(
    compileExpression(ast.value[2]),
    `if-goto ${trueLabel}`,
    hasElse ? compileStatements(ast.value[9]) : [],
    `goto ${endLabel}`,
    `label ${trueLabel}`,
    compileStatements(ast.value[5]),
    `label ${endLabel}`
  );

  return code.flat(10);
}

function compileWhile(ast) {
  if (ast.type !== NODE_TYPE.WHILE) return typeError(ast);
  const code = [];
  const startLabel = labMan.generate("LOOP");
  const endLabel = labMan.generate("END_LOOP");

  code.push(
    `label ${startLabel}`,
    compileExpression(ast.value[2]),
    "not",
    `if-goto ${endLabel}`,
    compileStatements(ast.value[5]),
    `goto ${startLabel}`,
    `label ${endLabel}`
  );

  return code.flat(10);
}

function compileDo(ast) {
  if (ast.type !== NODE_TYPE.DO) return typeError(ast);
  return callSubroutine(ast.value.slice(1, -1)).concat("pop temp 0");
}

function compileReturn(ast) {
  if (ast.type !== NODE_TYPE.RETURN) return typeError(ast);
  const code = [];
  if (ast.value[1].type === NODE_TYPE.EXP) {
    code.push(compileExpression(ast.value[1]));
  } else {
    code.push("push constant 0");
  }
  code.push("return");
  return code.flat(10);
}

function compileExpression(ast) {
  if (ast.type !== NODE_TYPE.EXP) return typeError(ast);
  const code = compileTerm(ast.value[0]);
  const exp = ast.value.slice(1);

  for (let i = 1; i < exp.length; i += 2) {
    code.push(compileTerm(exp[i]), compileOperator(exp[i - 1]));
  }

  return code.flat(10);
}

function compileTerm(ast) {
  if (ast.type !== NODE_TYPE.TERM) return typeError(ast);

  switch (ast.value[0].type) {
    case NODE_TYPE.INT:
      return [`push constant ${ast.value[0].value}`];
    case NODE_TYPE.STRING:
      return compileString(ast.value[0]);
    case NODE_TYPE.KEYWORD:
      if (ast.value[0].value === "this") return ["push pointer 0"];
      if (ast.value[0].value === "true") return ["push constant 1", "neg"];
      return ["push constant 0"];
    case NODE_TYPE.SYMBOL:
      const code = compileNode(ast.value[1]);
      if (ast.value.length === 2) {
        code.push(compileOperator(ast.value[0], true));
      }
      return code.flat(10);
    case NODE_TYPE.IDENTIFIER:
      if (ast.value.length === 1) return [pushVar(ast.value[0].value)];
      if (ast.value[1].value === "[") {
        return pushArray(ast.value[0], ast.value[2]);
      }
      return callSubroutine(ast.value);
    default:
      throw Error("Compilation Error: Invalid Term");
  }
}

function compileExpList(ast) {
  if (ast.type !== NODE_TYPE.EXP_LIST) return typeError(ast);
  return ast.value
    .map((node) => {
      if (node.type === NODE_TYPE.SYMBOL) return [];
      return compileExpression(node);
    })
    .flat(10);
}

function callSubroutine(nodes = []) {
  const nArgs = nodes
    .at(-2)
    .value.filter((n) => n.type === NODE_TYPE.EXP).length;
  const argCode = compileExpList(nodes.at(-2));
  const idName = nodes[0].value;

  if (nodes.length === 4) {
    return [
      "push pointer 0",
      argCode,
      `call ${env.className}.${idName} ${nArgs + 1}`,
    ].flat(10);
  }

  const subName = nodes[2].value;

  if (env.has(idName)) {
    return [
      pushVar(idName),
      argCode,
      `call ${env.get(idName).type}.${subName} ${nArgs + 1}`,
    ].flat(10);
  }

  return [argCode, `call ${idName}.${subName} ${nArgs}`].flat(10);
}

function compileOperator(ast, unary = false) {
  if (ast.type !== NODE_TYPE.SYMBOL) return typeError(ast);

  switch (ast.value) {
    case "+":
      return ["add"];
    case "-":
      return unary ? ["neg"] : ["sub"];
    case "*":
      return ["call Math.multiply 2"];
    case "/":
      return ["call Math.divide 2"];
    case "&":
      return ["and"];
    case "|":
      return ["or"];
    case "~":
      return ["not"];
    case "<":
      return ["lt"];
    case ">":
      return ["gt"];
    case "=":
      return ["eq"];
    default:
      throw Error(`Compilation Error: Unknown operator (${ast.value}).`);
  }
}

function compileString(ast) {
  if (ast.type !== NODE_TYPE.STRING) return typeError(ast);
  const code = [`push constant ${ast.value.length}`, "call String.new 1"];

  ast.value.split("").forEach((ch) => {
    code.push(`push constant ${ch.charCodeAt(0)}`, "call String.appendChar 2");
  });

  return code.flat(10);
}
