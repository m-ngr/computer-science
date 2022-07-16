// =========================== currying basics ============================

const sum = (x, y) => x + y; // Regular function
const multiply = (x) => (y) => x * y; // Curried function

console.log(sum(2, 3)); //prints: 5
console.log(multiply(2)(3)); //prints: 6

// partial application
const double = multiply(2);
const triple = multiply(3);

console.log(double(3)); //prints: 6
console.log(triple(3)); //prints: 9

// ======================== Manual Currying Basics =========================

// convert regular function to curried function
const curryBasic = (f) => (x) => (y) => f(x, y);
// convert curried function to regular function
const uncurryBasic = (f) => (x, y) => f(x)(y);

const curriedSum = curryBasic(sum);
const regularMultiply = uncurryBasic(multiply);

console.log(curriedSum(2)(3)); //prints: 5
console.log(regularMultiply(2, 3)); //prints: 6

// ============================ The Stepping Function ========================

function stepping(f, steps) {
  if (steps < 2) return f;

  function stepper(steps, prevArgs) {
    return (...args) => {
      if (steps === 0) return f(...prevArgs, ...args);
      return stepper(steps - 1, prevArgs.concat(args));
    };
  }

  return stepper(steps - 1, []);
}

const f1 = stepping((x, y, z) => x + y + z, 2);

console.log(f1(1)(2, 3)); //prints: 6
console.log(f1(3, 4, 5)()); //prints: 12
console.log(f1()(2, 2, 1)); //prints: 5
console.log(f1(1, 2)(3, 4, 5)); //prints: 6

// ============================= The Curry Function ==========================

const curry = (f) => stepping(f, f.length);

const f2 = curry((x, y, z) => x + y + z);

console.log(f2(1)(2)(3)); // typical call to a curried function
console.log(f2(1, 2)()(4)); // unbalanced calls
console.log(f2()(2, 4, 6)()); // unbalanced calls
console.log(f2()(1, 2)(3, 4)); // extra args are ignored

// =========================== The Uncurry Function ==========================

function uncurry(cf) {
  return (...args) => {
    let f = cf;
    for (const arg of args) {
      f = f(arg);
      if (typeof f !== "function") return f;
    }
    throw Error("too few arguments");
  };
}

const f3 = uncurry((x) => (y) => (z) => x + y + z);
console.log(f3(3, 2, 3)); // typical call to a regular function
console.log(f3(1, 2, 3, 4)); // extra args are ignored
console.log(f3(1, 2)); // will throw an error

// =========================== The Hycurry Function ==========================

function hycurry(f) {
  return function hybrid(...args) {
    if (args.length >= f.length) return f(...args);

    return (...restArgs) => hybrid(...args, ...restArgs);
  };
}

const f4 = hycurry((x, y, z) => x + y + z);
console.log(f4(3, 2, 3)); // typical call to a regular function
console.log(f4(1)(2)(3)); // typical call to a curried function
console.log(f4(1)(2, 3)); // hybrid call
console.log(f4(4, 5)(6, 7)); // extra args are ignored

// =========================== The Recurry Function ==========================

function recurry(cf) {
  return (...args) => {
    let f = cf;
    for (const arg of args) {
      f = f(arg);
      if (typeof f !== "function") return f;
    }
    return recurry(f);
  };
}

const f5 = recurry((x) => (y) => (z) => x + y + z);

console.log(f5(3, 2, 3)); // typical call to a regular function
console.log(f5(1)(2)(3)); // typical call to a curried function
console.log(f5(1)(2, 3)); // hybrid call
console.log(f5(4, 5)(6, 7)); // extra args are ignored

// =========================== The Partial Function ==========================
// applys f partially with args
function partial(f, ...args) {
  return (...restArgs) => f(...args, ...restArgs);
}

// ========================= JS- Preserving Context ==========================
{
  // context preserving version
  function stepping(f, steps) {
    if (steps < 2) return f;

    function stepper(steps, prevArgs) {
      return function (...args) {
        if (steps === 0) return f.call(this, ...prevArgs, ...args);
        return stepper.call(this, steps - 1, prevArgs.concat(args));
      };
    }

    return stepper.call(this, steps - 1, []);
  }

  const curry = (f) => stepping(f, f.length);

  // context preserving version
  function uncurry(cf) {
    return function (...args) {
      let f = cf;
      for (const arg of args) {
        f = f.call(this, arg);
        if (typeof f !== "function") return f;
      }
      throw Error("too few arguments");
    };
  }

  // context preserving version
  function hycurry(f) {
    return function hybrid(...args) {
      if (args.length >= f.length) return f.call(this, ...args);

      return function (...restArgs) {
        return hybrid.call(this, ...args, ...restArgs);
      };
    };
  }

  // context preserving version
  function recurry(cf) {
    return function (...args) {
      let f = cf;
      for (const arg of args) {
        f = f.call(this, arg);
        if (typeof f !== "function") return f;
      }
      return recurry.call(this, f);
    };
  }

  // context preserving version
  function partial(f, ...args) {
    return function (...restArgs) {
      return f.call(this, ...args, ...restArgs);
    };
  }
}
