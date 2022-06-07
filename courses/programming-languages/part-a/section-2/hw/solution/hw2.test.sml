(* Mahmoud Elnagar 07/06/2022 *)

use "hw2.sml";

val test1_a_1 = all_except_option ("string", []) = NONE;
val test1_a_2 = all_except_option ("string", ["a"]) = NONE;
val test1_a_3 = all_except_option ("string", ["string"]) = SOME [];
val test1_a_4 = all_except_option ("string", ["a","b"]) = NONE;
val test1_a_5 = all_except_option ("string", ["a","string"]) = SOME ["a"];
val test1_a_6 = all_except_option ("string", ["a","string","b"]) = SOME ["a","b"];

val test1_b_1 = get_substitutions1 ([["foo"],["there"]], "fox") = [];
val test1_b_2 = get_substitutions1 ([["foo"],["there"]], "foo") = [];
val test1_b_3 = get_substitutions1 ([["foo", "f"],["bar"]], "foo") = ["f"];
val test1_b_4 = get_substitutions1 ([["Fred","Fredrick"],["Elizabeth","Betty"],["Freddie","Fred","F"]],
                                    "Fred") = ["Fredrick","Freddie","F"];
val test1_b_5 = get_substitutions1([["Fred","Fredrick"],["Jeff","Jeffrey"],["Geoff","Jeff","Jeffrey"]],
                                    "Jeff") = ["Jeffrey","Geoff","Jeffrey"];

val test1_c_1 = get_substitutions2 ([["foo"],["there"]], "fox") = [];
val test1_c_2 = get_substitutions2 ([["foo"],["there"]], "foo") = [];
val test1_c_3 = get_substitutions2 ([["foo", "f"],["bar"]], "foo") = ["f"];
val test1_c_4 = get_substitutions2 ([["Fred","Fredrick"],["Elizabeth","Betty"],["Freddie","Fred","F"]],
                                    "Fred") = ["Fredrick","Freddie","F"];
val test1_c_5 = get_substitutions2([["Fred","Fredrick"],["Jeff","Jeffrey"],["Geoff","Jeff","Jeffrey"]],
                                    "Jeff") = ["Jeffrey","Geoff","Jeffrey"];

val test_name = {first="Mahmoud", middle="A", last="Elnagar"};
val test1_d_1 = similar_names ([], test_name) = [test_name];
val test1_d_2 = similar_names ([["Mahmoud"],["Elnagar","ngr"],["Mahmoud"]], test_name) = [test_name];

val test1_d_3 = similar_names ([["Fred","Fredrick"],["Elizabeth","Betty"],["Freddie","Fred","F"]], test_name) = [test_name];

val test1_d_4 = similar_names ([["Mahmoud","Mah"],["Elnagar","ngr"],["Mahmoud","Mhmod","Mah"]], test_name) = [test_name, {first="Mah", middle="A", last="Elnagar"},{first="Mhmod", middle="A", last="Elnagar"},{first="Mah", middle="A", last="Elnagar"}];

val test1_d_5 = similar_names ([["Fred","Fredrick"],["Elizabeth","Betty"],["Freddie","Fred","F"]], {first="Fred", middle="W", last="Smith"}) =
	    [{first="Fred", last="Smith", middle="W"}, {first="Fredrick", last="Smith", middle="W"},
	     {first="Freddie", last="Smith", middle="W"}, {first="F", last="Smith", middle="W"}];

val test2_a_1 = card_color (Clubs, Num 2) = Black;
val test2_a_2 = card_color (Spades, Num 2) = Black;
val test2_a_3 = card_color (Diamonds, Num 2) = Red;
val test2_a_4 = card_color (Hearts, Num 2) = Red;

val test2_b_1 = card_value (Clubs, Num 2) = 2;
val test2_b_2 = card_value (Clubs, Ace) = 11;
val test2_b_3 = card_value (Hearts, King) = 10;
val test2_b_4 = card_value (Diamonds, Queen) = 10;
val test2_b_5 = card_value (Clubs, Jack) = 10;

val test2_c_1 = (remove_card ([], (Hearts, Ace), IllegalMove)=[]) 
  handle IllegalMove => true;
val test2_c_2 = (remove_card ([(Hearts, Ace)], (Hearts, King), IllegalMove)=[]) 
  handle IllegalMove => true;
val test2_c_3 = remove_card ([(Hearts, Ace)], (Hearts, Ace), IllegalMove) = [];
val test2_c_4 = remove_card ([(Hearts, Ace),(Hearts, Ace)], (Hearts, Ace), IllegalMove) = [(Hearts, Ace)];
val test2_c_5 = remove_card ([(Hearts, Ace),(Hearts, King),(Hearts, Ace)], (Hearts, Ace), IllegalMove) 
  = [(Hearts, King),(Hearts, Ace)];

