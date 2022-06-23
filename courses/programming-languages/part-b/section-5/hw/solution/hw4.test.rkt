;; Mahmoud Elnagar 20/06/2022
#lang racket
(require "hw4.rkt")
(require rackunit)
(require rackunit/text-ui)

;; Helper functions
(define ones (lambda () (cons 1 ones)))
(define (make-stream f v) (lambda() (cons v (make-stream f (f v)))))
(define nats (make-stream (lambda (x) (+ x 1)) 1))

;; Tests
(run-tests
 (test-suite
  "Problem 1: sequence"
  (check-equal? (sequence 3 2 1) '() "low > high")
  (check-equal? (sequence 3 3 1) (list 3) "low = high")
  (check-equal? (sequence 1 9 50) (list 1) "Large Stride")
  (check-equal? (sequence 0 5 1) (list 0 1 2 3 4 5) "Small Stride")
  (check-equal? (sequence 3 11 2) (list 3 5 7 9 11) "high included")
  (check-equal? (sequence 3 8 3) (list 3 6) "high excluded")
  ))

(run-tests
 (test-suite
  "Problem 2: string-append-map"
  (check-equal? (string-append-map '() ".ngr") '() "Empty List")
  (check-equal? (string-append-map '("m") "-ngr") '("m-ngr") "1 element List")
  (check-equal? (string-append-map 
                 (list "dan" "dog" "curry" "dog2")
                 ".jpg") '("dan.jpg" "dog.jpg" "curry.jpg" "dog2.jpg") "many elements")
  ))
           
(run-tests
 (test-suite
  "Problem 3: list-nth-mod"
  (check-exn exn:fail? (lambda() (list-nth-mod (list 0 1 2 3 4) -2)) "negative n throws")
  (check-exn exn:fail? (lambda() (list-nth-mod '() 2)) "empty xs throws")
  (check-equal? (list-nth-mod (list "a" "b") 0) "a" "n = 0")
  (check-equal? (list-nth-mod (list 0 1 2 3 4) 2) 2 "n < xs.length")
  (check-equal? (list-nth-mod (list 0 1 2 3 4) 5) 0 "n = xs.length")
  (check-equal? (list-nth-mod (list 0 1 2 3 4) 7) 2 "n > xs.length")
  ))

(run-tests
 (test-suite
  "Problem 4: stream-for-n-steps"
  (check-equal? (stream-for-n-steps nats 0) '() "no values")
  (check-equal? (stream-for-n-steps nats 1) (list 1) "get one value from stream")
  (check-equal? (stream-for-n-steps nats 2) (list 1 2) "get two value from stream")
  ))

(run-tests
 (test-suite
  "Problem 5: funny-number-stream"
  (check-equal? (stream-for-n-steps funny-number-stream 16)
                (list 1 2 3 4 -5 6 7 8 9 -10 11 12 13 14 -15 16)
                "funny-number-stream test")
  ))

(run-tests
 (test-suite
  "Problem 6: dan-then-dog"
  (check-equal? (stream-for-n-steps dan-then-dog 5)
                (list "dan.jpg" "dog.jpg" "dan.jpg" "dog.jpg" "dan.jpg")
                "dan-then-dog test")
  ))

(run-tests
 (test-suite
  "Problem 7: stream-add-zero"
  (check-equal? (stream-for-n-steps (stream-add-zero nats) 3)
                (list (cons 0 1) (cons 0 2) (cons 0 3))
                "stream-add-zero test")
  ))

(run-tests
 (test-suite
  "Problem 8: cycle-lists"
  (check-equal? (stream-for-n-steps (cycle-lists (list 1) (list "a")) 3)
                (list (cons 1 "a") (cons 1 "a") (cons 1 "a")) 
                "basic test")
  
  (check-equal? (stream-for-n-steps (cycle-lists (list 1 2 3) (list "a" "b")) 4)
                (list (cons 1 "a") (cons 2 "b") (cons 3 "a") (cons 1 "b")) 
                "cycle-lists test")
  ))

(run-tests
 (test-suite
  "Problem 9: vector-assoc"
  (check-equal? (vector-assoc 1 (vector (cons 2 1) (cons 3 1) (cons 4 1) (cons 5 1)))
                #f "no pair matches")
  (check-equal? (vector-assoc 4 (vector (cons 2 1) (cons 3 1) (cons 4 1) (cons 5 1)))
                (cons 4 1) "one pair matches")
  (check-equal? (vector-assoc 3 (vector (cons 2 1) (cons 3 1) (cons 3 2) (cons 5 1)))
                (cons 3 1) "two pairs matches: get the first")
  ))

(define assoc-1 (cached-assoc (list (cons 1 2) (cons 3 4) (cons 3 5)) 2))

(run-tests
 (test-suite
  "Problem 10: cached-assoc"
  (check-equal? (assoc-1 2) #f "no pair matches - not cached") 
  (check-equal? (assoc-1 2) #f "no pair matches - from cache") ;((2.#f) #f)
  (check-equal? (assoc-1 1) (cons 1 2) "one pair matches - not cached")
  (check-equal? (assoc-1 1) (cons 1 2) "one pair matches - from cache") ;((2.#f) (1.2))
  (check-equal? (assoc-1 3) (cons 3 4) "two pairs matches: get the first - not cached")
  (check-equal? (assoc-1 3) (cons 3 4) "two pairs matches: get the first - from cache") ;((3.4) (1.2))
  (check-equal? (assoc-1 1) (cons 1 2) "one pair matches - from cache") ;((3.4) (1.2))
  (check-equal? (assoc-1 2) #f "no pair matches - not cached")
  ))

(define a 2)

(run-tests
 (test-suite
  "Problem 11: while-less"
  (check-equal? (while-less 7 do (begin (set! a (+ a 1)) a)) #t "terminate with #t")
  (check-equal? a 7 "a should equal 7")
  (check-equal? (begin (while-less  2 do (begin (set! a (+ a 1)) a)) a) 8 "e1 < e2")
  (check-equal? (begin (while-less  8 do (begin (set! a (+ a 1)) a)) a) 9 "e1 = e2")
  (check-equal? (begin (while-less 10 do (begin (set! a (+ a 1)) a)) a)10 "e1 = e2 + 1")
  (check-equal? (begin (while-less 15 do (begin (set! a (+ a 1)) a)) a)15 "e1 > e2")
  ))

    

