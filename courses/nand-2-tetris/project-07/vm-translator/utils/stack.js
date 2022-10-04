// Mahmoud Elnagar - 17/09/2022

import { Array } from "./array.js";
import { Memory } from "./memory.js";
import { Pointer } from "./pointer.js";

export const Stack = {
  /**
   * perform: *SP = `number` ; ++SP ;
   * @param {number | string} number
   * @returns {[string]} Array of hack commands
   */
  pushConstant: (number) =>
    Memory.set("SP", true, number).concat(Pointer.next("SP")),
  /**
   * perform: *SP = RAM[addr] ; ++SP ;
   * @param {string} addr Memory Address
   * @param {boolean} deref dereference `addr`
   * @returns {[string]} Array of hack commands
   */
  push: (addr, deref) =>
    Memory.assign("SP", true, addr, deref).concat(Pointer.next("SP")),
  /**
   * perform: *SP = *( RAM[addr] + i ) ; ++SP ;
   * @param {string} addr Memory Address
   * @param {string | number} i Index
   * @returns {[string]} Array of hack commands
   */
  pushIndex: (addr, i) =>
    Array.read("SP", true, addr, i).concat(Pointer.next("SP")),
  /**
   * perform: --SP; RAM[addr] = *SP ;
   * @param {string} addr Memory Address
   * @param {boolean} deref dereference `addr`
   * @returns {[string]} Array of hack commands
   */
  /**
   * perform: *SP = REG[`reg`] ; ++SP;
   * @param {string} reg Register(s) {AMD}
   * @returns {[string]} Array of hack commands
   */
  pushRegister: (reg) => Pointer.write("SP", reg).concat(Pointer.next("SP")),

  pop: (addr, deref) =>
    Pointer.prev("SP").concat(Memory.assign(addr, deref, "SP", true)),
  /**
   * perform: --SP ; *( RAM[addr] + i ) = *SP ;
   * @param {string} addr Memory Address
   * @param {string | number} i Index
   * @returns {[string]} Array of hack commands
   */
  popIndex: (addr, i) =>
    Pointer.prev("SP").concat(Array.write(addr, i, "SP", true)),
  /**
   * perform: --SP; REG[`reg`] = *SP ;
   * @param {string} reg Register(s) {AMD}
   * @returns {[string]} Array of hack commands
   */
  popRegister: (reg) => Pointer.prev("SP").concat(Pointer.read(reg, "SP")),
};
