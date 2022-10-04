// Mahmoud Elnagar - 17/09/2022

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

export const endProgram = () => ["@END_PROGRAM", "(END_PROGRAM)", "0;JMP"];
