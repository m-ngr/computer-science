function showWhen(res, show) {
  if (show) console.log(res);
}

function heavySum(x, y) {
  // Some Really Heavy Computation
  for (let i = 0; i < 3999999999; ++i) {
    x + y;
  }
  return x + y;
}

console.log("Started: Without a Thunk");
// will take time to unnecessarily eval heavySum(2, 3)
showWhen(heavySum(2, 3), false);
console.log("Finished: Without a Thunk");

// use: () => f(...args)
function thunk(f, ...args) {
  return () => f(...args);
}

// The function needs to change its expectation to be a thunk
function showWhenThunk(thunk, show) {
  if (show) console.log(thunk());
}

console.log("Started: With a Thunk");
// will not eval the heavySum
showWhenThunk(() => heavySum(2, 3), false); // better-style
showWhenThunk(thunk(heavySum, 2, 3), false); // good-style
console.log("Finished: With a Thunk");

// ============================ The Lazy Function ============================

function lazy(thunk) {
  let computed = false;
  let result;
  return () => {
    if (!computed) {
      result = thunk();
      computed = true;
    }
    return result;
  };
}

// ============================= Lazy Evaluation Example =====================

function sumNTimes(exp, n) {
  let sum = 0;
  for (let i = 0; i < n; i++) {
    sum += exp;
  }
  return sum;
}

function sumNTimesThunk(thunk, n) {
  let sum = 0;
  for (let i = 0; i < n; i++) {
    sum += thunk();
  }
  return sum;
}

console.log("================================================");

console.log("Started: Without a Thunk");
// will always eval heavySum once.
sumNTimes(heavySum(2, 3), 1000);
console.log("Finished: Without a Thunk");

console.log("================================================");

console.log("Started: With a Thunk");
// will eval heavySum n times
sumNTimesThunk(() => heavySum(2, 3), 10); // Notice: we set n = 10. and still takes more time than both
console.log("Finished: With a Thunk");

console.log("================================================");

console.log("Started: With a Lazy Thunk");
// will eval heavySum 0 or 1 time
sumNTimesThunk(
  lazy(() => heavySum(2, 3)),
  1000
);
console.log("Finished: With a Lazy Thunk");

// ============================== Delay and Force ============================
const delay = (thunk) => [false, thunk];

function force(delayedThunk) {
  if (!delayedThunk[0]) {
    delayedThunk[0] = true;
    delayedThunk[1] = delayedThunk[1]();
  }
  return delayedThunk[1];
}

{
  // OOP version of delay and force
  const delay = (thunk) => ({
    computed: false,
    result: undefined,
    thunk,
    force: function () {
      if (!this.computed) {
        this.computed = true;
        this.result = thunk();
      }
      return this.result;
    },
  });
}
