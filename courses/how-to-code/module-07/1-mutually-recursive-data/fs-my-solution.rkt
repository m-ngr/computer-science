;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname fs-my-solution) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/image)
;;NAME: Mahmoud Elnagar
;;DATE: 29/11/2021
;; fs-my-solution.rkt (type comments and examples)

;; Data definitions:
;; Note: In mutual-reference data definition, examples and templates are
;;       grouped together at the end.

(define-struct elt (name data subs))
;; Element is (make-elt String Integer ListOfElement)
;; interp. An element in the file system, with name, and EITHER data or subs.
;;         If data is 0, then subs is considered to be list of sub elements.
;;         If data is not 0, then subs is ignored.

;; ListOfElement is one of:
;;  - empty
;;  - (cons Element ListOfElement)
;; interp. A list of file system Elements

(define F1 (make-elt "F1" 1 empty))
(define F2 (make-elt "F2" 2 empty))
(define F3 (make-elt "F3" 3 empty))
(define D4 (make-elt "D4" 0 (list F1 F2)))
(define D5 (make-elt "D5" 0 (list F3)))
(define D6 (make-elt "D6" 0 (list D4 D5)))
#;
(define (fn-for-element e)
  (...(elt-name e)
      (elt-data e)
      (fn-for-loe (elt-subs e))))

;; Template Rules used:
;;  - compound: e is (make-elt String Integer ListOfElement)
;;  - reference: (elt-subs e) is ListOfElement [mutual-reference]
#;
(define (fn-for-loe loe)
  (cond [(empty? loe) (...)]
        [else
         (...(fn-for-element (first loe))
             (fn-for-loe (rest loe)))]))

;; Template Rules used:
;;  - one of: 2 cases
;;  - atomic distinct: empty
;;  - compound: (cons Element ListOfElement)
;;  - reference: (first loe) is an Element [mutual-reference]
;;  - self-reference: (rest loe) is a ListOfElement
