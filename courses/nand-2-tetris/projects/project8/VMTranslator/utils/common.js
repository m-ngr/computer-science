// Mahmoud Elnagar - 18/09/2022

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
/**
 *
 * @param {string} fileName
 * @param {string} currentFunction
 * @returns Label Manager Object
 */
export function makeLabelManager(fileName, currentFunction = "global") {
  let counter = 0;

  return {
    staticLabel: (number) => `${fileName}${number}`,

    statementLabel: (label) => `${currentFunction}$${label}`,

    returnLabel: () => `${currentFunction}$ret.${counter++}`,

    setFunction: (funName) => {
      currentFunction = funName;
      counter = 0;
    },

    getFunction: () => currentFunction,

    getFileName: () => fileName,
  };
}
