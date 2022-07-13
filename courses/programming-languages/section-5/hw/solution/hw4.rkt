;; Mahmoud Elnagar 20/06/2022
#lang racket
(provide (all-defined-out))

;======================================= Problem 1 =============================================
;; Number Number Number -> ListOfNumber
;; produce a list of numbers from low to high (including low and possibly high)
;; separated by stride and in sorted order. Assume stride is positive.

(define (sequence low high stride)
  (if (> low high)
      null
      (cons low (sequence (+ low stride) high stride))))

;======================================= Problem 2 =============================================
;; ListOfString String -> ListOfString
;; append the suffix to the end of all strings in the given list

(define (string-append-map xs suffix)
  (map (lambda (x) (string-append x suffix)) xs))

;======================================= Problem 3 =============================================
;; ListOfT1 Number -> T1
;; return the i-th element in the list, where i is the remainder of (n / xs.length).
;; raise error if n < 0 or the list is empty.

(define (list-nth-mod xs n)
  (let ([len (length xs)])
    (cond [(< n 0) (error "list-nth-mod: negative number")]
          [(= len 0) (error "list-nth-mod: empty list")]
          [else (car (list-tail xs (remainder n len)))])))

;======================================= Problem 4 =============================================
;; StreamOfT1 Number -> ListOfT1
;; produce a list of the first n values produced by the stream s in order. n is non-negative.

(define (stream-for-n-steps s n)
  (if (= n 0)
      null
      (let ([pr (s)])
        (cons (car pr)(stream-for-n-steps (cdr pr) (- n 1))))))

;======================================= Problem 5 =============================================
;; funny-number-stream is a thunk of (cons Number funny-number-stream)
;; a stream of natural numbers except numbers divisble by 5 are negated

(define (funny-number-stream)
  (letrec ([gen (lambda (x)
                  (cons (if (= (remainder x 5) 0) (- x) x)
                        (lambda ()(gen (+ x 1)))))])
    (gen 1)))

;======================================= Problem 6 =============================================
;; dan-then-dog is a thunk of (cons String dan-then-dog)
;; a stream that alternates between the strings "dan.jpg" and "dog.jpg"

(define (dan-then-dog)
  (letrec ([gen (lambda (x)
                  (cons (if x "dan.jpg" "dog.jpg")
                        (lambda () (gen (not x)))))])
    (gen true)))

;======================================= Problem 7 =============================================
;; StreamOfT1 -> StreamOf(Number.T1)
;; produces a stream of pair (0 . v) for each v in the given stream
;; where v is the element produced by the given stream

(define (stream-add-zero s)
  (lambda ()
    (let ([pr (s)])
      (cons (cons 0 (car pr))
            (stream-add-zero (cdr pr))))))

;======================================= Problem 8 =============================================
;; ListOfT1 ListOfT2 -> StreamOf(T1.T2)
;; Produces a stream of pair (v1.v2). The stream cycles forever through the lists.
;; Assume: Lists are non-empty, but may or may not be the same length.

(define (cycle-lists xs ys)
  (letrec ([loop (lambda (n)
                   (cons (cons (list-nth-mod xs n)
                               (list-nth-mod ys n))
                         (lambda ()(loop (+ n 1)))))])
    (lambda ()(loop 0))))

;======================================= Problem 9 =============================================
;; T1 VectorOfAny -> (T1.T2) or #f
;; produces the first pair in the given vector that starts with v. #f if no such pair is found
;; skip all non-pair elements

(define (vector-assoc v vec)
  (letrec ([search (lambda (n)
                     (if (>= n (vector-length vec))
                         #f
                         (let ([vpr (vector-ref vec n)])
                           (if (and (pair? vpr) (equal? (car vpr) v))
                               vpr
                               (search (+ n 1))))))])
    (search 0)))

;======================================= Problem 10 ============================================
;; ListOf(T1.T2) Number -> ( T1 -> (T1.T2) or #f )
;; returns a function that takes one argument v and returns the same thing as (assoc v xs)
;; using internal cache (a vector of n elements) to store the most recent n calls.
;; The cache starts empty (a vector of n elements all have value #f).
;; The cache slots are used in a round-robin fashion: from 0 to n-1 and back to 0 again.

(define (cached-assoc xs n)
  (let ([cache (make-vector n #f)]
        [index 0])
    (lambda (v)
      (let ([cache-ans (vector-assoc v cache)])
        (if cache-ans
            (if (cdr cache-ans) cache-ans #f)
            (let ([ans (assoc v xs)])
              (begin (vector-set! cache index (if ans ans (cons v ans)))
                     (set! index (remainder (+ index 1) n))
                     ans)))))))

;======================================= Problem 11 ============================================
;; MACRO: (while-less e1 do e2)

(define-syntax while-less
  (syntax-rules (do)
    [(while-less e1 do e2)
     (letrec ([v1 e1]
              [loop (lambda ()
                      (if (< e2 v1)
                          (loop)
                          #t))])
       (loop))]))

