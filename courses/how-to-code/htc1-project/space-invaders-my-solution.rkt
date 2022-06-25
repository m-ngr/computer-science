;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname space-invaders-my-solution) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
(require 2htdp/universe)
(require 2htdp/image)
;;NAME: Mahmoud Elnagar
;;DATE: 16/11/2021
;; Space Invaders


;; Constants:

(define WIDTH  300)
(define HEIGHT 500)

(define INVADER-X-SPEED 1.5)  ;speeds (not velocities) in pixels per tick
(define INVADER-Y-SPEED 1.5)  ;pixel/tick
(define TANK-SPEED 2)         ;pixel/tick
(define MISSILE-SPEED 10)     ;pixel/tick
 
(define INVADE-RATE 40)       ;tick for new invader

(define BACKGROUND (empty-scene WIDTH HEIGHT))

(define INVADER
  (overlay/xy (ellipse 10 15 "outline" "blue")              ;cockpit cover
              -5 6
              (ellipse 20 10 "solid"   "blue")))            ;saucer

(define TANK
  (overlay/xy (overlay (ellipse 28 8 "solid" "black")       ;tread center
                       (ellipse 30 10 "solid" "green"))     ;tread outline
              5 -14
              (above (rectangle 5 10 "solid" "black")       ;gun
                     (rectangle 20 10 "solid" "black"))))   ;main body

(define TANK-HEIGHT/2 (/ (image-height TANK) 2))

(define MISSILE (ellipse 5 15 "solid" "red"))

(define MISSILE-Y (- HEIGHT (image-height TANK))) ; initial y-pos of the missile

(define MISSILE-H/2 (/ (image-height MISSILE) 2)) ; MISSILE height/2
(define MISSILE-W/2 (/ (image-width MISSILE) 2))  ; MISSILE width/2

(define INVADER-H/2 (/ (image-height INVADER) 2)) ;INVADER height/2
(define INVADER-W/2 (/ (image-width INVADER) 2))  ;INVADER width/2
;=================================================================================================================================

;; Data Definitions:

(define-struct game (invaders missiles tank))
;; Game is (make-game ListOfInvader ListOfMissile Tank)
;; interp. the current state of a space invaders game
;;         with the current invaders, missiles and tank position

;; Game constants defined below Missile data definition

#;
(define (fn-for-game s)
  (... (fn-for-loinv (game-invaders s))
       (fn-for-lom (game-missiles s))
       (fn-for-tank (game-tank s))))

;=================================================

(define-struct tank (x dir))
;; Tank is (make-tank Number Integer[-1, 1])
;; interp. the tank location is x, HEIGHT - TANK-HEIGHT/2 in screen coordinates
;;         the tank moves TANK-SPEED pixels per clock tick left if dir -1, right if dir 1

(define T0 (make-tank (/ WIDTH 2) 1))   ;center going right
(define T1 (make-tank 50 1))            ;going right
(define T2 (make-tank 50 -1))           ;going left

#;
(define (fn-for-tank t)
  (... (tank-x t) (tank-dir t)))

;===================================================

(define-struct invader (x y dx))
;; Invader is (make-invader Number Number Number[-1,1])
;; interp. the invader is at (x, y) in screen coordinates
;;         the invader moves in right direction if dx = 1
;;         the invader moves in left direction if dx = -1

(define I1 (make-invader 150 100 1))           ;not landed, moving right
(define I2 (make-invader 150 HEIGHT -1))       ;exactly landed, moving left
(define I3 (make-invader 150 (+ HEIGHT 10) 1)) ;> landed, moving right


#;
(define (fn-for-invader invader)
  (... (invader-x invader) (invader-y invader) (invader-dx invader)))

;===================================================

(define-struct missile (x y))
;; Missile is (make-missile Number Number)
;; interp. the missile's location is x y in screen coordinates

(define M1 (make-missile 150 300))                              
(define M2 (make-missile (invader-x I1) (+ (invader-y I1) 10))) 
(define M3 (make-missile (invader-x I1) (+ (invader-y I1)  5)))  

#;
(define (fn-for-missile m)
  (... (missile-x m) (missile-y m)))
