(* Let go of any programming concepts: start fresh!! *)
(* SE: Static Environement, DE: Dynamic Environement *)

(* variable binding *)
val x = 34;  (*int*)
(* SE: x:int *)
(* DE: x=34 *)
val y = 17;  (*int*)
(* SE: x:int, y:int *)
(* DE: x=34, y=17 *)

(* you can use previous bindings, you can't use upcoming bindings [ML-Specific]*)
val z = (x + y) + (y + 2); (*int*)
(* SE:..., z:int *)
(* DE: x=34, y=17, z=70 *)

val abs_z = if z < 0 then 0 - z else z; (*int*)
(* SE:..., abs_z:int *)
(* DE:..., abs_z=70 *)

(* ============================ Functions ============================ *)
(* function definition *)
fun pow(x:int, y:int) = 
  if y=0
  then 1
  else x * pow(x, y-1);
(* function call*)
pow(3,2);

(* ============================ Compound Data ============================ *)
(* Pairs: just a tuple with two items*)
val p = (2,5); (* Building pairs: any types*)
#1 p + #2 p; (* accessing pair parts *)

(* Tuples *)
val tup = (2,5,"str",true); (* Building tuples *)
if #4 tup then (#1 tup + #2 tup) else 0; (* accessing tuples *)

(* Lists *)
val emptyList = []; (* a' list: a list of any type *)
val list = [1,5]; (* int list: must have the same type *)
val list = 4::list ; (* [cons] adding item to list *)
hd list; (* int ; get the first value of the list *)
tl list; (* int list ; get the rest of the list *)
null list; (* bool ; check if the list is empty *)
[1,2]@[3,4]; (* append two lists *)

(* ============================ Locals ============================ *)

let
  val x = 5;
  val y = 7;
in
  x+y (* x and y are only available here *)
end;

(* ============================ Options ============================ *)
(* represents the idea of something or nothing *)
(* Constructing Options *)
val emptyOption = NONE;  (* a' option *)
val someOption = SOME "Value"; (* string option: any type*)
(* Accessing Options *)
isSome someOption; (* bool *)
valOf someOption; (* string: any type *)

(* ============================ bools ============================ *)
false andalso true; (* supports short-circuit eval *)
true orelse false; (* supports short-circuit eval *)
not true;
5 <> 5; (* not equal; doesn't work with non-equality types, like real*)
5 = 5; (* equal; doesn't work with non-equality types, like real*)

(* ============================ MISC ============================ *)

Real.fromInt 51; (* convert from int to real *)
"he"^"llo"; (* string concat *)

(* ============================ Records ============================ *)
(* Constructing Records (Each-of type) *)
val r = { position=(4,4), name="Ball", hidden=false };
(* Accessing Records *)
if not (#hidden r) 
then #name r
else "";

(* ============================ Datatype Bindings ============================ *)
(* Constructing Datatypes (One-of type) *)

datatype mytype = Type1 of int * int
                | Type2 of string
                | Type0
                | SR_Type of mytype; (* Self-Reference *)

(* Making Values of mytype *)
Type1 (4,5); (* mytype *)
Type2 "Hello"; (* mytype *)
Type0 ;(* mytype *)

(* checking the variant and extracting the data Using Pattern-Matching*)
fun f x = 
  case x of
     Type0 => 0
   | Type1 (a,b) => a+b
   | Type2 s => String.size s
   | SR_Type mt => 4;

f (Type1 (2,3));
(* ============================ Type Synonym ============================ *)
type Position =  int * int;

val pos : Position = (4,5);

type Student = { name: string,
                 age : int,
                 class: string option };

fun student_age(s:Student) = #age s;

(* ============================ Polymorphic Data Type ============================ *)
(* single type parameter *)
datatype 'a mylist = Empty | Cons of 'a * 'a mylist;
(* multi type parameter *)
datatype ('a,'b) tree = Node of 'a * ('a,'b)tree * ('a,'b)tree | Leaf of 'b;

(* Note: ''a are called equality types, types that can be used with = operator.
         ''a is less general than 'a 
*)

(* ============================ Pattern-Matching ============================ *)
(* val p = e; *)
val (x,y) = (1,"p1x0"); (* tuple pattern *)
val {name = a, age = b} = {name="Mahmoud", age= 24}; (* record pattern *)
val [i,j,_,k] = [9,8,7,6]; (* list pattern, with escape char _ *)
(* fun func p = e; *)
fun fnky {fname= f, lname = l} = f ^ " " ^ l; 

fnky {fname="Mahmoud", lname="Elnagar"};

(* Function Pattern *)
fun sumlist1 lon=
  case lon of
    [] => 0
  | a::l => a + (sumlist1 l)
;

(* the above function can be converted to:*)
fun sumlist2 [] = 0
  | sumlist2 (a::l) = a + (sumlist2 l)
;

(* ============================ Exceptions ============================ *)
(* Constructing Exceptions *)
exception ExceptionValue; (* val: exn *)
exception ExceptionConstructor of string; (* fn: string -> exn *)

(* Raising Exceptions *)
(* raise ExceptionValue;
raise (ExceptionConstructor "Error Message"); *)

(* Handle Exceptions *)
(* e1 handle Pe => e2 *)
(raise ExceptionValue) handle ExceptionValue => "Error";
(raise ExceptionConstructor "Message") handle ExceptionConstructor m => m;
5 div 0 handle _ => 1; (* matches any exception *)

(* ============================ Functions ============================ *)
(* Anonymous Functions *)
val anon = fn x => 2 * x;
(* Built-in RTL composition function*)
fun add1(x)= x+1;
fun double(x)= x*2;
val add1ThenDouble = double o add1;
add1ThenDouble(1);
(* Define infix function *)
infix !>;  
fun a !> b = fn x => b(a(x)); (* LTR composition*)
val add1ThenDouble =  add1 !> double;
add1ThenDouble(1);

(* Backup functions *)
(* produce function h(x) that run f(x) if fails(exception) run g(x) *)
fun backup(f,g) = fn x => (f x) handle _ => g x;

(*============================= Currying ===============================*)

fun multiply x = fn y => x * y;
val double = multiply 2; (* int -> int *)
val triple = multiply 3; (* int -> int *)
multiply(2)(3); (*6*)

(* Currying syntactic suger *)
fun multiply x y = x * y;
val double = multiply 2; (* int -> int *) (* partial application *) 
val triple = multiply 3; (* int -> int *) (* partial application *)
multiply 2 3; (*6*)

(* curried abstract functions *)
fun map f xs = 
  case xs of
    [] => []
  | x::xs => f(x)::(map f xs);

(* partial application *)
(* fun doubleAll xs = map double xs;  Not-Good-Style   *)
(* fun f x = g x;  is just the same as: val f = g; *)
val doubleAll = map double; (* Better Style *)

doubleAll [1,2,3];

(* built-in abstract iterator functions are curried functions *)
List.foldl;
List.filter;
List.map;

(* Note: Using partial application to create polymorphic functions will not work due to value restriction *)

val something1 = List.map (fn x =>(x,x)); (* Warning: value restriction *)
(* The workaround is to wrap it in a function *)
fun something2 xs = List.map (fn x =>(x,x)) xs; 

(* Currying & uncurrying *)
fun sum (x,y)= x + y; (* tupled function *)
fun multiply x y = x * y; (* curried function *)
(* convert tupled function to curried function *)
fun currying f x y = f(x,y);
(* convert curried function to tupled function *) 
fun uncurrying f (x,y) = f x y; 

val curriedSum = currying sum;
val tupledMultiply = uncurrying multiply;
(* reordering parameters *)
fun pairMul (a,b) x = (a*x, b*x);
fun swap f x y = f y x; (* generic swap parameter function *)
val mulPair = swap pairMul; (* reorder parameters using swap *)
fun mulPair x ab = pairMul ab x; (* manual parameter reordering *)
val doublePair = mulPair 2;
doublePair (2,4);

(* Note about efficiency: the efficiency difference between regular functions and curried functions is so small that we can neglect it in most cases. Also, in other functional languages like F# and Haskell, curried functions are more efficient than regular functions. Usually, Currying is the norm in these languages*)

(* ============================ Mutable-References ============================ *)
(* Create reference *)
val r1 = ref 5 ; (* r1: int ref *)
(* Mutate the reference *)
r1 := 7;
(* get the value of reference *)
!r1;

(* ============================ Mutual definitions ============================ *)
(* Mutual functions*)
fun f1 x = if x then "OK" else f2 x
and f2 x = if x then f1 x else "Not OK";
f1 true; f1 false; f2 true; f2 false;
(* Mutual types *)
datatype t1 = Str of string | Mutual1 of t2
and t2 = Num of int | Mutual2 of t1;

(* ========================== ML Module System ========================= *)
(* Define a module *)
structure Library =
struct
  fun double x = 2 * x;
  val const = 5;
end
;
(* Use module bindings *)
Library.double 5;
Library.const;
(* Using the signature for module system *)
signature LIBRARY =
sig
  type Number
  val double: int -> Number;
  val const: Number;
end
;
structure Library :> LIBRARY =
struct
  datatype Number = Num of int | Float of real;
  fun double x = Num (2 * x);
  (* any binding not defined in the signature is private *)
  fun private_fun x = x + 1;
  val const = Num(private_fun 5);
end
;