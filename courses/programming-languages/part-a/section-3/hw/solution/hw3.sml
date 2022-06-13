(* Mahmoud Elnagar 12/06/2022 *)

exception NoAnswer

datatype pattern = Wildcard
		 | Variable of string
		 | UnitP
		 | ConstP of int
		 | TupleP of pattern list
		 | ConstructorP of string * pattern

datatype valu = Const of int
	      | Unit
	      | Tuple of valu list
	      | Constructor of string * valu

fun g f1 f2 p =
    let 
			val r = g f1 f2 
    in
			case p of
					Wildcard          => f1 ()
				| Variable x        => f2 x
				| TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
				| ConstructorP(_,p) => r p
				| _                 => 0
		end

(**** for the challenge problem only ****)

datatype typ = Anything
	     | UnitT
	     | IntT
	     | TupleT of typ list
	     | Datatype of string

(* =============================== Problem 1 =============================== *)
(* string list -> string list
	 filters the given list to have only strings that starts with uppercase letter.
	 Assume all strings have at least 1 character
 *)
val only_capitals = List.filter (fn s => Char.isUpper(String.sub(s,0)));

(* =============================== Problem 2 =============================== *)
(* string list -> string
	 return the first longest string in the given list, "" if list is empty.
*)
val longest_string1 =
	List.foldl (fn (s,r) => if String.size(s) > String.size(r) then s else r) "" ;

(* =============================== Problem 3 =============================== *)
(* string list -> string
	 return the last longest string in the given list, "" if list is empty.
*)
val longest_string2 =
	List.foldl (fn (s,r) => if String.size(s) >= String.size(r) then s else r) "" ;

(* =============================== Problem 4 =============================== *)
(* =============================== Problem 4-A ============================= *)
(* (int * int -> bool) -> string list -> string
	 return the last string that passes the predicate, "" if list is empty.
	 predicate form: pred(string length, last-string-passed-the-predicate length).
*)
fun longest_string_helper p = 
	List.foldl (fn (s,r) => if p(String.size(s),String.size(r)) then s else r) "" ;

(* =============================== Problem 4-B ============================= *)
(* string list -> string
	 return the first longest string in the given list, "" if list is empty.
*)
val longest_string3 = longest_string_helper (fn (s,r) => s > r);

(* =============================== Problem 4-C ============================= *)
(* string list -> string
	 return the last longest string in the given list, "" if list is empty.
*)
val longest_string4 = longest_string_helper (fn (s,r) => s >= r);

(* =============================== Problem 5 =============================== *)
(* string list -> string
	 return the first longest capitalized string in the given list, "" if list is empty.
*)
val longest_capitalized = longest_string1 o only_capitals;

(* =============================== Problem 6 =============================== *)
(* string -> string
	 reverse the given string
*)
val rev_string = String.implode o List.rev o String.explode; 

(* =============================== Problem 7 =============================== *)
(* (’a -> ’b option) -> ’a list -> ’b
	 applying f to each element of the given list, until returns: 
	 	- the first solved answer (v when f(x) evals to SOME v).
	 	- raise NoAnswer if all evals to NONE.
*)
fun first_answer f xs =
	case xs of
		[] => raise NoAnswer
	| x::xs => case f(x) of
							NONE => first_answer f xs
						| SOME v => v
;
(* =============================== Problem 8 =============================== *)
(* (’a -> ’b list option) -> ’a list -> ’b list option
	 applying f to all elements of the given list and return: 
	 	- NONE: if f(x) = NONE for any element.
	 	- SOME lst: where lst is all the lists from all f(x) appended together.
*)

fun all_answers f xs =
	let
		fun ans (xs,acc) = 
			case xs of
				[] => SOME acc
			| x::xs => case f(x) of
										NONE => NONE
									| SOME l => ans(xs, l@acc)
		;
	in
		ans (xs,[])
	end
;
(* =============================== Problem 9 =============================== *)
(* =============================== Problem 9-A ============================= *)
(* pattern -> int
	 return the number of Wildcard patterns in the given pattern
*)
val count_wildcards = g (fn()=> 1) (fn _ => 0);
(* =============================== Problem 9-B ============================= *)
(* pattern -> int
	 return the number of Wildcard patterns + sum of string lengths for all variable patterns
*)
val count_wild_and_variable_lengths = g (fn()=> 1) String.size;
(* =============================== Problem 9-C ============================= *)
(* string * pattern -> int
	 returns the number of times the string appears as a variable in the pattern.
*)
fun count_some_var (s,p) = g (fn()=>0) (fn x => if x = s then 1 else 0) p;
(* =============================== Problem 10 ============================== *)
(* pattern -> bool
	 returns true if all the Variable patterns use different strings.
*)
fun check_pat p =
	let
		fun var_list p =
			case p of
				Variable s => [s]
			| TupleP pl => List.foldl (fn (p,sl) => (var_list p) @ sl) [] pl
			| ConstructorP(_,p) => var_list p
			| _ => []
		;
		fun unique sl =
			case sl of
				[] => true
			| s::sl => (not (List.exists (fn x => x=s) sl)) andalso (unique sl) 
		;	
	in
		unique (var_list p)
	end
;
(* =============================== Problem 11 ============================== *)
(* valu * pattern -> (string * valu) list option
	 return:
	 - NONE: if the pattern doesn't match
	 - SOME lst: where lst is the list of bindings
*)
fun match (v,p) =
	case (v,p) of
		(_, Wildcard) => SOME []
	| (v, Variable s) => SOME [(s,v)]
	| (Unit, UnitP) => SOME []
	| (Const vi, ConstP i) => if vi = i then SOME [] else NONE
	| (Constructor(s1, v) , ConstructorP (s2, p)) =>
			if s1 = s2 then match(v, p) else NONE
	| (Tuple vs, TupleP ps) => 
			if (List.length vs = List.length ps) andalso (check_pat p) 
			then all_answers match (ListPair.zip (vs,ps))
			else NONE
	| _ => NONE
;
(* =============================== Problem 12 ============================== *)
(* valu -> pattern list -> (string * valu) list option
	 return:
	  - NONE: if no pattern in the given list matches
		- SOME lst: where lst is the list of bindings for the first pattern that matches
*)
fun first_match v ps = 
	SOME (first_answer (fn p => match (v,p)) ps) 
	handle NoAnswer => NONE
;