val test2_d_1 = all_same_color [] = true;
val test2_d_2 = all_same_color [(Hearts, Ace)] = true;
val test2_d_3 = all_same_color [(Hearts, Ace), (Hearts, Ace)] = true;
val test2_d_4 = all_same_color [(Hearts, Ace), (Clubs, Ace)] = false;

val test2_e_1 = sum_cards [] = 0;
val test2_e_2 = sum_cards [(Clubs, Num 2)] = 2;
val test2_e_3 = sum_cards [(Clubs, Num 2),(Clubs, Num 2)] = 4;
val test2_e_4 = sum_cards [(Clubs, King),(Clubs, Num 2)] = 12;

val test2_f_1 = score ([],10) = 5;
val test2_f_2 = score ([(Hearts, Num 2)],10) = 4;
val test2_f_3 = score ([(Hearts, Num 2),(Clubs, Num 4)],10) = 4;
val test2_f_4 = score ([(Hearts, Num 6),(Clubs, Num 4)],10) = 0;
val test2_f_5 = score ([(Hearts, Num 6),(Clubs, Num 6)],10) = 6;
val test2_f_6 = score ([(Hearts, Num 6),(Hearts, Num 6)],10) = 3;

val test2_g_1 = officiate ([(Hearts, Num 2),(Clubs, Num 4)],[Draw], 15) = 6;

val test2_g_2 = officiate ([(Clubs,Ace),(Spades,Ace),(Clubs,Ace),(Spades,Ace)],
                        [Draw,Draw,Draw,Draw,Draw],
                        42)
             = 3;

val test2_g_3 = ((officiate([(Clubs,Jack),(Spades,Num(8))],
                         [Draw,Discard(Hearts,Jack)],
                         42);
               false) 
              handle IllegalMove => true)

val ace = (Hearts, Ace);
val ace1 = (Hearts, Num 1);
val ace11 = (Hearts, Num 11);
val c2 = (Hearts, Num 2);
val c3 = (Hearts, Num 3);

val test3_a0_0 = generate([]) = [[]]; 
val test3_a0_1 = generate([c2]) = [[c2]]; 
val test3_a0_2 = generate([ace]) = [[ace1],[ace11]]; 
val test3_a0_3 = generate([c2,ace,c3]) = [[c2,ace1,c3],[c2,ace11,c3]]; 
val test3_a0_4 = generate([ace,ace]) = [[ace1,ace1],[ace1,ace11],[ace11,ace1],[ace11,ace11]]; 

val test3_a1_0 = score_challenge ([],10) = 5;
val test3_a1_1 = score_challenge ([(Hearts, Num 2)],10) = 4;
val test3_a1_2 = score_challenge ([(Hearts, Num 2),(Clubs, Num 4)],10) = 4;
val test3_a1_3 = score_challenge ([(Hearts, Num 6),(Clubs, Num 4)],10) = 0;
val test3_a1_4 = score_challenge ([(Hearts, Num 6),(Clubs, Num 6)],10) = 6;
val test3_a1_5 = score_challenge ([(Clubs, Ace),(Hearts, Num 5)],10) = 4;
val test3_a1_6 = score_challenge ([(Hearts, Ace),(Hearts, Num 5)],10) = 2;
val test3_a1_7 = score_challenge ([(Clubs, Ace),(Hearts, Num 2)],11) = 6;
val test3_a1_8 = score_challenge ([(Hearts, Ace),(Hearts, Num 2)],10) = 3;
val test3_a1_9 = score_challenge ([(Hearts, Ace),(Clubs, Ace)],10) = 6; 

val test3_a2_1 = officiate_challenge ([(Hearts, Num 2),(Clubs, Num 4)],[Draw], 15) = 6;

val test3_a2_2 = officiate_challenge ([(Clubs,Num 10),(Spades,Num 10),(Clubs,Num 10),(Spades,Ace)],
                        [Draw,Draw,Draw,Draw,Draw],
                        35)
             = 2;

val test3_a2_3 = officiate_challenge ([(Clubs,Num 10),(Spades,Num 10),(Clubs,Num 10),(Spades,Ace)],
                        [Draw,Draw,Draw,Draw,Draw],
                        29)
             = 1;

val test3_a2_4 = ((officiate_challenge([(Clubs,Jack),(Spades,Num(8))],
                         [Draw,Discard(Hearts,Jack)],
                         42);
               false) 
              handle IllegalMove => true); 

val test3_b_1 = careful_player([],0) = [];
val test3_b_2 = careful_player([],1) = [];
val test3_b_3 = careful_player([],11) = [Draw];
val test3_b_4 = careful_player([(Clubs, Ace)],11) = [Draw];
val test3_b_5 = careful_player([(Clubs, Num 10), (Clubs, Ace)], 13) 
  = [Draw];
val test3_b_6 = careful_player([(Clubs, Num 9), (Clubs, King)], 11) 
  = [Draw,Discard (Clubs,Num 9),Draw];
val test3_b_7 = careful_player([(Clubs, Ace), (Clubs, Ace)], 11) = [Draw];