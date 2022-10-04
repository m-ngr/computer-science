// Mahmoud Elnagar - 17/09/2022

import { Register } from "./register.js";

export const Array = {
  /**
   * perform: RAM[`dest`] = *( RAM[`src`] + i );
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {string} src Memory Address (Pointer to Array)
   * @param {string | number} i Index
   * @returns {[string]} Array of hack commands
   */
  read: (dest, destDeref, src, i) =>
    Register.index("D", src, i).concat(Register.write("D", dest, destDeref)),
  /**
   * perform: *( RAM[`dest`] + i ) = RAM[`src`];
   * @param {string} dest Memory Address (Pointer to Array)
   * @param {string | number} i Index
   * @param {string} src Memory Address
   * @param {boolean} srcDeref dereference `src`
   * @param {string} tempAddr Temporary Memory Address = R13
   * @returns {[string]} Array of hack commands
   */
  write: (dest, i, src, srcDeref, tempAddr = "R13") =>
    Register.add("D", dest, i, false).concat(
      Register.write("D", tempAddr, false),
      Register.read("D", src, srcDeref),
      Register.write("D", tempAddr, true)
    ),
  /**
   * perform: *( RAM[`dest`] + `destIndex` ) = *( RAM[`src`] + `srcIndex` );
   * @param {string} dest Memory Address (Pointer to Array)
   * @param {string | number} destIndex Index of `dest`
   * @param {string} src Memory Address (Pointer to Array)
   * @param {string | number} srcIndex Index of `src`
   * @param {string} tempAddr Temporary Memory Address = R13
   * @returns {[string]} Array of hack commands
   */
  assign: (dest, destIndex, src, srcIndex, tempAddr = "R13") =>
    Register.add("D", dest, destIndex, false).concat(
      Register.write("D", tempAddr, false),
      Register.index("D", src, srcIndex),
      Register.write("D", tempAddr, true)
    ),
};
