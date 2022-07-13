(* Mahmoud Elnagar 07/06/2022 *)

(* DON'T USE: null, hd, tl, isSome, valOf, # *)
(*======================================= Problem 1 ===============================================*)
(* if you use this function to compare two strings (returns true if the same
   string), then you avoid several of the functions in problem 1 having
   polymorphic types that may be confusing *)
fun same_string(s1 : string, s2 : string) = s1 = s2; 
(*===================================== Function 1-A ==============================================*)
(* string * string list -> string list option
   Return - NONE if the string is not in the list.
          - SOME lst where lst is the argument list that doesn't contain the given string.
   Assume the string is in the list at most once.
*)

fun all_except_option (str, slist) =
   case slist of
      [] => NONE
   |  s::sl => 
         if same_string(s,str) 
         then SOME sl 
         else case all_except_option(str, sl) of
                  NONE => NONE
               |  SOME ls => SOME (s::ls)
;

(* IMO, this is a better style, using helpers makes the code more readable *)

(* fun all_except_option (str, slist) =
   let
      fun cons_opt (s, slo) = 
         case slo of NONE => NONE 
         | SOME sl => SOME (s::sl)
      ;
      fun filter [] = NONE
       |  filter (s::sl) = 
            if same_string(s,str) 
            then SOME sl 
            else cons_opt(s, filter(sl))
      ;
   in
      filter(slist) 
   end
; *)
(*===================================== Function 1-B ==============================================*)
(* string list list * string -> string list
   produces the union of all lists in subs that contains s, but without s itself in the result
 *)

 fun get_substitutions1 (subs, s) = 
   case subs of
      [] => []
   |  sl::sll => case all_except_option(s,sl) of
                     NONE => get_substitutions1(sll,s)
                  |  SOME slf => slf@get_substitutions1(sll,s)
 ;

(*===================================== Function 1-C ==============================================*)
(* string list list * string -> string list
   produces the union of all lists in subs that contains s, but without s itself in the result
   <tail-recursive Version>
 *)

 fun get_substitutions2 (subs, s) = 
   let
      fun get_list(sl,acc)= 
         case all_except_option(s,sl) of 
            NONE => acc 
         |  SOME slf => acc@slf
      ;
      fun get_subs(subs, acc)=
         case subs of
            [] => acc
         |  sl::sll => get_subs(sll, get_list(sl, acc))
   in
      get_subs(subs,[])
   end
 ;
(*===================================== Function 1-D ==============================================*)
(* type FullName = {first:string,middle:string,last:string};
   string list list * FullName -> FullName list
   produce a list of all possible names by substituting the first name only. The list should begin with the original name.
 *)

fun similar_names (subs, name) =
   let
      val {first=f_name, middle=m_name, last=l_name} = name;
      fun make_full_name(s)= {first = s,middle = m_name,last = l_name};
      fun get_name_list(sl)= 
         case sl of
            [] => []
         |  s::sl => make_full_name(s)::get_name_list(sl)
      ;
   in
      name::get_name_list(get_substitutions2(subs,f_name))
   end
;
(*======================================= Problem 2 ===============================================*)
(* you may assume that Num is always used with values 2, 3, ..., 10
   though it will not really come up *)
datatype suit = Clubs | Diamonds | Hearts | Spades
datatype rank = Jack | Queen | King | Ace | Num of int 
type card = suit * rank

datatype color = Red | Black
datatype move = Discard of card | Draw 

exception IllegalMove
(*===================================== Function 2-A ==============================================*)
(* card -> color
   returns Black if the card suit is Spades or Clubs, Red otherwise.
 *)
fun card_color (Clubs,_) = Black
 |  card_color (Spades,_) = Black
 |  card_color (Diamonds,_) = Red
 |  card_color (Hearts,_) = Red
;
(*===================================== Function 2-B ==============================================*)
(* card -> int
   return the value of a given card : Num i = i; Ace = 11; anything else = 10;
 *)
fun card_value (_, Num i) = i
 |  card_value (_, Ace) = 11
 |  card_value _ = 10
;
(*===================================== Function 2-C ==============================================*)
(* card list * card * exn -> card list
   remove the first occurrence of the card c from the card list cs
   If c is not in the list, raise the exception e
 *)
fun remove_card (cs,c,e) = 
   case cs of
      [] => raise e
   |  crd::cl => 
         if crd = c then cl 
         else crd::remove_card(cl,c,e)
;
(*===================================== Function 2-D ==============================================*)
(* card list -> bool
   return true if all cards have the same color, false otherwise
 *)
fun all_same_color cs =
   case cs of
      [] => true
   |  [_] => true
   |  c1::c2::cl => 
         if card_color(c1) = card_color(c2)
         then all_same_color(c2::cl)
         else false
;
(*===================================== Function 2-E ==============================================*)
(* card list -> int
   produce the sum of all card values. make it tail-recursive
 *)
fun sum_cards cs =
   let
      fun sum(cards, acc)=
         case cards of
            [] => acc
          | c::cl => sum(cl, card_value(c) + acc)
   in
      sum(cs, 0)
   end
;
(*===================================== Function 2-F ==============================================*)
(* card list * int -> int
   returns the score of the given list
      - let s = sum of all card values in the given list
      - if s > g then ps = 3*(s-g) else ps = g-s
      - if all cards have same color then score = ps/2 else score = ps
 *)
fun score (cs, g) =
   let
      val s = sum_cards(cs);
      val ps = if s > g then 3 * (s-g) else g-s;
   in
      if all_same_color(cs) then ps div 2 else ps
   end
;
(*===================================== Function 2-G ==============================================*)
(* card list * move list * int -> int
   return the score at the end of the game.
*)

fun officiate (cs, ms, goal) =
   let
      fun play(card_list, move_list, held_cards)=
         case (card_list, move_list) of
            (_, []) => score(held_cards, goal)
         |  (_, (Discard c)::ml) => play(card_list, ml, remove_card(held_cards, c, IllegalMove))
         |  ([], Draw::_) => score(held_cards, goal)
         |  (c::cl, Draw::ml) => 
               if sum_cards(c::held_cards) > goal 
               then score(c::held_cards, goal)
               else play(cl,ml,c::held_cards)
   in
     play(cs,ms,[])
   end
;
(*======================================= Problem 3 ===============================================*)
(* fun min(a,b) = if a < b then a else b; *)
(*==================================== Function 3-A-0 =============================================*)
(* card list -> card list list
   generate all possible card lists by replacing Ace with Num 1 or Num 11
*)
fun generate(cs) =  
   let
      fun cons_all (c, cll) = 
         case cll of
            [] => []
         |  cl::cll => (c::cl)::cons_all(c,cll)
      ;
      fun cons_ace(suit, cll) = 
         cons_all((suit, Num(1)), cll) @ cons_all((suit, Num(11)), cll)
      ;
   in
      case cs of 
         [] => [[]]
      |  (suit, Ace)::cl => cons_ace(suit, generate(cl))
      |  c::cl => cons_all(c, generate(cl))
   end
;
(*==================================== Function 3-A-1 =============================================*)
(* card list * int -> int
   returns the least possible score of the given list, if Ace value is 1 or 11.
*)
fun score_challenge (cs, g) =
   let
      fun get_score [] = score([],g)
       |  get_score [cl] = score(cl,g)
       |  get_score (cl::cll)= Int.min(score(cl,g), get_score(cll))
      ;
   in
      get_score(generate(cs))
   end
;
(*==================================== Function 3-A-2 =============================================*)
(* card list * move list * int -> int
   return the score at the end of the game.
*)

fun officiate_challenge (cs, ms, goal) =
   let
      fun min_sum [] = sum_cards([])
       |  min_sum [cl] = sum_cards(cl)
       |  min_sum (cl::cll) = Int.min(sum_cards(cl), min_sum(cll))
      ;
      fun play(card_list, move_list, held_cards)=
         case (card_list, move_list) of
            (_, []) => score_challenge(held_cards, goal)
         |  (_, (Discard c)::ml) => play(card_list, ml, remove_card(held_cards, c, IllegalMove))
         |  ([], Draw::_) => score_challenge(held_cards, goal)
         |  (c::cl, Draw::ml) => 
               if min_sum(generate(c::held_cards)) > goal 
               then score_challenge(c::held_cards, goal)
               else play(cl,ml,c::held_cards)
   in
     play(cs,ms,[])
   end
;
(*===================================== Function 3-B ==============================================*)
(* card list * int -> move list
   given a card-list and goal it should return move-list such that calling officiate has this behavior:
    - The value of the held cards never exceeds the goal.
    - draw when goal > (value + 10)
    - If a score of 0 is reached, there must be no more moves.
    - If it is possible to reach a score of 0 by discarding a card followed by drawing a card, then this
      must be done.
*)
fun careful_player(cs, goal) =
   let
      fun can_draw(hlist) = (goal - sum_cards(hlist)) > 10;
      fun is_zero(hlist) = score(hlist, goal) = 0;

      fun cheat(dhl, c, hlist) =
         case hlist of
            [] => []
         |  hc::hl => 
               if is_zero(dhl@(c::hl))
               then [Discard hc, Draw]
               else cheat(hc::dhl, c, hl)
      ;

      fun get_moves(clist, hlist)=
         if is_zero(hlist) then []
         else case (clist, can_draw(hlist)) of
               ([],false)    => []
            |  ([],true)     => [Draw]
            |  (c::cl,true)  => Draw::get_moves(cl,c::hlist)
            |  (c::cl,false) => cheat([],c, hlist)
      ;
   in
      get_moves(cs,[])
   end
;