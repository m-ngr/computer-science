;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname Test) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f () #f)))
;String tutorial
(string-append "mahmoud" " " "elnagar")
(string-length "car")
;substring from index 0 upto but not including 3 => index[0,1,2]
(substring "manners" 0 3)
;Images tutorial
(require 2htdp/image);I think: Include image library from 2htdp
(circle 10 "solid" "red"); (circle radius fillType color)
(rectangle 20 30 "outline" "green"); (rectangle width height fillType color)
(text "Hello World" 24 "orange"); (text string fontSize color)
; put images above each other
(above (circle 10 "solid" "red")
       (circle 20 "solid" "yellow")
       (circle 30 "solid" "green"))
; put images beside each other
(beside (circle 10 "solid" "red")
        (circle 20 "solid" "yellow")
        (circle 30 "solid" "green"))
; overlay images on each other
(overlay (circle 10 "solid" "red")
         (circle 20 "solid" "yellow")
         (circle 30 "solid" "green"))