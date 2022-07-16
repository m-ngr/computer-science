// ========================== backup Basics ===============================
const willFail = (x, y) => {
  throw "Error";
};
const willPass1 = (x, y) => x + y;
const willPass2 = (x, y) => x * y;

function backupBasic(f, g) {
  return (...args) => {
    try {
      return f(...args);
    } catch {
      return g(...args);
    }
  };
}

const f0 = backupBasic(willFail, willFail); // will fail
const f1 = backupBasic(willFail, willPass2); // will eval willPass2
const f2 = backupBasic(willPass1, willPass2); // will eval willPass1

//console.log(f0(2, 3)); //will fail
console.log(f1(2, 3)); //prints: 6
console.log(f2(2, 3)); //prints: 5

// ======================== The backup function ===========================

function backup(...funs) {
  return (...args) => {
    for (const f of funs) {
      try {
        return f(...args);
      } catch {}
    }
    throw Error("All Backups Fail");
  };
}

const f3 = backup(willFail, willFail, willFail); // will fail
const f4 = backup(willFail, willPass2, willFail); // will eval willPass2
const f5 = backup(willPass1, willPass2, willFail); // will eval willPass1

//console.log(f3(2, 3)); //will fail
console.log(f4(2, 3)); //prints: 6
console.log(f5(2, 3)); //prints: 5

// =================== backup with a custom exception =====================

function backupEx(exception, ...funs) {
  return (...args) => {
    for (const f of funs) {
      try {
        return f(...args);
      } catch {}
    }
    throw exception;
  };
}

const f6 = backupEx(Error("My Exception"), willFail, willFail);

//console.log(f6(2, 3)); //will fail
