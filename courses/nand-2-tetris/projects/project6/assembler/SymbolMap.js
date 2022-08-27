// Mahmoud Elnagar - 27/08/2022
export class SymbolMap {
  #symbols = new Map();
  #varIndex = 16;

  constructor() {
    for (let i = 0; i < 16; ++i) {
      this.#symbols.set(`R${i}`, i);
    }
    this.#symbols.set("SCREEN", 16384);
    this.#symbols.set("KBD", 24576);
    this.#symbols.set("SP", 0);
    this.#symbols.set("LCL", 1);
    this.#symbols.set("ARG", 2);
    this.#symbols.set("THIS", 3);
    this.#symbols.set("THAT", 4);
  }

  set = (symbol, value) => this.#symbols.set(symbol, value);
  get = (symbol) => this.#symbols.get(symbol);
  has = (symbol) => this.#symbols.has(symbol);

  forEach = (cb) => this.#symbols.forEach(cb);
  size = () => this.#symbols.size;

  add = (symbol) => this.#symbols.set(symbol, this.#varIndex++);
}
