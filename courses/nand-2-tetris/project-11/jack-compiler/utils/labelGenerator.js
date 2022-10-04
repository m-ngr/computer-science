// Mahmoud Elnagar - 30/09/2022

export function labelGenerator() {
  const counter = new Map();

  return {
    /**
     * @param {string} label input label
     * @return {string} unique label
     */
    generate: (label) => {
      const n = counter.get(label) ?? 0;
      counter.set(label, n + 1);
      return `${label}_${n}`;
    },
    reset: () => counter.clear(),
  };
}
