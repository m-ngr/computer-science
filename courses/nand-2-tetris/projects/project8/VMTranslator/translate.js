// Mahmoud Elnagar - 18/09/2022

import { Array } from "./utils/array.js";
import { Memory } from "./utils/memory.js";
import { Stack } from "./utils/stack.js";
import { Pointer } from "./utils/pointer.js";
import { ifStatment, makeLabelManager } from "./utils/common.js";

let labMan = makeLabelManager("Program"); //Label Manager

export const translate = (commands, fileName) => {
  labMan = makeLabelManager(fileName);
  return commands.map((cmd) => translateCommand(cmd)).flat(10);
};

export const endProgram = () => ["@END_PROGRAM", "(END_PROGRAM)", "0;JMP"];

export const initProgram = () =>
  [Memory.set("SP", false, 256), callFunction("Sys.init", 0)].flat(10);

function translateCommand(cmd) {
  const asm = [`// ${cmd.join(" ")}`];

  switch (cmd[0]) {
    case "pop":
      return asm.concat(pop(cmd[1], cmd[2]));
    case "push":
      return asm.concat(push(cmd[1], cmd[2]));
    case "goto":
    case "label":
    case "if-goto":
      return asm.concat(branching(cmd[0], cmd[1]));
    case "call":
      return asm.concat(callFunction(cmd[1], cmd[2]));
    case "function":
      return asm.concat(declareFunction(cmd[1], cmd[2]));
    case "return":
      return asm.concat(functionReturn());
    default:
      return asm.concat(compute(cmd[0]));
  }
}

function push(segment, i) {
  switch (segment) {
    case "local":
      return Stack.pushIndex("LCL", i);
    case "argument":
      return Stack.pushIndex("ARG", i);
    case "this":
      return Stack.pushIndex("THIS", i);
    case "that":
      return Stack.pushIndex("THAT", i);
    case "temp":
      return Stack.push(`R${5 + +i}`, false);
    case "static":
      return Stack.push(labMan.staticLabel(i), false);
    case "pointer":
      return Stack.push(+i === 1 ? "THAT" : "THIS", false);
    case "constant":
      return Stack.pushConstant(i);
    default:
      throw Error("Invalid Segment Name: " + segment);
  }
}

function pop(segment, i) {
  switch (segment) {
    case "local":
      return Stack.popIndex("LCL", i);
    case "argument":
      return Stack.popIndex("ARG", i);
    case "this":
      return Stack.popIndex("THIS", i);
    case "that":
      return Stack.popIndex("THAT", i);
    case "temp":
      return Stack.pop(`R${5 + +i}`, false);
    case "static":
      return Stack.pop(labMan.staticLabel(i), false);
    case "pointer":
      return Stack.pop(+i === 1 ? "THAT" : "THIS", false);
    default:
      throw Error("Invalid Segment Name: " + segment);
  }
}

function compute(f) {
  const commands = Stack.popRegister("D");

  if (isBinaryOperation(f)) commands.push(Stack.popRegister("A"));

  switch (f) {
    case "add":
      commands.push("D=D+A");
      break;
    case "sub":
      commands.push("D=A-D");
      break;
    case "neg":
      commands.push("D=-D");
      break;
    case "eq":
      commands.push("D=A-D");
      commands.push(ifStatment(["D;JEQ"], ["D=-1"], ["D=0"]));
      break;
    case "gt":
      commands.push("D=A-D");
      commands.push(ifStatment(["D;JGT"], ["D=-1"], ["D=0"]));
      break;
    case "lt":
      commands.push("D=A-D");
      commands.push(ifStatment(["D;JLT"], ["D=-1"], ["D=0"]));
      break;
    case "and":
      commands.push("D=D&A");
      break;
    case "or":
      commands.push("D=D|A");
      break;
    case "not":
      commands.push("D=!D");
      break;
    default:
      throw Error("Invalid Operation: " + f);
  }

  commands.push(Stack.pushRegister("D"));
  return commands.flat(10);
}

function isBinaryOperation(f) {
  const ops = ["add", "sub", "eq", "gt", "lt", "and", "or"];
  return ops.includes(f);
}

function branching(jumpType, label) {
  const identifier = labMan.statementLabel(label);

  switch (jumpType) {
    case "goto":
      return [`@${identifier}`, "0;JMP"];
    case "label":
      return [`(${identifier})`];
    case "if-goto":
      return Stack.popRegister("D").concat(`@${identifier}`, "D;JNE");
    default:
      throw Error(`Invalid Branching Command: ${jumpType}`);
  }
}

function callFunction(funName, numArgs) {
  numArgs = Number(numArgs);
  const returnLabel = labMan.returnLabel();
  return [
    `@${returnLabel}`,
    "D=A",
    Stack.pushRegister("D"),
    Stack.push("LCL", false),
    Stack.push("ARG", false),
    Stack.push("THIS", false),
    Stack.push("THAT", false),
    Memory.sub("ARG", false, "SP", false, 5 + numArgs),
    Memory.assign("LCL", false, "SP", false),
    `@${funName}`,
    "0;JMP",
    `(${returnLabel})`,
  ].flat(10);
}

function declareFunction(funName, numLocals) {
  labMan.setFunction(funName);
  const commands = [`(${funName})`];
  for (let i = 0; i < numLocals; ++i) commands.push(Stack.pushConstant(0));

  return commands.flat(10);
}

function functionReturn() {
  const returnAddress = "R14"; //Temp Register
  return [
    Array.read(returnAddress, false, "LCL", -5),
    Stack.pop("ARG", true),
    Memory.add1("SP", false, "ARG", false),
    Array.read("THAT", false, "LCL", -1),
    Array.read("THIS", false, "LCL", -2),
    Array.read("ARG", false, "LCL", -3),
    Array.read("LCL", false, "LCL", -4),
    Pointer.deref(returnAddress),
    "0;JMP",
  ].flat(10);
}
