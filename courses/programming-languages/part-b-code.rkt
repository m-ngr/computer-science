#lang racket
(provide (all-defined-out))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                             Racket Basics                              ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;variable definition
(define CONST 5)
;function definiton
(define (cube1 x) (* x x x))
;bound variable to anonymous function
(define cube2 (lambda (x) (* x x x)))
; recursive function pow
(define (pow1 x y)
  (if (= y 0) 1 (* x (pow1 x (- y 1)))))
; curried recursive function pow
(define pow2
  (lambda (x)
    (lambda (y)
      (if (= y 0) 1 (* x (pow2 x (- y 1)))))))
; make an empty list
(define emptyList1 empty)
(define emptyList2 null)
(define emptyList3 '())
; make a list
(define list0 '(1 2 3))
(define list1 (list 1 2 3))
(define list2 (cons 0 (list 1 2 3)))
; checking if the list is empty
(empty? emptyList1)
(null? emptyList1)
; first of the list
(first list1)
(car list1)
; rest of the list
(rest list1)
(cdr list1)
; raise exceptions
(if true "OK" (error "Error Message"))
; let bindings are not available for the previous or next bindings
; let bindings are available only in let body
; the current binding is evaluated in the environment before let-expression
(let ([v1 1]
      [v2 2]
      [v3 3])
  (+ v1 v2 v3))

; let bindings are not available for the previous bindings
; let bindings are available only in the next bindings and let body
; the current binding is evaluated in the environment before let-expression
; extended by the previous bindings
(let* ([v1 1]
       [v2 (+ v1 1)]
       [v3 3])
  (+ v1 v2 v3))

; let bindings are available for previous and next bindings and let body
; NOTE: you can't use the next bindings until they are initialized but you can
;       reference them in a function to be called after they are initialized
; the current binding is evaluated in the environment before let-expression
; extended by the previous bindings and a reference to next bindings
(letrec ([v1 1]
         [v2 (lambda()(+ v1 v3))]
         [v3 3])
  (+ v1 (v2) v3))
; local defines has the same semantics as letrec
(define (fun)
  (define v1 1)
  (define (v2) (+ v1 v3))
  (define v3 3)
  (+ v1 (v2) v3))
;Mutation
(define x 5); x = 5
(set! x 10) ; x = 10

; proper list: a deep nested pair that ends with empty list
; improper list: a deep nested pair that doesn't end with empty list

(define p1 (cons 5 "hi")) ; pair (5,"hi")
(define p2 (cons 5 (cons "hi" #t))) ; pair ( 5,("hi",#t) )
(define p3 (cons (cons 4 5) (cons "hi" #t))) ; pair ( (4,5),("hi",#t) )

(define l1(cons 5 (cons "hi" null))) ; pair( 5,("hi",null) ) is also a list

; list? returns true for proper lists, including the empty list
; pair? return true for anything built with cons, but false for empty list
(pair? p1) ;true
(list? p1) ;false
(list? l1) ;true
(pair? l1) ;true
(length l1) ; will work; work only with proper lists
;(length p1) ; will fail
; Mutable pairs: are improper lists
(define mpr (mcons 1 "hi"))
(mcar mpr) ; get first element in the mpair
(mcdr mpr) ; get second element in the mpair
(mpair? mpr) ; is it mutable pair
(set-mcar! mpr "M-1") ; mutate the value of the first element
(set-mcdr! mpr "M-2") ; mutate the value of the second element

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                          Lazy Evaluation and Thunks                      ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; some heavy computation
(define (slow-add x y)
  (letrec ([slow-id (lambda (y z)
                      (if (= 0 z)
                          y
                          (slow-id y (- z 1))))])
    (+ (slow-id x 50000000) y)))
; Lazy evaluation using higher-order-functions
(define (lazy-thunk thunk)
  (let ([isComputed false]
        [result false])
    (lambda ()
      (if isComputed
          result
          (begin (set! result (thunk))
                 (set! isComputed true)
                 result)))))

(define lazy-add (lazy-thunk (lambda () (slow-add 1 2))))

(lazy-add) ;1st computation, takes time
(lazy-add) ;2nd computation, doesn't take time

; Lazy evaluation (Promises) using delay and force
(define (delay thunk) (mcons false thunk))

(define (force p)
  (if (mcar p)
      (mcdr p)
      (begin (set-mcdr! p ((mcdr p)))
             (set-mcar! p true)
             (mcdr p))))

(define delay-thunk (delay (lambda () (slow-add 1 2))))

(force delay-thunk) ;1st computation, takes time
(force delay-thunk) ;2nd computation, doesn't take time

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                  Streams                                ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; stream maker
(define (make-stream f v)
  (cons v (lambda () (make-stream f (f v)))))
; streams
(define nats (make-stream (lambda (x) (+ x 1)) 1))
(define pows2 (make-stream (lambda (x) (* x 2)) 1))
; a function that operates on a stream
(define (number-until stream tester)
  (letrec ([get-number (lambda (s n)
                         (let ([pr (s)])
                           (if (tester (car pr))
                               n
                               (get-number (cdr pr) (+ n 1)))
                           ))])
    (get-number stream 0)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                               Memoization                                ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (fib x)
  (letrec ([memo null]
           [calc-fib (lambda (x)
                       (if (or (= x 1) (= x 2))
                           1
                           (+ (get-fib (- x 1))
                              (get-fib (- x 2)))))]
           [get-fib (lambda (x)
                      (let ([pr (assoc x memo)])
                        (if (false? pr)
                            (let ([ans (calc-fib x)])
                              (begin
                                (set! memo (cons (cons x ans) memo))
                                ans ))
                            (cdr pr)
                            )))])
    (get-fib x)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;                                   Macros                                   ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Macro Code Template: 
;(define-syntax syntaxName
;  (syntax-rules (keyword1 keyword2 ...)
;    [(..new syntax..)(..source syntax..)]))

;; Macros by examples

(define-syntax my-if
  (syntax-rules (then else)
    [(my-if e1 then e2 else e3)
     (if e1 e2 e3)]))

(define-syntax comment-out
  (syntax-rules ()
    [(comment-out e1 e2) e2]))

(define-syntax my-delay
  (syntax-rules ()
    [(my-delay e1)
     (mcons #f (lambda () e1))]))

(define-syntax for
  (syntax-rules (to do)
    [(for e1 to e2 do e3)
     (let ([v1 e1]
           [v2 e2])
       (letrec ([loop (lambda (i)
                        (if (> i v2)
                            #t
                            (begin e3 (loop (+ i 1)))))])
         (loop v1))
       )]))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;