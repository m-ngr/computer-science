// Mahmoud Elnagar - 17/09/2022
import { Pointer } from "./pointer.js";

export const Register = {
  /**
   * perform: REG[`reg`] = RAM[`addr`] + 1;
   * - set `reg = M` to update the value at RAM[`addr`]
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {boolean} deref defererence `addr`
   * @returns {[string]} Array of hack commands
   */
  add1: (reg, addr, deref = false) =>
    [deref ? Pointer.deref(addr) : `@${addr}`, `${reg}=M+1`].flat(10),

  /**
   * perform: REG[`reg`] = RAM[`addr`] - 1;
   * - set `reg = M` to update the value at RAM[`addr`]
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {boolean} deref defererence `addr`
   * @returns {[string]} Array of hack commands
   */
  sub1: (reg, addr, deref = false) =>
    [deref ? Pointer.deref(addr) : `@${addr}`, `${reg}=M-1`].flat(10),

  /**
   * perform: REG[`reg`] = RAM[`addr`] + `number`;
   * - set `reg = M` to update the value at RAM[`addr`]
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {number | string} number
   * @param {boolean} deref defererence `addr`
   * @returns {[string]} Array of hack commands
   */
  add: (reg, addr, number, deref = false) =>
    [
      `@${number}`,
      "D=A",
      deref ? Pointer.deref(addr) : `@${addr}`,
      `${reg}=D+M`,
    ].flat(10),

  /**
   * perform: REG[`reg`] = RAM[`addr`] - `number`;
   * - set `reg = M` to update the value at RAM[`addr`]
   * - set `neg = true` to perform: `number` - RAM[addr]
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {number | string} number
   * @param {boolean} deref defererence `addr`
   * @param {boolean} neg negate the result
   * @returns {[string]} Array of hack commands
   */
  sub: (reg, addr, number, deref = false, neg = false) =>
    [
      `@${number}`,
      "D=A",
      deref ? Pointer.deref(addr) : `@${addr}`,
      `${reg}=${neg ? "D-M" : "M-D"}`,
    ].flat(10),
  /**
   * perform: REG[reg] = RAM[addr] ;
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {boolean} deref defererence `addr`
   * @returns {[string]} Array of hack commands
   */
  read: (reg, addr, deref = false) =>
    [deref ? Pointer.deref(addr) : `@${addr}`, `${reg}=M`].flat(10),
  /**
   * perform: RAM[addr] = REG[reg] ;
   * - set `deref = true` if `addr` is a pointer
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address
   * @param {boolean} deref defererence `addr`
   * @returns {[string]} Array of hack commands
   */
  write: (reg, addr, deref = false) =>
    [deref ? Pointer.deref(addr) : `@${addr}`, `M=${reg}`].flat(10),

  /**
   * perform: REG[reg] = *(RAM[addr] + i) ;
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address (Pointer)
   * @param {string | number} i Index
   * @returns {[string]} Array of hack commands
   */
  index: (reg, addr, i) => Register.add("A", addr, i, false).concat(`${reg}=M`),
};
