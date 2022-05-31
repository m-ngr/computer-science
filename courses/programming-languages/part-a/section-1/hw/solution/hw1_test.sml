(* Mahmoud Elnagar 28/01/2022 *)


val test1_1 = is_older ((1,2,3),(2,3,4)) = true
val test1_2 = is_older ((1,2,3),(1,2,3)) = false
val test1_3 = is_older ((2,3,4),(1,2,3)) = false
val test1_4 = is_older ((1,2,3),(1,2,4)) = true
val test1_5 = is_older ((1,2,3),(1,2,2)) = false

val test2_1 = number_in_month ([(2012,2,28),(2013,12,1)],2) = 1
val test2_2 = number_in_month ([],2) = 0
val test2_3 = number_in_month ([(2012,2,28)],2) = 1
val test2_4 = number_in_month ([(2012,3,28)],2) = 0
val test2_5 = number_in_month ([(2012,3,28),(2013,12,1),(2014,3,28)],3) = 2

val test3_1 = number_in_months ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[2,3,4]) = 3
val test3_2 = number_in_months ([],[]) = 0
val test3_3 = number_in_months ([],[2,3,4]) = 0
val test3_4 = number_in_months ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[]) = 0
val test3_5 = number_in_months ([(2012,7,28),(2013,12,1),(2011,2,31),(2011,7,28)],[2,11,7]) = 3

val test4_1 = dates_in_month ([(2012,2,28),(2013,12,1)],2) = [(2012,2,28)]
val test4_2 = dates_in_month ([],2) = []
val test4_3 = dates_in_month ([(2012,2,28),(2013,12,1)], 4) = []
val test4_4 = dates_in_month ([(2012,2,28),(2013,12,1)],12) = [(2013,12,1)]
val test4_5 = dates_in_month ([(2012,2,28),(2013,2,1)] , 2) = [(2012,2,28),(2013,2,1)]

val test5_1 = dates_in_months ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[2,3,4]) = [(2012,2,28),(2011,3,31),(2011,4,28)]
val test5_2 = dates_in_months ([],[]) = []
val test5_3 = dates_in_months ([],[2,3,4]) = []
val test5_4 = dates_in_months ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[]) = []
val test5_5 = dates_in_months ([(2012,11,28),(2013,12,1),(2011,2,31),(2011,2,28)],[3,2,4]) = [(2011,2,31),(2011,2,28)]

val test6_1 = get_nth (["hi", "there", "how", "are", "you"], 2) = "there"
val test6_2 = get_nth (["hi", "there", "how", "are", "you"], 5) = "you"

val test7 = date_to_string (2013, 6, 1) = "June 1, 2013"

val test8_1 = number_before_reaching_sum (10, [1,2,3,4,5]) = 3
val test8_2 = number_before_reaching_sum ( 9, [1,2,3,4,5]) = 3
val test8_3 = number_before_reaching_sum (11, [1,2,3,4,5]) = 4
val test8_4 = number_before_reaching_sum ( 1, [1,2,3,4,5]) = 0

val test9_1 = what_month 70 = 3
val test9_2 = what_month  1 = 1
val test9_3 = what_month 365 = 12
val test9_4 = what_month 33 = 2

val test10_1 = month_range (31, 34) = [1,2,2,2]
val test10_2 = month_range (31, 31) = [1]
val test10_3 = month_range (31, 32) = [1,2]
val test10_4 = month_range (31, 30) = []

val test11_1 = oldest([(2012,2,28),(2011,3,31),(2011,4,28)]) = SOME (2011,3,31)
val test11_2 = oldest([]) = NONE
val test11_3 = oldest([(2012,2,28)]) = SOME (2012,2,28)

val test12_0_1 = remove_duplicate_numbers([]) = [] 
val test12_0_2 = remove_duplicate_numbers([1,2]) = [1,2] 
val test12_0_3 = remove_duplicate_numbers([1,1]) = [1] 
val test12_0_4 = remove_duplicate_numbers([1,2,3,4]) = [1,2,3,4] 
val test12_0_5 = remove_duplicate_numbers([1,2,1,2,3,3,4,1,2,3,4]) = [1,2,3,4] 

val test12_1_1 = number_in_months_challenge ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[2,3,4,2,3,4]) = 3
val test12_1_2 = number_in_months_challenge ([],[]) = 0
val test12_1_3 = number_in_months_challenge ([],[2,3,4]) = 0
val test12_1_4 = number_in_months_challenge ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[]) = 0
val test12_1_5 = number_in_months_challenge ([(2012,7,28),(2013,12,1),(2011,2,31),(2011,7,28)],[2,11,7,2,11,7]) = 3

val test12_2_1 = dates_in_months_challenge ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[2,3,4,2,3]) = [(2012,2,28),(2011,3,31),(2011,4,28)]
val test12_2_2 = dates_in_months_challenge ([],[]) = []
val test12_2_3 = dates_in_months_challenge ([],[2,3,4]) = []
val test12_2_4 = dates_in_months_challenge ([(2012,2,28),(2013,12,1),(2011,3,31),(2011,4,28)],[]) = []
val test12_2_5 = dates_in_months_challenge ([(2012,11,28),(2013,12,1),(2011,2,31),(2011,2,28)],[3,2,4,2,2,3]) = [(2011,2,31),(2011,2,28)]

val test13_1 = reasonable_date((1,1,1)) = true
val test13_2 = reasonable_date((0,1,1)) = false
val test13_3 = reasonable_date((1,0,1)) = false
val test13_4 = reasonable_date((1,13,1)) = false
val test13_5 = reasonable_date((1,1,0)) = false
val test13_6 = reasonable_date((1,1,32)) = false
val test13_7 = reasonable_date((2000,2,29)) = true
val test13_8 = reasonable_date((2002,2,29)) = false
val test13_9 = reasonable_date((1600,2,29)) = true