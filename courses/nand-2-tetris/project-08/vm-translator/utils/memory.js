// Mahmoud Elnagar - 18/09/2022
import { Pointer } from "./pointer.js";
import { Register } from "./register.js";

export const Memory = {
  /**
   * perform: RAM[`dest`] = RAM[`src`] + 1;
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {string} src Memory Address
   * @param {boolean} srcDeref dereference `src`
   * @returns {[string]} Array of hack commands
   */
  add1: (dest, destDeref, src, srcDeref) =>
    Register.add1("D", src, srcDeref).concat(
      Register.write("D", dest, destDeref)
    ),
  /**
   * perform: RAM[`dest`] = RAM[`src`] - 1;
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {string} src Memory Address
   * @param {boolean} srcDeref dereference `src`
   * @returns {[string]} Array of hack commands
   */
  sub1: (dest, destDeref, src, srcDeref) =>
    Register.sub1("D", src, srcDeref).concat(
      Register.write("D", dest, destDeref)
    ),
  /**
   * perform: RAM[`dest`] = RAM[`src`] + `n`;
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {string} src Memory Address
   * @param {boolean} srcDeref dereference `src`
   * @param {number | string} n non-negative number
   * @returns {[string]} Array of hack commands
   */
  add: (dest, destDeref, src, srcDeref, n) =>
    Register.add("D", src, n, srcDeref).concat(
      Register.write("D", dest, destDeref)
    ),
  /**
   * perform: RAM[`dest`] = RAM[`src`] - `n`;
   * - set `neg = true` to perform: `n` - RAM[`src`]
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {string} src Memory Address
   * @param {boolean} srcDeref dereference `src`
   * @param {number | string} n non-negative number
   * @param {boolean} neg negate the result
   * @returns {[string]} Array of hack commands
   */
  sub: (dest, destDeref, src, srcDeref, n, neg = false) =>
    Register.sub("D", src, n, srcDeref, neg).concat(
      Register.write("D", dest, destDeref)
    ),
  /**
   * perform: RAM[`dest`] = RAM[`src`] ;
   * @param {string} dest Memory Address (possibly Pointer)
   * @param {boolean} destDeref assign `true` if `destName` is a pointer
   * @param {string} src Memory Address (possibly Pointer)
   * @param {boolean} srcDeref assign `true` if `srcName` is a pointer
   * @returns {[string]} Array of hack commands
   */
  assign: (dest, destDeref, src, srcDeref) =>
    Register.read("D", src, srcDeref).concat(
      Register.write("D", dest, destDeref)
    ),
  /**
   * perform: RAM[`dest`] = `number`;
   * @param {string} dest Memory Address
   * @param {boolean} destDeref dereference `dest`
   * @param {number | string} number
   * @returns {[string]} Array of hack commands
   */
  set: (dest, destDeref, number) => {
    number = Number(number);
    if (-1 <= number && number <= 1) {
      return [destDeref ? Pointer.deref(dest) : `@${dest}`, `M=${number}`].flat(
        10
      );
    }

    return (
      number < 0 ? [`@${-number}`, "D=-A"] : [`@${number}`, "D=A"]
    ).concat(Register.write("D", dest, destDeref));
  },
};
