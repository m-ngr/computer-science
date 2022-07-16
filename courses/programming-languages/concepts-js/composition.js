// ========================= Composition Basics ======================
const add1 = (x) => x + 1;
const double = (x) => x * 2;
const triple = (x) => x * 3;

// Right to Left Composition
const composeRTL = (f, g) => (x) => f(g(x));
// Left to Right Composition
const composeLTR = (f, g) => (x) => g(f(x));

const doubleThenAdd1 = composeRTL(add1, double);
const add1ThenDouble = composeLTR(add1, double);

console.log(doubleThenAdd1(2)); //prints: 5
console.log(add1ThenDouble(2)); //prints: 6

// ========================= Single Argument Composition ======================

// Right to Left Composition
function compose(...funs) {
  return (x) => funs.reduceRight((x, f) => f(x), x);
}
// Left to Right Composition
function pipe(...funs) {
  return (x) => funs.reduce((x, f) => f(x), x);
}

// triple then double then add1
const computation1 = compose(add1, double, triple);
// add1 then double then triple
const computation2 = pipe(add1, double, triple);

console.log(computation1(2)); //prints: 13
console.log(computation2(2)); //prints: 18

// ========================= Multi Argument Composition ======================

// Right to Left Composition
function composeAll(...funs) {
  return (...xs) => funs.reduceRight((xs, f) => f(...xs), xs);
}
// Left to Right Composition
function pipeAll(...funs) {
  return (...xs) => funs.reduce((xs, f) => f(...xs), xs);
}

const add1All = (...all) => all.map(add1);
const doubleAll = (...all) => all.map(double);
const tripleAll = (...all) => all.map(triple);

/* we can use mapAll to define add1All, doubleAll, and tripleAll

 const mapAll = (f) => (...all) => all.map(f);
 const add1All = mapAll(add1);
 const doubleAll = mapAll(double);
 const tripleAll = mapAll(triple);
*/

// triple then double then add1
const computation1All = composeAll(add1All, doubleAll, tripleAll);
// add1 then double then triple
const computation2All = pipeAll(add1All, doubleAll, tripleAll);

console.log(computation1All(2, 3, 4, 5)); //prints: [13, 19, 25, 31]
console.log(computation2All(2, 3, 4, 5)); //prints: [18, 24, 30, 36]
