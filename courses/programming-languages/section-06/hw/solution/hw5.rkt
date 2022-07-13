;; Mahmoud Elnagar 23/06/2022
#lang racket
(provide (all-defined-out))

;; definition of structures for MUPL programs
(struct var  (string) #:transparent)  ;; a variable, e.g., (var "foo")
(struct int  (num)    #:transparent)  ;; a constant number, e.g., (int 17)
(struct add  (e1 e2)  #:transparent)  ;; add two expressions
(struct ifgreater (e1 e2 e3 e4)    #:transparent) ;; if e1 > e2 then e3 else e4
(struct fun  (nameopt formal body) #:transparent) ;; a recursive(?) 1-argument function
(struct call (funexp actual)       #:transparent) ;; function call
(struct mlet (var e body) #:transparent) ;; a local binding (let var = e in body) 
(struct apair (e1 e2)     #:transparent) ;; make a new pair
(struct fst  (e)    #:transparent) ;; get first part of a pair
(struct snd  (e)    #:transparent) ;; get second part of a pair
(struct aunit ()    #:transparent) ;; unit value -- good for ending a list
(struct isaunit (e) #:transparent) ;; evaluate to 1 if e is unit else 0

;; a closure is not in "source" programs but /is/ a MUPL value; it is what functions evaluate to
(struct closure (env fun) #:transparent) 

;; ======================================== Problem 1-A =========================================
;; List -> MUPL-List
;; produce a MUPL list with the same elements of the given Racket list in the same order.

(define (racketlist->mupllist lox)
  (if (null? lox)
      (aunit)
      (apair (car lox) (racketlist->mupllist (cdr lox)))))

;; ======================================== Problem 1-B =========================================
;; MUPL-List -> List
;; produce a Racket list with the same elements of the given MUPL list in the same order.

(define (mupllist->racketlist lomx)
  (if (aunit? lomx)
      null
      (cons (apair-e1 lomx) (mupllist->racketlist (apair-e2 lomx)))))

;; ======================================== Problem 2 ===========================================
;; Env is ListOf(String.MUPL)
;; ----------------------------------------------------------------------------------------------
;; Env String -> MUPL
;; lookup a variable in an environment return MUPL value if found, fail otherwise.

(define (envlookup env str)
  (cond [(null? env) (error "unbound variable during evaluation" str)]
        [(equal? (car (car env)) str) (cdr (car env))]
        [#t (envlookup (cdr env) str)]))

;;------------------------------------------------------------------------------------------------
;; MUPL Env -> MUPL
;; produce MUPL value that results from evaluating the given MUPL expression in the given environment

(define (eval-under-env e env)
  (cond [(int? e) e]

        [(aunit? e) e]

        [(closure? e) e]
        
        [(var? e) (envlookup env (var-string e))]

        [(fun? e) (closure env e)]

        [(add? e) 
         (let ([v1 (eval-under-env (add-e1 e) env)]
               [v2 (eval-under-env (add-e2 e) env)])
           (if (and (int? v1)(int? v2))
               (int (+ (int-num v1)(int-num v2)))
               (error "MUPL addition applied to non-number")))]
        
        [(ifgreater? e)
         (let ([v1 (eval-under-env (ifgreater-e1 e) env)]
               [v2 (eval-under-env (ifgreater-e2 e) env)])
           (cond [(not (and (int? v1) (int? v2)))
                  (error "MUPL comparison applied to non-number")]
                 [(> (int-num v1) (int-num v2))
                  (eval-under-env (ifgreater-e3 e) env)]
                 [#t
                  (eval-under-env (ifgreater-e4 e) env)]))]
   
        [(call? e)
         (let ([v1 (eval-under-env (call-funexp e) env)]
               [v2 (eval-under-env (call-actual e) env)])
           (if (closure? v1)
               (let* ([Env1 (cons
                             (cons (fun-formal (closure-fun v1)) v2)
                             (closure-env v1))]
                      [funName (fun-nameopt (closure-fun v1))]
                      [newEnv (if funName (cons (cons funName v1) Env1) Env1)])
                 
                 (eval-under-env (fun-body (closure-fun v1)) newEnv))
              
               (error "MUPL call applied to non-function")))]

        [(mlet? e)
         (eval-under-env (mlet-body e)
                         (cons (cons (mlet-var e)
                                     (eval-under-env (mlet-e e) env))
                               env))]

        [(apair? e)
         (apair (eval-under-env (apair-e1 e) env)
                (eval-under-env (apair-e2 e) env))]

        [(fst? e)
         (let ([v (eval-under-env (fst-e e) env)])
           (if (apair? v)
               (apair-e1 v)
               (error "MUPL fst applied to non-pair")))]

        [(snd? e)
         (let ([v (eval-under-env (snd-e e) env)])
           (if (apair? v)
               (apair-e2 v)
               (error "MUPL snd applied to non-pair")))]
        
        [(isaunit? e)
         (if (aunit? (eval-under-env (isaunit-e e) env))
             (int 1)
             (int 0))]
        
        [#t (error (format "bad MUPL expression: ~v" e))]))

;;------------------------------------------------------------------------------------------------
;; MUPL -> MUPL
;; produce MUPL value that results from evaluating the given MUPL expression

(define (eval-exp e) (eval-under-env e null))

;; ======================================== Problem 3-A =========================================
;; MUPL MUPL MUPL -> MUPL
;; returns a MUPL expression that when evaluated will:
;;  - return the value of eval e2 when value of e1 is aunit
;;  - return the value of eval e3 when value of e1 is not aunit

(define (ifaunit e1 e2 e3) (ifgreater (isaunit e1) (int 0) e2 e3))

;; ======================================== Problem 3-B =========================================
;; ListOf(String.MUPL) MUPL -> MUPL
;; returns a MUPL expression that when evaluated will return the value of evaluating e2
;; under an env extended by lstlst sequentially [env is extended for each binding]

(define (mlet* lstlst e2)
  (if (null? lstlst)
      e2
      (mlet (car (car lstlst))
            (cdr (car lstlst))
            (mlet* (cdr lstlst) e2))))

;; ======================================== Problem 3-C =========================================
;; MUPL MUPL MUPL MUPL -> MUPL
;; produce a MUPL expression that when evaluated will:
;;  - return the value of eval e3, if eval(e1) = eval(e2)
;;  - return the value of eval e4 otherwise.

(define (ifeq e1 e2 e3 e4)
  (mlet "_x" e1
        (mlet "_y" e2
              (ifgreater (var "_x") (var "_y") e4
                         (ifgreater (var "_y") (var "_x") e4 e3)))))

;; ======================================== Problem 4-A =========================================
;; MUPL-fun -> (MUPL-list -> MUPL-list)
;; write a curried map function in MUPL

(define mupl-map
  (fun "map" "f"
       (fun #f "xs"
            (ifaunit (var "xs")
                     (aunit)
                     (apair
                      (call (var "f") (fst (var "xs")))
                      (call (call (var "map")(var "f"))
                            (snd (var "xs"))))))))

;; ======================================== Problem 4-B =========================================
;; MUPL-int -> (MUPL-list -> MUPL-list)
;; return a function that when called with a MUPL-list of int will produce a MUPL-list
;; with the int i added to every element

(define mupl-mapAddN 
  (mlet "map" mupl-map
        (fun #f "i" (call (var "map")
                          (fun #f "x" (add (var "x") (var "i")))))))

;; ==================================== Challenge Problem =======================================

(struct fun-challenge (nameopt formal body freevars) #:transparent)

;; ======================================== Problem 5-A =========================================
;; MUPL -> SetOfString
;; return a set of free variables in the given MUPL expression

(define (get-free-vars e)
  (cond [(var? e) (set (var-string e))]
        
        [(add? e)
         (set-union (get-free-vars (add-e1 e))
                    (get-free-vars (add-e2 e)))]
        
        [(ifgreater? e)
         (set-union (get-free-vars (ifgreater-e1 e))
                    (get-free-vars (ifgreater-e2 e))
                    (get-free-vars (ifgreater-e3 e))
                    (get-free-vars (ifgreater-e4 e)))]
                                   
        [(fun? e)
         (let ([freeVars (set-remove (get-free-vars (fun-body e))
                                     (fun-formal e))]
               [fName (fun-nameopt e)])
           (if fName (set-remove freeVars fName) freeVars))]
        
        [(call? e)
         (set-union (get-free-vars (call-funexp e))
                    (get-free-vars (call-actual e)))]
        
        [(mlet? e)
         (set-union (get-free-vars (mlet-e e))
                    (set-remove (get-free-vars (mlet-body e))
                                (mlet-var e)))]
        
        [(apair? e)
         (set-union (get-free-vars (apair-e1 e))
                    (get-free-vars (apair-e2 e)))]
        
        [(fst? e) (get-free-vars (fst-e e))]
        
        [(snd? e) (get-free-vars (snd-e e))]
        
        [(isaunit? e) (get-free-vars (isaunit-e e))]
        
        [#t (set)]))

;; ======================================== Problem 5-B =========================================
;; MUPL -> MUPL-C
;; replace each fun expression with fun-challenge in the given MUPL
;; fun-challenge will need an extra freevars fields

(define (compute-free-vars e)
  (cond [(add? e)
         (add (compute-free-vars (add-e1 e))
              (compute-free-vars (add-e2 e)))]
        
        [(ifgreater? e)
         (ifgreater (compute-free-vars (ifgreater-e1 e))
                    (compute-free-vars (ifgreater-e2 e))
                    (compute-free-vars (ifgreater-e3 e))
                    (compute-free-vars (ifgreater-e4 e)))]
        
        [(fun? e)
         (fun-challenge (fun-nameopt e)
                        (fun-formal e)
                        (compute-free-vars (fun-body e))
                        (get-free-vars e))]
        
        [(call? e)
         (call (compute-free-vars (call-funexp e))
               (compute-free-vars (call-actual e)))]
        
        [(mlet? e)
         (mlet (mlet-var e)
               (compute-free-vars (mlet-e e))
               (compute-free-vars (mlet-body e)))]
        
        [(apair? e)
         (apair (compute-free-vars (apair-e1 e))
                (compute-free-vars (apair-e2 e)))]
        
        [(fst? e) (fst (compute-free-vars (fst-e e)))]
        
        [(snd? e) (snd (compute-free-vars (snd-e e)))]
        
        [(isaunit? e)(isaunit (compute-free-vars (isaunit-e e)))]
        
        [#t e]))

;; ======================================== Problem 5-C =========================================
;; MUPL-C Env -> MUPL
;; produce MUPL value that results from evaluating the given MUPL-C expression in the given environment

(define (eval-under-env-c e env)
  (cond [(fun-challenge? e)
         (closure (filter (lambda (x)
                            (set-member? (fun-challenge-freevars e)
                                         (car x)))
                          env)
                  (fun (fun-challenge-nameopt e)
                       (fun-challenge-formal e)
                       (fun-challenge-body e)))]
        
        [(int? e) e]
        
        [(aunit? e) e]
        
        [(closure? e) e]
        
        [(var? e) (envlookup env (var-string e))]
        
        [(add? e) 
         (let ([v1 (eval-under-env-c (add-e1 e) env)]
               [v2 (eval-under-env-c (add-e2 e) env)])
           (if (and (int? v1)
                    (int? v2))
               (int (+ (int-num v1) 
                       (int-num v2)))
               (error "MUPL addition applied to non-number")))]
        
        [(ifgreater? e)
         (let ([v1 (eval-under-env-c (ifgreater-e1 e) env)]
               [v2 (eval-under-env-c (ifgreater-e2 e) env)])
           (cond [(not (and (int? v1) (int? v2)))
                  (error "MUPL comparison applied to non-number")]
                 [(> (int-num v1) (int-num v2))
                  (eval-under-env-c (ifgreater-e3 e) env)]
                 [#t
                  (eval-under-env-c (ifgreater-e4 e) env)]))]
        
        
        [(call? e)
         (let ([v1 (eval-under-env-c (call-funexp e) env)]
               [v2 (eval-under-env-c (call-actual e) env)])
           (if (closure? v1)
               (let* ([Env1 (cons
                             (cons (fun-formal (closure-fun v1)) v2)
                             (closure-env v1))]
                      [funName (fun-nameopt (closure-fun v1))]
                      [newEnv (if funName (cons (cons funName v1) Env1) Env1)])
                 (eval-under-env-c (fun-body (closure-fun v1)) newEnv))
              
               (error "MUPL call applied to non-function")))]
        
        [(mlet? e)
         (eval-under-env-c (mlet-body e)
                           (cons (cons (mlet-var e)
                                       (eval-under-env-c (mlet-e e) env))
                                 env))]
        
        [(apair? e)
         (apair (eval-under-env-c (apair-e1 e) env)
                (eval-under-env-c (apair-e2 e) env))]
        
        [(fst? e)
         (let ([v (eval-under-env-c (fst-e e) env)])
           (if (apair? v)
               (apair-e1 v)
               (error "MUPL fst applied to non-pair")))]
        
        [(snd? e)
         (let ([v (eval-under-env-c (snd-e e) env)])
           (if (apair? v)
               (apair-e2 v)
               (error "MUPL snd applied to non-pair")))]
        
        [(isaunit? e)
         (if (aunit? (eval-under-env-c (isaunit-e e) env))
             (int 1)
             (int 0))]

        [#t (error (format "bad MUPL expression: ~v" e))]))

;;------------------------------------------------------------------------------------------------
;; MUPL -> MUPL
;; produce MUPL value that results from evaluating the given MUPL expression
;; using MUPL-C transformation to reduce closure env to free-vars only

(define (eval-exp-c e) (eval-under-env-c (compute-free-vars e) null))
