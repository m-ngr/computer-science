// Mahmoud Elnagar - 18/09/2022
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
   * @param {number | string} number non-negative number
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
   * @param {number | string} number non-negative number
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
   * - set `deref = true` to get the value `addr[i]`
   * - set `deref = false` to get the address `addr + i`
   * @param {string} reg Register(s) {AMD}
   * @param {string} addr Memory Address (Pointer)
   * @param {string | number} i Index
   * @param {boolean} deref defererence `(RAM[addr] + i)`
   * @returns {[string]} Array of hack commands
   */
  index: (reg, addr, i, deref = true) => {
    i = Number(i);
    if (i === 0) return Register.read(reg, addr, deref);

    if (deref) {
      if (i === 1) return Register.add1("A", addr, false).concat(`${reg}=M`);
      if (i === -1) return Register.sub1("A", addr, false).concat(`${reg}=M`);
      if (i < 0) return Register.sub("A", addr, -i, false).concat(`${reg}=M`);
      return Register.add("A", addr, i, false).concat(`${reg}=M`);
    } else {
      if (i === 1) return Register.add1(reg, addr, false);
      if (i === -1) return Register.sub1(reg, addr, false);
      if (i < 0) return Register.sub(reg, addr, -i, false);
      return Register.add(reg, addr, i, false);
    }
  },
};
