// Mahmoud Elnagar - 08/09/2022
export const pointer = {
  /**
   * perform: ++ptr ; in the hack language
   * @param {string} ptr Pointer Symbol
   * @returns {[string]} Array of hack commands
   */
  next: (ptr) => [`@${ptr}`, "M=M+1"],
  /**
   * perform: --ptr ; in the hack language
   * @param {string} ptr Pointer Symbol
   * @returns {[string]} Array of hack commands
   */
  prev: (ptr) => [`@${ptr}`, "M=M-1"],
  /**
   * perform: *ptr ; in the hack language.
   * the register `M` will have the content of *ptr
   * @param {string} ptr Pointer Symbol
   * @returns {[string]} Array of hack commands
   */
  deref: (ptr) => [`@${ptr}`, "A=M"],
  /**
   * perform: dest = *ptr ; in the hack language
   * @param {string} ptr Pointer Symbol
   * @param {string} dest Pointer Symbol
   * @returns {[string]} Array of hack commands
   */
  read: (ptr, dest) => pointer.deref(ptr).concat(`${dest}=M`),
  /**
   * perform: *ptr = src ; in the hack language
   * @param {string} ptr Pointer Symbol
   * @param {string} src Pointer Symbol
   * @returns {[string]} Array of hack commands
   */
  write: (ptr, src) => pointer.deref(ptr).concat(`M=${src}`),
};

/**
 * perform: dest = src ; in the hack language
 * @param {string} destName
 * @param {boolean} destDeref assign `true` if `destName` is a pointer
 * @param {string} srcName
 * @param {boolean} srcDeref assign `true` if `srcName` is a pointer
 * @returns {[string]} Array of hack commands
 */
export function assign(destName, destDeref, srcName, srcDeref) {
  const commands = [];
  //handle src
  commands.push(`@${srcName}`);
  if (srcDeref) commands.push("A=M");
  commands.push("D=M");
  //handle dest
  commands.push(`@${destName}`);
  if (destDeref) commands.push("A=M");
  commands.push("M=D");

  return commands;
}

/**
 * perform: dest = segment + i ; in the hack language
 * @param {string} segment Virtual Memory Segment
 * @param {number | string} i index
 * @returns {[string]} Array of hack commands
 */
export function index(dest, segment, i) {
  return [`@${segment}`, "D=M", `@${i}`, "D=D+A", `@${dest}`, "M=D"];
}

let ifCounter = -1;
/**
 * Perform, in the hack language:
 *
 * if (`condition`) `trueStatments` else `falseStatements`;
 *
 * - Don't Use Register `A` in the `condition` commands
 * @param {[string]} condition Array of hack commands
 * @param {[string]} trueStatments Array of hack commands
 * @param {[string]} falseStatements Array of hack commands
 * @returns {[string]} Array of hack commands
 */
export function ifStatment(condition, trueStatments, falseStatements) {
  ++ifCounter;
  return [
    `@IF_TRUE_${ifCounter}`,
    ...condition,
    ...falseStatements,
    `@END_IF_${ifCounter}`,
    "0;JMP",
    `(IF_TRUE_${ifCounter})`,
    ...trueStatments,
    `(END_IF_${ifCounter})`,
  ];
}