;----------------------------------------------------
(define G0 (make-game empty empty T0))
(define G1 (make-game empty empty T1))
(define G2 (make-game (list I1) (list M1) T1))
(define G3 (make-game (list I1 I2) (list M1 M2) T1))
;===================================================

;; ListOfMissile is one of:
;;  - empty
;;  - (cons Missile ListOfMissile)
;; Interp. a list of missiles

(define LOM-0 empty)
(define LOM-1 (list M1 M2))
#;
(define (fn-for-lom lom)
  (cond [(empty? lom) (...)]
        [else
         (... (fn-for-missile (first lom))
              (fn-for-lom (rest lom)))
         ]))
;====================================================
;; ListOfInvader is one of:
;;  - empty
;;  - (cons Invader ListOfInvader)
;; Interp. a list of invaders

(define LOINV-0 empty)
(define LOINV-1 (list I1 I2))
#;
(define (fn-for-loinv loinv)
  (cond [(empty? loinv) (...)]
        [else
         (... (fn-for-invader (first loinv))
              (fn-for-loinv (rest loinv)))
         ]))
;====================================================
(define-struct world (game ticks))
;; World is (make-world Game Natural)
;; Interp. world with game representing the game elements
;; ticks represents ticks from the start of the game

(define W0 (make-world G0  0)) ;initial world state
(define W1 (make-world G0 50)) ;world state with 50 ticks passed
#;
(define (fn-for-world w)
  (... (fn-for-game (world-game w))
       (world-ticks w)))
;=========================================================[Function Design]==========================================================

;; Functions:

;; World -> World
;; start the world with (main W0)

(define (main w)
  (big-bang w                   ; World
    (on-tick   next-world)      ; World -> World
    (to-draw   render-world)    ; World -> Image
    (stop-when end-world?)      ; World -> Boolean
    (on-key    handle-key)))    ; World KeyEvent -> World

;=====================================================================================================
;; World -> World
;; produce the next game state, and increase ticks by one

;(define (next-world w) w) ;stub

;;<game examples are provided through next-game>
(check-random (next-world (make-world G0 0))
                          (make-world (next-game G0 0) 1))

(define (next-world w)
  (make-world (next-game (world-game w) (world-ticks w))
              (add1 (world-ticks w))))
;============================================================
;; Game Natural -> Game
;; produce the next game state, steps:
;;  A- Remove images from the game:
;;     1- invaders that collide with missiles
;;     2- missiles that collide with invaders
;;     3- missiles out of the screen
;;  B- Advance images on the game:
;;     1- advance tank
;;     2- advance missiles upward
;;     3- advance invaders
;;  C- Add images to the game:
;;     1- add new invaders

;(define (next-game g n) g) ;stub

;; <examples are long to code, I will depend on sub-functions examples and graphical output>

(define (next-game g n)
  (add-to-game (advance-game (remove-from-game g)) n))
;------------------------------------------------------------
;; Game -> Game
;; Remove from the game:
;;  1- invaders that collide with missiles
;;  2- missiles that collide with invaders
;;  3- missiles out of the screen

;(define (remove-from-game g) g) ;stub

(check-expect (remove-from-game (make-game
                                 (list (make-invader 150 100  1)
                                       (make-invader 120 120 -1))
                                 (list (make-missile 150 100)
                                       (make-missile 20 0))
                                 (make-tank 50 1)))
              (make-game
               (list (make-invader 120 120 -1))
               empty
               (make-tank 50 1)))

(define (remove-from-game g)
  (make-game (destroy-invaders (game-invaders g) (game-missiles g))
             (remove-missiles
              (destroy-missiles (game-invaders g) (game-missiles g)))
             (game-tank g)))
;************************************************************
;; ListOfInvader ListOfMissile -> ListOfInvader
;; remove invaders that collide with missiles from the given list of invaders
;(define (destroy-invaders loinv lom) loinv) ;stub

(check-expect (destroy-invaders empty empty) empty)
(check-expect (destroy-invaders empty (list (make-missile 150 100)
                                            (make-missile 20 50)))
              empty)
(check-expect (destroy-invaders (list (make-invader 150 100  1)
                                      (make-invader 120 120 -1)
                                      (make-invader 110 190  1))
                                (list (make-missile 150 100)
                                      (make-missile 20 50)
                                      (make-missile 110 190)))
              (list (make-invader 120 120 -1)))

