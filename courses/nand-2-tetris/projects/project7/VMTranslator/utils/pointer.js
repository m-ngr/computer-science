// Mahmoud Elnagar - 17/09/2022
import { Register } from "./register.js";

export const Pointer = {
  /**
   * performs: ++ptr;
   * @param {string} ptr Pointer Symbol (Memory Address)
   * @returns {[string]} Array of hack commands
   */
  next: (ptr) => Register.add1("M", ptr, false),
  /**
   * performs: --ptr;
   * @param {string} ptr Pointer Symbol (Memory Address)
   * @returns {[string]} Array of hack commands
   */
  prev: (ptr) => Register.sub1("M", ptr, false),
  /**
   * perform: *ptr ;
   * - sets REG[`A`] to have the value of RAM[ptr]
   * - results in REG[`M`] having the value of *ptr
   * @param {string} ptr Pointer Symbol (Memory Address)
   * @returns {[string]} Array of hack commands
   */
  deref: (ptr) => [`@${ptr}`, "A=M"],
  /**
   * perform: reg = *ptr ;
   * @param {string} reg Register(s) {AMD}
   * @param {string} ptr Pointer Symbol (Memory Address)
   * @returns {[string]} Array of hack commands
   */
  read: (reg, ptr) => Pointer.deref(ptr).concat(`${reg}=M`),
  /**
   * perform: *ptr = reg ;
   * @param {string} ptr Pointer Symbol (Memory Address)
   * @param {string} reg Register(s) {AMD}
   * @returns {[string]} Array of hack commands
   */
  write: (ptr, reg) => Pointer.deref(ptr).concat(`M=${reg}`),
};
