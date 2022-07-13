(* Mahmoud Elnagar 12/06/2022 *)

use "hw3.sml";

val test1_0 = only_capitals [] = [];
val test1_1 = only_capitals ["a"] = [];
val test1_2 = only_capitals ["aA"] = [];
val test1_3 = only_capitals ["A"] = ["A"];
val test1_4 = only_capitals ["A","B","C"] = ["A","B","C"];
val test1_5 = only_capitals ["a","B","cC"] = ["B"];

val test2_0 = longest_string1 [] = "";
val test2_1 = longest_string1 ["A"] = "A";
val test2_2 = longest_string1 ["A","bc","C"] = "bc";
val test2_3 = longest_string1 ["A","bc","BC","d"] = "bc";

val test3_0 = longest_string2 [] = "";
val test3_1 = longest_string2 ["A"] = "A";
val test3_2 = longest_string2 ["A","bc","C"] = "bc";
val test3_3 = longest_string2 ["A","bc","BC","d"] = "BC";

val test4a_0 = longest_string_helper (fn (x,y)=> x > y) [] = "";
val test4a_1 = longest_string_helper (fn (x,y)=> x > y) ["A"] = "A";
val test4a_2 = longest_string_helper (fn (x,y)=> x > y) ["A", "B"] = "A";
val test4a_3 = longest_string_helper (fn (x,y)=> x = 2) ["AB", "BC","d"] = "BC";

val test4b_0 = longest_string3 [] = "";
val test4b_1 = longest_string3 ["A"] = "A";
val test4b_2 = longest_string3 ["A","bc","C"] = "bc";
val test4b_3 = longest_string3 ["A","bc","BC","d"] = "bc";

val test4c_0 = longest_string4 [] = "";
val test4c_1 = longest_string4 ["A"] = "A";
val test4c_2 = longest_string4 ["A","bc","C"] = "bc";
val test4c_3 = longest_string4 ["A","bc","BC","d"] = "BC";

val test5_0 = longest_capitalized [] = "";
val test5_1 = longest_capitalized ["aR"] = "";
val test5_2 = longest_capitalized ["Ar"] = "Ar";
val test5_3 = longest_capitalized ["A","BC","C"] = "BC";
val test5_4 = longest_capitalized ["A","Bc","CD","dEFG"] = "Bc";

val test6_0 = rev_string "" = "";
val test6_1 = rev_string "a" = "a";
val test6_2 = rev_string "Abc" = "cbA";

val test7_0 = (first_answer (fn x => if x > 3 then SOME x else NONE) [] = 4) handle NoAnswer => true;
val test7_1 = (first_answer (fn x => if x > 3 then SOME x else NONE) [1,2] = 4) handle NoAnswer => true;
val test7_2 = first_answer (fn x => if x > 3 then SOME x else NONE) [1,2,3,4,5] = 4;

val test8_0 = all_answers (fn x => if x = 0 then SOME [x] else NONE) [] = SOME [];
val test8_1 = all_answers (fn x => if x = 0 then SOME [x] else NONE) [1] = NONE;
val test8_2 = all_answers (fn x => if x = 0 then SOME [x] else NONE) [0] = SOME [0];

val test8_3 = all_answers (fn x => if x > 0 then SOME [x] else NONE) [1,2,3] = SOME [3,2,1];
val test8_4 = all_answers (fn x => if x > 0 then SOME [x] else NONE) [1,0,3] = NONE;

val test9a_0 = count_wildcards UnitP = 0;
val test9a_1 = count_wildcards Wildcard = 1;
val test9a_2 = count_wildcards (TupleP [UnitP]) = 0;
val test9a_3 = count_wildcards (TupleP [Wildcard]) = 1;
val test9a_4 = count_wildcards (TupleP [UnitP, Wildcard, ConstructorP("C", Wildcard)]) = 2;
val test9a_5 = count_wildcards (TupleP [Variable "int", Wildcard, UnitP]) = 1;

