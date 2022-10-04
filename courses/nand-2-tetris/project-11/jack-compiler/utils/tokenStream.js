// Mahmoud Elnagar - 26/09/2022

/**
 * Makes a Stream Object for the given `input`
 * @param {[any]} input input array of tokens
 * @returns Stream Object
 */
export function tokenStream(input) {
  const stream = input.reverse();
  return {
    next: () => stream.at(-1),
    get: () => stream.pop(),
    put: (...tokens) => stream.push(...tokens),
    hasNext: () => stream.length > 0,

    getUntil(delimiter) {
      let buffer = [];
      while (this.hasNext() && this.next() !== delimiter) {
        buffer.push(this.get());
      }

      if (this.hasNext() === false)
        throw Error(
          `Syntax Error: ${delimiter} is expected after '${buffer.join("")}.'`
        );

      buffer.push(this.get());

      return buffer;
    },
  };
}
