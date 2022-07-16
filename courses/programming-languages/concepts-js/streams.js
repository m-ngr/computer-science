// ======================= Immutable Streams (Pairs) ======================
// produce stream by number n
function streamN(fun) {
  const thunkGenerator = (n) => () => [fun(n), thunkGenerator(n + 1)];
  return thunkGenerator(0);
}
// produce stream by value v
function streamV(fun, init) {
  const thunkGenerator = (v) => () => [v, thunkGenerator(fun(v))];
  return thunkGenerator(init);
}

const nats = streamN((n) => n); // 0,1,2,3,4,...
const pows2 = streamN((n) => 2 ** n); // 1,2,4,8,16,...

const odds = streamV((v) => v + 2, 1); // 1,3,5,7,9,...
const evens = streamV((v) => v + 2, 0); // 0,2,4,6,8,...

function printN(stream, n) {
  if (n === 0) return stream; // return the last stream is a good style
  console.log(stream()[0]);
  printN(stream()[1], n - 1);
}

console.log("======== nats stream =========");
printN(nats, 5);
console.log("======== pows2 stream =========");
printN(pows2, 5);
console.log("======== odds stream =========");
printN(odds, 5);
console.log("======== evens stream =========");
printN(evens, 5);

// ======================= Immutable Streams (Objects) ======================

{
  // produce stream by number n
  function streamN(fun) {
    const generator = (n) => ({ value: fun(n), next: () => generator(n + 1) });
    return generator(0);
  }

  // produce stream by value v
  function streamV(fun, init) {
    const generator = (value) => ({ value, next: () => generator(fun(value)) });
    return generator(init);
  }

  const nats = streamN((n) => n); // 0,1,2,3,4,...
  const pows2 = streamN((n) => 2 ** n); // 1,2,4,8,16,...

  const odds = streamV((v) => v + 2, 1); // 1,3,5,7,9,...
  const evens = streamV((v) => v + 2, 0); // 0,2,4,6,8,...

  function printN(stream, n) {
    if (n === 0) return stream; // return the last stream is a good style
    console.log(stream.value);
    printN(stream.next(), n - 1);
  }

  console.log("======== nats stream =========");
  printN(nats, 5);
  console.log("======== pows2 stream =========");
  printN(pows2, 5);
  console.log("======== odds stream =========");
  printN(odds, 5);
  console.log("======== evens stream =========");
  printN(evens, 5);
}

// ========================= Mutable Streams (Objects) ======================

{
  // produce stream by number n
  function streamN(fun) {
    let n = 0;
    return {
      value: () => fun(n),
      next: () => (n += 1),
      reset: () => (n = 0),
    };
  }

  // produce stream by value v
  function streamV(fun, init) {
    let v = init;
    return {
      value: () => v,
      next: () => (v = fun(v)),
      reset: () => (v = init),
    };
  }

  const nats = streamN((n) => n); // 0,1,2,3,4,...
  const pows2 = streamN((n) => 2 ** n); // 1,2,4,8,16,...

  const odds = streamV((v) => v + 2, 1); // 1,3,5,7,9,...
  const evens = streamV((v) => v + 2, 0); // 0,2,4,6,8,...

  function printN(stream, n) {
    for (let i = 0; i < n; i++) {
      console.log(stream.value());
      stream.next();
    }
  }

  console.log("======== nats stream =========");
  printN(nats, 5);
  printN(nats, 5); // will continue from last value
  console.log("======== pows2 stream =========");
  printN(pows2, 5);
  pows2.reset();
  printN(pows2, 5);
  console.log("======== odds stream =========");
  printN(odds, 5);
  printN(odds, 5); // will continue from last value
  console.log("======== evens stream =========");
  printN(evens, 5);
  evens.reset();
  printN(evens, 5);
}