val test9b_0 = count_wild_and_variable_lengths (ConstructorP("C", UnitP)) = 0;
val test9b_1 = count_wild_and_variable_lengths (ConstructorP("C", Wildcard)) = 1;
val test9b_2 = count_wild_and_variable_lengths (Wildcard) = 1;
val test9b_3 = count_wild_and_variable_lengths (Variable("a")) = 1;
val test9b_4 = count_wild_and_variable_lengths (Variable("abc")) = 3;
val test9b_5 = count_wild_and_variable_lengths (TupleP [Wildcard,Variable("abc"),Variable("abc")]) = 7;

val test9c_0 = count_some_var ("x", UnitP) = 0;
val test9c_1 = count_some_var ("x", ConstructorP("x", Wildcard)) = 0;
val test9c_2 = count_some_var ("x", Variable("y")) = 0;
val test9c_3 = count_some_var ("x", Variable("x")) = 1;
val test9c_4 = count_some_var ("x", TupleP [Variable("x"), Variable("x")]) = 2;
val test9c_5 = count_some_var ("x", TupleP [ConstructorP("x", Variable("x")), Variable("y")]) = 1;

val test10_0 = check_pat (UnitP) = true;
val test10_1 = check_pat (Variable("x")) = true;
val test10_2 = check_pat (TupleP [Variable("x"), Variable("y")]) = true;
val test10_3 = check_pat (TupleP [Variable("x"), Variable("x")]) = false;
val test10_4 = check_pat (TupleP [Variable("x"), ConstructorP("x", Variable("y"))]) = true;
val test10_5 = check_pat (TupleP [Variable("x"), ConstructorP("x", Variable("x"))]) = false;

val test11_00 = match (Const(1), Wildcard) = (SOME []);
val test11_01 = match (Unit, Wildcard) = (SOME []);
val test11_02 = match (Tuple [Unit], Wildcard) = (SOME []);

val test11_03 = match (Const(1), Variable "x") = (SOME [("x", Const(1))]);
val test11_04 = match (Unit, Variable "x") = (SOME [("x", Unit)]);
val test11_05 = match (Tuple [Unit], Variable "x") = (SOME [("x", Tuple [Unit])]);

val test11_06 = match (Unit, UnitP) = (SOME []);
val test11_07 = match (Const 1, UnitP) = NONE;
val test11_08 = match (Tuple [Unit], UnitP) = NONE;

val test11_09 = match (Const 17, ConstP 17) = (SOME []);
val test11_10 = match (Const 15, ConstP 17) = NONE;
val test11_11 = match (Unit, ConstP 17) = NONE;

val test11_12 = match (Tuple [Const 1, Const 2], TupleP [Variable "x", Variable "y"]) 
  = (SOME [("y", Const 2), ("x", Const 1)]);
val test11_13 = match (Tuple [Const 1, Const 2], TupleP [Variable "x"]) = NONE;
val test11_14 = match (Tuple [Const 1], TupleP [Variable "x", Variable "y"]) = NONE;
val test11_15 = match (Tuple [Const 1, Const 2], TupleP [Variable "x", Variable "x"]) = NONE;
val test11_16 = match (Tuple [Const 1, Const 2], TupleP [Variable "x", ConstP 15]) = NONE;
val test11_17 = match (Tuple [Const 1, Const 2], TupleP [Variable "x", Wildcard]) 
  = (SOME [("x", Const 1)]);

val test11_18 = match (Constructor("C", Const 1), ConstructorP ("C", Variable "x")) 
  = (SOME [("x", Const 1)]);
val test11_19 = match (Constructor("C", Const 1), ConstructorP ("D", Variable "x")) = NONE;
val test11_20 = match (Constructor("C", Const 1), ConstructorP ("C", ConstP 5)) = NONE;

val test12_0 = first_match Unit [] = NONE;
val test12_1 = first_match Unit [ConstP 5] = NONE;
val test12_2 = first_match Unit [UnitP] = SOME [];
val test12_3 = first_match (Const 5) [Variable "x"] = SOME [("x", Const 5)];
val test12_4 = first_match (Const 5) [UnitP, Variable "x"] = SOME [("x", Const 5)];
val test12_5 = first_match (Const 5) [Wildcard, Variable "x"] = SOME [];