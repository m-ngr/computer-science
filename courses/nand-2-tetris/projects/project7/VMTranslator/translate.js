// Mahmoud Elnagar - 08/09/2022
import { pointer, assign, index, ifStatment } from "./utils.js";

export const translate = (commands, filename) => {
  return commands.map((cmd) => translateCommand(cmd, filename).join("\n"));
};

function translateCommand(cmd, filename) {
  const asm = [`// ${cmd.join(" ")}`];

  if (cmd[0] === "pop") return asm.concat(pop(cmd[1], cmd[2], filename));
  if (cmd[0] === "push") return asm.concat(push(cmd[1], cmd[2], filename));

  return asm.concat(compute(cmd[0]));
}

function push(segment, i, filename = "global") {
  switch (segment) {
    case "local":
      return pushVirtualSegment("LCL", i);
    case "argument":
      return pushVirtualSegment("ARG", i);
    case "this":
      return pushVirtualSegment("THIS", i);
    case "that":
      return pushVirtualSegment("THAT", i);
    case "temp":
      return pushVirtualSegment("R5", i);
    case "static":
      return pushSymbol(`${filename}.${i}`);
    case "pointer":
      return pushSymbol(+i === 1 ? "THAT" : "THIS");
    case "constant":
      return pushConstant(i);
    default:
      throw Error("Invalid Segment Name: " + segment);
  }
}

function pop(segment, i, filename = "global") {
  switch (segment) {
    case "local":
      return popVirtualSegment("LCL", i);
    case "argument":
      return popVirtualSegment("ARG", i);
    case "this":
      return popVirtualSegment("THIS", i);
    case "that":
      return popVirtualSegment("THAT", i);
    case "temp":
      return popVirtualSegment("R5", i);
    case "static":
      return popSymbol(`${filename}.${i}`);
    case "pointer":
      return popSymbol(+i === 1 ? "THAT" : "THIS");
    default:
      throw Error("Invalid Segment Name: " + segment);
  }
}

function popVirtualSegment(segment, i) {
  return [].concat(
    index("R13", segment, i),
    pointer.prev("SP"),
    assign("R13", true, "SP", true)
  );
}

function popSymbol(symbol) {
  return [].concat(pointer.prev("SP"), assign(symbol, false, "SP", true));
}

function pushVirtualSegment(segment, i) {
  return [].concat(
    index("R13", segment, i),
    assign("SP", true, "R13", true),
    pointer.next("SP")
  );
}

function pushConstant(i) {
  return [`@${i}`, "D=A"].concat(pointer.write("SP", "D"), pointer.next("SP"));
}

function pushSymbol(symbol) {
  return [].concat(assign("SP", true, symbol, false), pointer.next("SP"));
}

function compute(f) {
  const commands = [];

  commands.push(...pointer.prev("SP"), ...pointer.read("SP", "D"));
  if (isBinaryOperation(f)) {
    commands.push(...pointer.prev("SP"), ...pointer.read("SP", "A"));
  }

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
      commands.push(...ifStatment(["D;JEQ"], ["D=-1"], ["D=0"]));
      break;
    case "gt":
      commands.push("D=A-D");
      commands.push(...ifStatment(["D;JGT"], ["D=-1"], ["D=0"]));
      break;
    case "lt":
      commands.push("D=A-D");
      commands.push(...ifStatment(["D;JLT"], ["D=-1"], ["D=0"]));
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

  commands.push(...pointer.write("SP", "D"), ...pointer.next("SP"));
  return commands;
}

function isBinaryOperation(f) {
  const ops = ["add", "sub", "eq", "gt", "lt", "and", "or"];
  return ops.includes(f);
}
