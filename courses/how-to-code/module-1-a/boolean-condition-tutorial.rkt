;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname Test) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)
;Boolean
(> 2 3)     ; 2 >  3 => #false
(< 2 3)     ; 2 <  3 => #true
(= 2 3)     ; 2 =  3 => #false
(>= 2 3)    ; 2 >= 3 => #false
(<= 2 3)    ; 2 <= 3 => #true
(not(= 2 3)); 2 != 3 => #true
;Boolean operator
(and true true false) ; true if all true, otherwise false
(or true false false) ; false if all false, otherwise true
(not true) ; invert logic
;simple if condition =>(if bool-expression true-expression false-expression)
(if false 2 3); Simple Example for if condition
;Example
(define IMG (rectangle 20 30 "solid" "red"))
(if (< (image-width IMG)(image-height IMG));bool-expression
    "the image is tall"                    ;true-expression
    "the image is wide")                   ;false-expression