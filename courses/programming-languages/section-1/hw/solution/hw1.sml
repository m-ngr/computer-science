(* Mahmoud Elnagar first-written at 28/01/2022, revised at 31/05/2022 *)

(* This file contains some notes and alternative (more efficient) implementations in the comments 
    To make it easy for the reviewer, feel free to skip the comments and just read the code,
    which is implemented using only what we learned in week 1.

    The comments contain:
        - tail-recursive implementation of some functions
        - the signature, the purpose of each function
        - hints about other implementations using abstract functions
            but no implementation was provided since we didn't learn it yet
*)

(*===================================== Data Definition =============================================*)

(*  Note:   We didn't learn how to make custom data structure in SML yet, 
            but I will leave the data definition comment below. *)

(*  Date is (int * int * int)
    interp. a date in the form (year, month, day).
        - year > 0
        - month = [1, 12]
        - day = [1,31] ; (or less depending on the month)

    Example: val today = (2022,1,28);
    Template:
        fun fun_for_date(date: int * int * int) =
            ... (#1 date) (*year*)
                (#2 date) (*month*)
                (#3 date) (*day*)
 *)

(* this definition will be used in more than a function*)
 val days_in_month = [31,28,31,30,31,30,31,31,30,31,30,31];
(* represents max days of month in a common year *)

(*======================================= Function 1 ===============================================*)

(*  (int * int * int) * (int * int * int) -> bool
    Produces true if the date1 comes before the date2 (date1 < date2), false otherwise.
*)
fun is_older(date1: int * int * int , date2: int * int * int) =
    (#1 date1) < (#1 date2) orelse 
    (#1 date1) = (#1 date2) andalso (#2 date1) < (#2 date2) orelse 
    (#1 date1) = (#1 date2) andalso (#2 date1) = (#2 date2) andalso (#3 date1) < (#3 date2) 
;
(*======================================= Function 2 ===============================================*)

(*  (int * int * int) list * int -> int
    Produces the number of dates in the list that are in the given month.
*)
fun number_in_month(dates: (int * int * int) list , month: int) = 
    if null dates
    then 0
    else 
        if (#2 (hd dates) = month)
        then 1 + number_in_month(tl dates, month)
        else number_in_month(tl dates, month)
; 
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator

fun number_in_month(dates: (int * int * int) list , month: int) = (* tail-recursive version *)
    let 
        fun number_in_month(dates: (int * int * int) list , rsf: int) = 
        if null dates
        then rsf
        else 
            if (#2 (hd dates) = month)
            then number_in_month(tl dates, rsf+1)
            else number_in_month(tl dates, rsf)
    in
        number_in_month(dates, 0)
    end
; *)

(* This function could be implemented using an abstract folding function, but we didn't learn about it yet in SML *)

(*======================================= Function 3 ===============================================*)

(*  (int * int * int) list * int list -> int
    Produces the number of dates in the dates list that are in any month of given month list.
*)
fun number_in_months(dates: (int * int * int) list , months: int list) = 
    if null months orelse null dates
    then 0
    else number_in_month (dates, hd months) + 
         number_in_months(dates, tl months)  
;
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator

fun number_in_months(dates: (int * int * int) list , months: int list) = (* tail-recursive version *)
    if null dates
    then 0
    else
        let
            fun number_in_months(months: int list, rsf: int) = 
                if null months
                then rsf
                else number_in_months(tl months, rsf + number_in_month (dates, hd months))  
        in
            number_in_months(months, 0)
        end 
;*)

(* This function could be implemented using an abstract folding function, but we didn't learn about it yet in SML *)

(*======================================= Function 4 ===============================================*)

(*  (int * int * int) list * int -> (int * int * int) list
    Produces a list of dates from the given list of dates that are in the given month
*)
fun dates_in_month(dates: (int * int * int) list , month: int) = 
    if null dates
    then []
    else
        if #2 (hd dates) = month
        then (hd dates)::dates_in_month(tl dates, month)
        else dates_in_month(tl dates, month)
;
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator 

fun dates_in_month(dates: (int * int * int) list , month: int) = (* tail-recursive version *)
    let
        fun dates_in_month(dates: (int * int * int) list , rsf: (int * int * int) list) = 
            if null dates
            then rsf
            else
                if #2 (hd dates) = month
                then dates_in_month(tl dates, rsf@[(hd dates)])
                else dates_in_month(tl dates, rsf)
    in
        dates_in_month(dates, [])
    end
;*)

(* This function could be implemented using a filtering function, but we didn't learn about it yet in SML *)
(* This function could be implemented using an abstract folding function, but we didn't learn about it yet in SML *)

(*======================================= Function 5 ===============================================*)

(*  (int * int * int) list * int list -> (int * int * int) list
    Produces a list of dates from the given list of dates that are in any month of given month list.
*)
fun dates_in_months(dates: (int * int * int) list , months: int list) = 
    if null months orelse null dates
    then []
    else dates_in_month (dates, hd months) @ 
         dates_in_months(dates, tl months) 
;
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator 

fun dates_in_months(dates: (int * int * int) list , months: int list) = (* tail-recursive version *)
    if null dates
    then []
    else
        let
            fun dates_in_months(months: int list, rsf: (int * int * int) list) = 
                if null months
                then rsf
                else dates_in_months(tl months, rsf @ dates_in_month (dates, hd months))  
        in
            dates_in_months(months, [])
        end 
;*)

(* This function could be implemented using an abstract folding function, but we didn't learn about it yet in SML *)

(*======================================= Function 6 ===============================================*)

(*  string list * int -> string
    Produces nth string in the given list. Assume list starts at 1 and n <= length of list
*)
fun get_nth(strings: string list , n: int) = 
    if n = 1
    then hd strings
    else get_nth(tl strings, n-1)
; 
(*======================================= Function 7 ===============================================*)

(*  (int * int * int) -> string
    Produces a string of the given date in the form {January 20, 2013}
*)

fun date_to_string(date: int * int * int) =
    let
        val month_string = ["January", "February", "March", "April",
                      "May", "June", "July", "August", "September",
                      "October", "November", "December"];
    in
        get_nth(month_string, #2 date) ^ " " ^
        Int.toString(#3 date) ^ ", " ^
        Int.toString(#1 date)
    end
;
(*======================================= Function 8 ===============================================*)

(*  int * int list-> int
    Produces a number n where the first n elements of the list add to less than sum, but the first
    n + 1 elements of the list add to sum or more. Assume all numbers are positive.
*)
fun number_before_reaching_sum(sum: int, numbers: int list) =
    if (sum - hd numbers) <= 0
    then 0
    else 1 + number_before_reaching_sum(sum - hd numbers, tl numbers)
;
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator 

fun number_before_reaching_sum(sum: int, numbers: int list) = (* tail-recursive version *)
    let
        fun number_before_reaching_sum(sum: int, numbers: int list, rsf: int) =
            if (sum - hd numbers) <= 0
            then rsf
            else number_before_reaching_sum(sum - hd numbers, tl numbers, rsf + 1)
    in
        number_before_reaching_sum(sum, numbers, 0)
    end
;*)
(*======================================= Function 9 ===============================================*)

(*  int -> int
    Produces the number of the month the give day is in, where day = [1, 365]
*)

(*From Data Definition: val days_in_month = [31,28,31,30,31,30,31,31,30,31,30,31]; *)

fun what_month(day: int) =
    1 + number_before_reaching_sum(day, days_in_month)
;
(*======================================= Function 10 ===============================================*)

(*  int * int -> int list
    Produces a list of month number for all days in [day1, day2]
*)
fun month_range(day1: int, day2: int) =
    if day1 > day2
    then []
    else what_month(day1) :: month_range(day1 + 1 , day2)
;
(* Note: This function could be made tail-recursive by adding rsf (result-so-far) accumulator 

fun month_range(day1: int, day2: int) = (* tail-recursive version *)
    let
        fun month_range(day2: int, rsf: int list) =
            if day1 > day2
            then rsf
            else month_range(day2 - 1, what_month(day2)::rsf )
    in
        month_range(day2, [])
    end
;*)
(*======================================= Function 11 ===============================================*)

(*  (int * int * int) list -> (int*int*int) option
    Produces NONE if the list has no dates and SOME d if the date d is the oldest date in the list.
*)
fun oldest(dates: (int * int * int) list) =
    if null dates
    then NONE
    else
        let fun older(old_date: (int * int * int), dates: (int * int * int) list) = 
            if null dates
            then old_date
            else
                if is_older(old_date, hd dates)
                then older(old_date, tl dates)
                else older(hd dates, tl dates)
        in
            SOME (older(hd dates, tl dates))
        end 
;
(*======================================= Function 12 ===============================================*)

(*  int list -> int list
    Produces the given list with no duplicates.
*)
fun remove_duplicate_numbers(numbers: int list) =
    if null numbers
    then []
    else
        let 
            fun is_duplicated(num: int, lon: int list) =
                if null lon
                then false
                else 
                    if (hd lon) = num
                    then true
                    else is_duplicated(num, tl lon)

            fun remove_duplicates(result: int list, numbers: int list) =
                if null numbers
                then result
                else
                    if is_duplicated(hd numbers, result)
                    then remove_duplicates(result, tl numbers)
                    else remove_duplicates(result@[hd numbers], tl numbers)
        in
            remove_duplicates([hd numbers], tl numbers)
        end
;
(*--------------------------------------------------------------*)
(*  (int * int * int) list * int list -> int
    Produces the number of dates in the dates list that are in any month of given month list.
    Challenge: the list of months may have duplicates
*)
fun number_in_months_challenge(dates: (int * int * int) list , months: int list) = 
    number_in_months(dates, remove_duplicate_numbers(months))
;
(*--------------------------------------------------------------*)
(*  (int * int * int) list * int list -> (int * int * int) list
    Produces a list of dates from the given list of dates that are in any month of given month list.
    Challenge: the list of months may have duplicates
*)
fun dates_in_months_challenge(dates: (int * int * int) list , months: int list) = 
    dates_in_months(dates, remove_duplicate_numbers(months))
;
(*======================================= Function 13 ===============================================*)

(*  (int * int * int) -> bool
    Produces true if the given date describes a real date in the common era. real date has:
        - year > 0
        - month = [1, 12]
        - day = [1, 31]; consider the leap year
*)

fun reasonable_date(date: int * int * int) =
    let
        fun get_nth(numbers: int list , n: int) = 
            if n = 1
            then hd numbers
            else get_nth(tl numbers, n-1)

        fun is_leap(year: int)=
            if (year mod 4) <> 0 then false
            else if (year mod 400) = 0 then true
            else if (year mod 100) = 0 then false
            else true

        fun max_days(year: int, month: int) =
            if month <> 2
            then get_nth(days_in_month, month)
            else 
                if is_leap(year)
                then 29
                else 28
    in
        (#1 date) >  0 andalso 
        (#2 date) >= 1 andalso (#2 date) <= 12 andalso 
        (#3 date) >= 1 andalso (#3 date) <= max_days((#1 date), (#2 date))
    end
;