// Mahmoud Elnagar - 17/09/2022
import { endProgram, ifStatment, Stack } from "./utils/index.js";

export const translate = (commands, identifier) => {
  return commands
    .map((cmd) => translateCommand(cmd, identifier).join("\n"))
    .concat(endProgram());
};

function translateCommand(cmd, identifier) {
  const asm = [`// ${cmd.join(" ")}`];

  if (cmd[0] === "pop") return asm.concat(pop(cmd[1], cmd[2], identifier));
  if (cmd[0] === "push") return asm.concat(push(cmd[1], cmd[2], identifier));

  return asm.concat(compute(cmd[0]));
}

function push(segment, i, identifier = "global") {
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
      return Stack.push(`${identifier}.${i}`, false);
    case "pointer":
      return Stack.push(+i === 1 ? "THAT" : "THIS", false);
    case "constant":
      return Stack.pushConstant(i);
    default:
      throw Error("Invalid Segment Name: " + segment);
  }
}

function pop(segment, i, identifier = "global") {
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
      return Stack.pop(`${identifier}.${i}`, false);
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