(define (destroy-invaders loinv lom)
  (cond [(empty? loinv) empty]
        [else
         (if (invader-collide? (first loinv) lom)
             (destroy-invaders (rest loinv) lom)
             (cons (first loinv) (destroy-invaders (rest loinv) lom) )
             )]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Invader ListOfMissile -> Boolean
;; return true if the given invader collides with any of the given missiles
;(define (invader-collide? inv lom) false) ;stub

(check-expect (invader-collide? (make-invader 10 20 1) empty) false)
(check-expect (invader-collide? (make-invader 10 20 1)
                                (list (make-missile 20 50)
                                      (make-missile 30 50)
                                      (make-missile 40 50)))
              false)
(check-expect (invader-collide? (make-invader 10 20 1)
                                (list (make-missile 20 50)
                                      (make-missile 10 20)
                                      (make-missile 40 50)))
              true)

(define (invader-collide? inv lom)
  (cond [(empty? lom) false]
        [else
         (if (collide? inv (first lom))
             true
             (invader-collide? inv (rest lom)))
         ]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Invader Missile -> Boolean
;; return true if the given invader collides(has shared pixels)with the given missile
;(define (collide? inv m) false) ;stub

(check-expect (collide? (make-invader 10 20 1) (make-missile 130 120)) false)
(check-expect (collide? (make-invader 10 20 1) (make-missile 10 20)) true)
(check-expect (collide? (make-invader 9  20 1) (make-missile 10 20)) true)
(check-expect (collide? (make-invader 10 22 1) (make-missile 10 20)) true)
;; <require too many tests!, I will depend on graphical results>

(define (collide? inv m)
  (and (< (- (missile-x m) MISSILE-W/2) (+ (invader-x inv) INVADER-W/2))
       (> (+ (missile-x m) MISSILE-W/2) (- (invader-x inv) INVADER-W/2))
       (< (- (missile-y m) MISSILE-H/2) (+ (invader-y inv) INVADER-H/2))
       (> (+ (missile-y m) MISSILE-H/2) (- (invader-y inv) INVADER-H/2))
       ))
;************************************************************
;; ListOfInvader ListOfMissile -> ListOfMissile
;; remove missiles that collide with invaders from the given list of missiles
;(define (destroy-missiles loinv lom) lom) ;stub

(check-expect (destroy-missiles empty empty) empty)
(check-expect (destroy-missiles (list (make-invader 150 100  1)
                                      (make-invader 120 120 -1)) empty)
              empty)
(check-expect (destroy-missiles (list (make-invader 150 100  1)
                                      (make-invader 120 120 -1)
                                      (make-invader 110 190  1))
                                (list (make-missile 150 100)
                                      (make-missile 20 50)
                                      (make-missile 110 190)))
              (list(make-missile 20 50)))

(define (destroy-missiles loinv lom)
  (cond [(empty? lom) empty]
        [else
         (if (missile-collide? (first lom) loinv)
             (destroy-missiles loinv (rest lom))
             (cons (first lom) (destroy-missiles loinv (rest lom))))
         ]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Missile ListOfInvader -> Boolean
;; return true if the given missile collides with any of the given invaders
;(define (missile-collide? m loinv) false) ;stub

(check-expect (missile-collide? (make-missile 10 20) empty) false)
(check-expect (missile-collide? (make-missile 10 20)
                                (list (make-invader 150 100  1)
                                      (make-invader 120 120 -1)
                                      (make-invader 110 190  1)))
              false)
(check-expect (missile-collide? (make-missile 10 20)
                                (list (make-invader 150 100  1)
                                      (make-invader 10 20 -1)
                                      (make-invader 110 190  1)))
              true)

(define (missile-collide? m loinv)
  (cond [(empty? loinv) false]
        [else
         (if (collide? (first loinv) m)
             true
             (missile-collide? m (rest loinv)))
         ]))
;************************************************************
;; ListOfMissile -> ListOfMissile
;; remove missiles out of the screen from the given list
;(define (remove-missiles lom) lom) ;stub

(check-expect (remove-missiles empty) empty)
(check-expect (remove-missiles (list (make-missile 150 100)
                                     (make-missile 20 0)))
              (list (make-missile 150 100)))
(check-expect (remove-missiles (list (make-missile 150  0)
                                     (make-missile 20  10)))
              (list (make-missile 20 10)))
(check-expect (remove-missiles (list (make-missile 150   0)
                                     (make-missile 20   -1)))
              empty)

(define (remove-missiles lom)
  (cond [(empty? lom) empty]
        [else
         (if (remove-missile? (first lom))
             (remove-missiles (rest lom))
             (cons (first lom) (remove-missiles (rest lom)))
             )]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Missile -> Boolean
;; returns true if missile's y-pos <= 0
;(define (remove-missile? m) false) ;stub

(check-expect (remove-missile? (make-missile 10 20)) false)
(check-expect (remove-missile? (make-missile 10  0)) true)
(check-expect (remove-missile? (make-missile 10 -1)) true)

(define (remove-missile? m)
  (<= (missile-y m) 0))
;------------------------------------------------------------
;; Game -> Game
;; Advance images on the game:
;;  1- advance tank
;;  2- advance missiles upward
;;  3- advance invaders

;(define (advance-game g) g) ;stub

(check-expect (advance-game (make-game
                             (list (make-invader 150 100  1)
                                   (make-invader 120 120 -1))
                             (list (make-missile 50 50)
                                   (make-missile 20 30))
                             (make-tank 50 1)))
              (make-game
               (list (make-invader (+ 150 (* 1 INVADER-X-SPEED))
                                   (+ 100 INVADER-Y-SPEED)   1)
                     (make-invader (+ 120 (* -1 INVADER-X-SPEED))
                                   (+ 120 INVADER-Y-SPEED)  -1))
               (list (make-missile 50 (- 50 MISSILE-SPEED))
                     (make-missile 20 (- 30 MISSILE-SPEED)))
               (make-tank (+ 50 TANK-SPEED) 1)))

(define (advance-game g)
  (make-game (advance-invaders (game-invaders g))
             (advance-missiles (game-missiles g))
             (advance-tank     (game-tank g))))
;************************************************************
;; ListOfInvader -> ListOfInvader
;; advance invaders at a 45 degree angle. change direction when hit a wall
;(define (advance-invaders loinv) loinv) ;stub

(check-expect (advance-invaders empty) empty)

(check-expect (advance-invaders  (list (make-invader 50 30  1)
                                       (make-invader 60 40 -1)))
              (list (make-invader (+ 50 (* 1 INVADER-X-SPEED))
                                  (+ 30 INVADER-Y-SPEED)   1)
                    (make-invader (+ 60 (* -1 INVADER-X-SPEED))
                                  (+ 40 INVADER-Y-SPEED)  -1)))

(check-expect (advance-invaders  (list (make-invader 0 30  1)
                                       (make-invader 0 40 -1)))
              (list (make-invader (+ 0 (* 1 INVADER-X-SPEED))
                                  (+ 30 INVADER-Y-SPEED)  1)
                    (make-invader 0 40 1)))

(check-expect (advance-invaders  (list (make-invader WIDTH 30  1)
                                       (make-invader WIDTH 40 -1)))
              (list (make-invader WIDTH 30  -1)
                    (make-invader (+ WIDTH (* -1 INVADER-X-SPEED))
                                  (+ 40 INVADER-Y-SPEED)  -1)))


(define (advance-invaders loinv)
  (cond [(empty? loinv) empty]
        [else
         (cons (advance-invader (first loinv))
               (advance-invaders (rest loinv)))
         ]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Invader -> Invader
;; advance the given invader in x-pos by INVADER-X-SPEED and
;; in y-pos by INVADER-Y-SPEED, direction dx changed when hit a wall
;(define (advance-invader inv) inv) ;stub

(check-expect (advance-invader (make-invader WIDTH 30  1))  (make-invader WIDTH 30  -1))
(check-expect (advance-invader (make-invader WIDTH 40 -1))
              (make-invader (+ WIDTH (* -1 INVADER-X-SPEED))(+ 40 INVADER-Y-SPEED)  -1))

(check-expect (advance-invader (make-invader 0 30  1))
              (make-invader (+ 0 (* 1 INVADER-X-SPEED))(+ 30 INVADER-Y-SPEED)  1))
(check-expect (advance-invader (make-invader 0 40 -1))  (make-invader 0 40 1))

(check-expect (advance-invader (make-invader 50 30  1))
              (make-invader (+ 50 (* 1 INVADER-X-SPEED))
                            (+ 30 INVADER-Y-SPEED)   1))

(check-expect (advance-invader (make-invader 60 40 -1))
              (make-invader (+ 60 (* -1 INVADER-X-SPEED))
                            (+ 40 INVADER-Y-SPEED)  -1))

(define (advance-invader inv)
  (cond [(<= (invader-new-x inv)     0)   (make-invader     0 (invader-y inv)  1)]
        [(>= (invader-new-x inv) WIDTH)   (make-invader WIDTH (invader-y inv) -1)]
        [else
         (make-invader (invader-new-x inv)
                       (+ (invader-y inv) INVADER-Y-SPEED)
                       (invader-dx inv))]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Invader -> Number
;; produce the new x position of the given invader after advancing
;; regardless of the screen bounds

;(define (invader-new-x inv) 0);stub

(check-expect (invader-new-x (make-invader 0 0  1)) (+ 0 (*  1 INVADER-X-SPEED)))
(check-expect (invader-new-x (make-invader 0 0 -1)) (+ 0 (* -1 INVADER-X-SPEED)))

(define (invader-new-x inv)
  (+ (invader-x inv) (* (invader-dx inv) INVADER-X-SPEED)))
;************************************************************
;; ListOfMissile -> ListOfMissile
;; advance missiles upward
;(define (advance-missiles lom) lom) ;stub

(check-expect (advance-missiles empty) empty)
(check-expect (advance-missiles  (list (make-missile 50 50)
                                       (make-missile 20 30)))
              (list (make-missile 50 (- 50 MISSILE-SPEED))
                    (make-missile 20 (- 30 MISSILE-SPEED))))

(define (advance-missiles lom)
  (cond [(empty? lom) empty]
        [else
         (cons (advance-missile (first lom))
               (advance-missiles (rest lom)))
         ]))
;++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Missile -> Missile
;; advance the given missile upward
;(define (advance-missile m) m) ;stub

(check-expect (advance-missile (make-missile 20 30))
              (make-missile 20 (- 30 MISSILE-SPEED)))

(define (advance-missile m)
  (make-missile (missile-x m)
                (- (missile-y m) MISSILE-SPEED)))
;************************************************************
;; Tank -> Tank
;; advance the x-coordinate of the tank depending on its direction
;; tank moves right at dir = 1 ; tank moves left at dir = -1

;(define (advance-tank t) t) ;stub

(check-expect (advance-tank (make-tank 0  1)) (make-tank (+ 0 TANK-SPEED) 1))
(check-expect (advance-tank (make-tank 0 -1)) (make-tank 0 -1))
(check-expect (advance-tank (make-tank WIDTH  1)) (make-tank WIDTH 1))
(check-expect (advance-tank (make-tank WIDTH -1)) (make-tank (+ WIDTH (* TANK-SPEED -1)) -1))

(check-expect (advance-tank (make-tank 50  1)) (make-tank (+ 50 (* TANK-SPEED  1))  1))
(check-expect (advance-tank (make-tank 50 -1)) (make-tank (+ 50 (* TANK-SPEED -1)) -1))

(define (advance-tank t)
  (cond [(<= (tank-new-x t)     0)  (make-tank     0 (tank-dir t))]
        [(>= (tank-new-x t) WIDTH)  (make-tank WIDTH (tank-dir t))]
        [else (make-tank (tank-new-x t) (tank-dir t))]))
;+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;; Tank -> Number
;; produce the x position of the given tank after advancing regardless of screen bounds
;(define (tank-new-x t) 0) ;stub

(check-expect (tank-new-x (make-tank 0  1)) (+ 0 (* TANK-SPEED  1)))
(check-expect (tank-new-x (make-tank 0 -1)) (+ 0 (* TANK-SPEED -1)))
(check-expect (tank-new-x (make-tank WIDTH  1)) (+ WIDTH (* TANK-SPEED  1)))
(check-expect (tank-new-x (make-tank WIDTH -1)) (+ WIDTH (* TANK-SPEED -1)))

(check-expect (tank-new-x (make-tank 50  1)) (+ 50 (* TANK-SPEED  1)))
(check-expect (tank-new-x (make-tank 50 -1)) (+ 50 (* TANK-SPEED -1)))

(define (tank-new-x t)
  (+ (tank-x t)(* TANK-SPEED (tank-dir t))))
;------------------------------------------------------------
;; Game Natural -> Game
;; Add images to the game:
;;  1- add new invaders each INVADE-RATE ticks passed
;(define (add-to-game g n) g) ;stub

(check-random (add-to-game G2 0)
              (make-game (cons (make-invader (random WIDTH) 0 1)
                               (game-invaders G2))
                         (game-missiles G2)
                         (game-tank G2)))
(check-expect (add-to-game G2 4) G2)

(define (add-to-game g n)
  (if (= (modulo n INVADE-RATE) 0)
      (make-game (add-invader (game-invaders g))
                 (game-missiles g)
                 (game-tank g))
      g))
;************************************************************
;; ListOfInvader -> ListOfInvader
;; adds new invader at random x-position at the top of the screen to the given list of invaders
;(define (add-invader loinv) loinv) ;stub

(check-random (add-invader empty) (list (make-invader (random WIDTH) 0 1)))
(check-random (add-invader LOINV-1) (cons (make-invader (random WIDTH) 0 1) LOINV-1))

(define (add-invader loinv)
  (cons (make-invader (random WIDTH) 0 1)loinv))
;============================================================
;; World -> Image
;; render the current world state, mapping for render-game
;(define (render-world w) empty-image) ;stub

;; <examples are provided in render-game>

(define (render-world w)
  (render-game (world-game w)))
;============================================================
;; Game -> Image
;; render the game on the BACKGROUND, steps:
;;   1- render tank
;;   2- render missiles
;;   3- render invaders

;(define (render-game g) BACKGROUND);stub

(check-expect (render-game G0)
              (put-image TANK (/ WIDTH 2) TANK-HEIGHT/2
                         BACKGROUND))

(check-expect (render-game G2)
              (place-image INVADER 150 100
                           (place-image MISSILE 150 300
                                        (put-image TANK 50 TANK-HEIGHT/2
                                                   BACKGROUND))))

(define (render-game g)
  (render-invaders (game-invaders g)
                   (render-missiles (game-missiles g)
                                    (render-tank (game-tank g) BACKGROUND))))
;------------------------------------------------
;; Tank Image -> Image
;; render the tank on the given image
;(define (render-tank t img) img) ;stub

(check-expect (render-tank T0 BACKGROUND)
              (put-image TANK (/ WIDTH 2) TANK-HEIGHT/2 BACKGROUND))

(define (render-tank t img)
  (put-image TANK (tank-x t) TANK-HEIGHT/2 img))

;------------------------------------------------
;; ListOfMissile Image -> Image
;; render the given list of missiles on the given image
;(define (render-missiles lom img) img) ;stub

(check-expect (render-missiles empty BACKGROUND) BACKGROUND)
(check-expect (render-missiles (list (make-missile 150 300)
                                     (make-missile 100 200)) BACKGROUND)
              (place-image MISSILE 150 300
                           (place-image MISSILE 100 200 BACKGROUND)))

(define (render-missiles lom img)
  (cond [(empty? lom) img]
        [else
         (render-missiles (rest lom)
                          (render-missile (first lom) img) )
         ]))
;------------------------------------------------
;; Missile Image -> Image
;; render the given missile on the given image
;(define (render-missile m img) img) ;stub

(check-expect (render-missile (make-missile 150 300) BACKGROUND)
              (place-image MISSILE 150 300 BACKGROUND))

(define (render-missile m img)
  (place-image MISSILE (missile-x m) (missile-y m) img))
;------------------------------------------------
;; ListOfInvader Image -> Image
;; render the given list of invaders on the given image
;(define (render-invaders loinv img) img) ;stub

(check-expect (render-invaders empty BACKGROUND) BACKGROUND)
(check-expect (render-invaders (list (make-invader 150 100 12)
                                     (make-invader 100 120 12)) BACKGROUND)
              (place-image INVADER 150 100
                           (place-image INVADER 100 120 BACKGROUND)))

(define (render-invaders loinv img)
  (cond [(empty? loinv) img]
        [else
         (render-invaders (rest loinv)
                          (render-invader (first loinv) img))
         ]))
;-----------------------------------------------------------
;; Invader Image -> Image
;; render the given invader on the given image
;(define (render-invader inv img) img) ;stub

(check-expect (render-invader (make-invader 100 120 12) BACKGROUND )
              (place-image INVADER 100 120 BACKGROUND))

(define (render-invader inv img)
  (place-image INVADER (invader-x inv) (invader-y inv) img))

;============================================================
;; World -> Boolean
;; produces true to end the world if an invader landed
;(define (end-world? w) false) ;stub
;; <examples are provided through end-game?>

(define (end-world? w)
  (end-game? (world-game w)))
;============================================================
;; Game -> Boolean
;; produces true when any invader landed (reaches the bottom of the screen)
;(define (end-game? g) false) ;stub

(check-expect (end-game? (make-game (list (make-invader 1 (- HEIGHT 1) 1)
                                          (make-invader 0 0 1))
                                    empty T0))
              false)
(check-expect (end-game? (make-game (list (make-invader 1 0 1)
                                          (make-invader 0 HEIGHT 1))
                                    empty T0))
              true)

(define (end-game? g)
  (invader-landed? (game-invaders g)))
;------------------------------------------------------------
;; ListOfInvader -> Boolean
;; produces true when any invader landed (reaches the bottom of the screen)
;(define (invader-landed? loinv) false) ;stub

(check-expect (invader-landed? empty) false)
(check-expect (invader-landed? (list (make-invader 1 (- HEIGHT 1) 1)
                               (make-invader 0 0 1)))
  false)
(check-expect (invader-landed? (list (make-invader 1 HEIGHT 1)
                               (make-invader 0 0 1)))
  true)

(define (invader-landed? loinv)
  (cond [(empty? loinv) false]
        [else
         (or (>= (invader-y (first loinv)) HEIGHT)
             (invader-landed? (rest loinv)))
         ]))
;============================================================
;; World KeyEvent -> World
;; handle keys for the current world
;(define (handle-key w ke) w) ;stub
;; Mapping from World to Game

;; <tests are provided at handle-game-key>

(define (handle-key w ke)
  (make-world (handle-game-key (world-game w) ke)
              (world-ticks w)))
;============================================================
;; Game KeyEvent -> Game
;; provide controller keys that changes the state of the game, where:
;;  - add missile at tank top when space key is pressed
;;  - change the direction of the tank to left when left arrow is pressed
;;  - change the direction of the tank to right when right arrow is pressed

;(define (handle-game-key g ke) g) ;stub

(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 1)) " ")
              (make-game (list I1) (list (make-missile 10 MISSILE-Y) M1)(make-tank 10 1)))

(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 1)) "left")
              (make-game (list I1) (list M1)(make-tank 10 -1)))
(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 -1)) "left")
              (make-game (list I1) (list M1)(make-tank 10 -1)))

(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 1)) "right")
              (make-game (list I1) (list M1)(make-tank 10 1)))
(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 -1)) "right")
              (make-game (list I1) (list M1)(make-tank 10 1)))

(check-expect (handle-game-key (make-game (list I1) (list M1)(make-tank 10 1)) "a")
              (make-game (list I1) (list M1)(make-tank 10 1)))


(define (handle-game-key g ke)
  (cond [(key=? ke " ") (make-game (game-invaders g)
                                   (cons (make-missile (tank-x (game-tank g)) MISSILE-Y)
                                         (game-missiles g))
                                   (game-tank g))]
        [(key=? ke "left") (make-game (game-invaders g)
                                      (game-missiles g)
                                      (make-tank (tank-x (game-tank g))
                                                 -1))]
        [(key=? ke "right") (make-game (game-invaders g)
                                       (game-missiles g)
                                       (make-tank (tank-x (game-tank g))
                                                  1))]
        [else g]))
;=======================================================[Play]=======================================================================

(main W0)
