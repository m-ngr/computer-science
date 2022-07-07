(* Mahmoud Elnagar - 07/07/2022 *)
use "hw7.sml";

val real_equal = real_close;
(* ============================= My Tests: Preprocess ========================================= *)
fun geomEq (g1,g2) =
	case (g1,g2) of
	 	(NoPoints, NoPoints) => true
	|	(LineSegment(a,b,c,d), LineSegment(w,x,y,z)) => 
			real_equal(a,w) andalso real_equal(b,x) 
			andalso real_equal(c,y) andalso real_equal(d,z)
	| (Point(a,b), Point(c,d)) => real_equal(a,c) andalso real_equal(b,d)
	| (Line(a,b), Line(c,d)) => real_equal(a,c) andalso real_equal(b,d)
	| (VerticalLine a, VerticalLine b) => real_equal(a,b)
	| (Intersect(a,b), Intersect(c,d)) => geomEq(a,c) andalso geomEq(b,d)
	| (Let(s1,g1,h1), Let(s2,g2,h2)) => s1=s2 andalso geomEq(g1,g2) andalso geomEq(h1,h2)
	| (Var s1, Var s2) => s1=s2
	| (Shift (a1,b1,g1), Shift (a2,b2,g2)) => 
			real_equal(a1,a2) andalso real_equal(a1,a2) andalso geomEq(g1,g2)
	| _ => false
;

val test1_0 = geomEq(preprocess_prog(NoPoints), NoPoints);
val test1_1 = geomEq(preprocess_prog(LineSegment(3.2,4.1,3.2,4.1)), Point(3.2,4.1));
val test1_2 = geomEq(preprocess_prog(LineSegment(2.1,4.1,3.2,4.1)), LineSegment(2.1,4.1,3.2,4.1));
val test1_3 = geomEq(preprocess_prog(LineSegment(2.1,4.1,2.1,5.1)), LineSegment(2.1,4.1,2.1,5.1));
val test1_4 = geomEq(preprocess_prog(LineSegment(5.1,4.1,3.2,8.1)), LineSegment(3.2,8.1,5.1,4.1));
val test1_5 = geomEq(preprocess_prog(LineSegment(2.1,8.1,2.1,5.1)), LineSegment(2.1,5.1,2.1,8.1));
val test1_5 = geomEq(preprocess_prog(LineSegment(1.00000999,1.0,1.0,2.0)), 
																		 LineSegment(1.00000999,1.0,1.0,2.0));

val test1_6 = geomEq(preprocess_prog(Point(3.2,4.5)), Point(3.2,4.5));
val test1_7 = geomEq(preprocess_prog(Line(3.2,4.5)), Line(3.2,4.5));
val test1_8 = geomEq(preprocess_prog(VerticalLine(3.2)), VerticalLine(3.2));
val test1_9 = geomEq(preprocess_prog(
	Intersect(LineSegment(3.2,4.1,3.2,4.1), LineSegment(5.1,4.1,3.2,8.1))), 
	Intersect(Point(3.2,4.1), LineSegment(3.2,8.1,5.1,4.1)))
;
val test1_10 = geomEq(preprocess_prog(
	Let("x",LineSegment(3.2,4.1,3.2,4.1), LineSegment(5.1,4.1,3.2,8.1))), 
	Let("x",Point(3.2,4.1), LineSegment(3.2,8.1,5.1,4.1)))
;
val test1_11 = geomEq(preprocess_prog(Var("x")), Var("x"));
val test1_12 = geomEq(preprocess_prog(
	Shift(1.1, 2.2, LineSegment(3.2,4.1,3.2,4.1))), 
	Shift(1.1, 2.2, Point(3.2,4.1)))
;

val test2_0 = geomEq(eval_prog(Shift(1.1,2.2, NoPoints),[]), NoPoints);
val test2_1 = geomEq(eval_prog(Shift(1.1,2.2, Point(3.2,4.1)),[]), Point(4.3, 6.3));
val test2_2 = geomEq(eval_prog(Shift(1.1,2.2, Line(3.2,4.1)),[]), Line(3.2, 2.78));
val test2_3 = geomEq(eval_prog(Shift(1.1,2.2, VerticalLine(3.2)),[]), VerticalLine(4.3));
val test2_4 = geomEq(eval_prog(Shift(1.1,2.2, LineSegment(1.1,2.2,3.3,4.4)),[]), 
	LineSegment(2.2,4.4,4.4,6.6));
(* =================================== Provided Tests ======================================== *)
(* Preprocess tests *)
let
	val Point(a,b) = preprocess_prog(LineSegment(3.2,4.1,3.2,4.1))
	val Point(c,d) = Point(3.2,4.1)
in
	if real_equal(a,c) andalso real_equal(b,d)
	then (print "preprocess converts a LineSegment to a Point successfully\n")
	else (print "preprocess does not convert a LineSegment to a Point succesfully\n")
end;

let 
	val LineSegment(a,b,c,d) = preprocess_prog (LineSegment(3.2,4.1,~3.2,~4.1))
	val LineSegment(e,f,g,h) = LineSegment(~3.2,~4.1,3.2,4.1)
in
	if real_equal(a,e) andalso real_equal(b,f) andalso real_equal(c,g) andalso real_equal(d,h)
	then (print "preprocess flips an improper LineSegment successfully\n")
	else (print "preprocess does not flip an improper LineSegment successfully\n")
end;

(* eval_prog tests with Shift*)
let 
	val Point(a,b) = (eval_prog (preprocess_prog (Shift(3.0, 4.0, Point(4.0,4.0))), []))
	val Point(c,d) = Point(7.0,8.0) 
in
	if real_equal(a,c) andalso real_equal(b,d)
	then (print "eval_prog with empty environment worked\n")
	else (print "eval_prog with empty environment is not working properly\n")
end;

(* Using a Var *)
let 
	val Point(a,b) = (eval_prog (Shift(3.0,4.0,Var "a"), [("a",Point(4.0,4.0))]))
	val Point(c,d) = Point(7.0,8.0) 
in
	if real_equal(a,c) andalso real_equal(b,d)
	then (print "eval_prog with 'a' in environment is working properly\n")
	else (print "eval_prog with 'a' in environment is not working properly\n")
end;


(* With Variable Shadowing *)
let 
	val Point(a,b) = (eval_prog (Shift(3.0,4.0,Var "a"), [("a",Point(4.0,4.0)),("a",Point(1.0,1.0))]))
	val Point(c,d) = Point(7.0,8.0) 
in
	if real_equal(a,c) andalso real_equal(b,d)
	then (print "eval_prog with shadowing 'a' in environment is working properly\n")
	else (print "eval_prog with shadowing 'a' in environment is not working properly\n")
end;