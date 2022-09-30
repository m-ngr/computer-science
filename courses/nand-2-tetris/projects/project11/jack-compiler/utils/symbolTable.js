// Mahmoud Elnagar - 30/09/2022

export function symbolTable() {
  const symbols = new Map();
  const count = { local: 0, argument: 0, static: 0, this: 0 };

  return {
    /**
     * @param {string} name Symbol Name
     * @returns {boolean}
     */
    has: (name) => symbols.has(name),
    /**
     * @param {string} name Symbol Name
     * @returns {{ name:string, type:string, kind:string, index:number } | undefined}
     */
    get: (name) => symbols.get(name),
    /**
     * @param {string} name Symbol Name
     * @param {"int"| "char" | "boolean" | string} type Symbol Type
     * @param {"local" | "argument" | "static" | "this"} kind Symbol Segment Kind
     */
    set: (name, type, kind) => {
      const index = count[kind]++;
      symbols.set(name, { name, type, kind, index });
    },
    /**
     * @param {"local" | "argument" | "static" | "this"} kind Symbol Segment Kind
     * @returns {number}
     */
    numberOf: (kind) => count[kind],
    clear: () => {
      symbols.clear();
      count.argument = 0;
      count.local = 0;
      count.static = 0;
      count.this = 0;
    },
  };
}
