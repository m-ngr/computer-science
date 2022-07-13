# Mahmoud Elnagar - 07/07/2022
require "./hw7.rb"
require "./hw7.extend.rb"

puts "==================================== NoPoints Tests ===================================="
np = NoPoints.new()
puts "Preprocess: " + pass(np.preprocess_prog  == np);
puts "Eval: " + pass(np.eval_prog([])  == np)
puts "Shift: " + pass(np.shift(1.1,2.2)  == np)

puts "Intersect-1: " + pass(np.intersect(NoPoints.new())  == np)
puts "Intersect-2: " + pass(np.intersect(Point.new(1.1, 2.2))  == np)
puts "Intersect-3: " + pass(np.intersect(Line.new(3.2,4.1))  == np)
puts "Intersect-4: " + pass(np.intersect(VerticalLine.new(3.2))  == np)
puts "Intersect-5: " + pass(np.intersect(LineSegment.new(1.1,2.2,3.3,4.4))  == np)

puts "====================================== Point Tests ======================================"
p = Point.new(1.1, 2.2)
puts "Preprocess: " + pass(p.preprocess_prog == p)
puts "Eval: " + pass(p.eval_prog([])  == p)
puts "Shift: " + pass(p.shift(1.1,2.2)  == Point.new(2.2, 4.4))

puts "Intersect-1: " + pass(p.intersect(NoPoints.new())  == NoPoints.new())
puts "Intersect-2: " + pass(p.intersect(p)  == p)
puts "Intersect-3: " + pass(p.intersect(Point.new(5,10))  == NoPoints.new())
puts "Intersect-4: " + pass(p.intersect(Line.new(1.0,1.1))  == p)
puts "Intersect-5: " + pass(p.intersect(Line.new(3.2,4.1))  == NoPoints.new())
puts "Intersect-6: " + pass(p.intersect(VerticalLine.new(1.1))  == p)
puts "Intersect-7: " + pass(p.intersect(VerticalLine.new(3.2))  == NoPoints.new())
puts "Intersect-8: " + pass(p.intersect(LineSegment.new(3.3,4.4,5.5,6.6))  == NoPoints.new())
puts "Intersect-9: " + pass(p.intersect(LineSegment.new(1.1,2.2,3.3,4.4))  == p)
puts "Intersect-10: " + pass(p.intersect(LineSegment.new(0.0,0.0,1.1,2.2))  == p)
puts "Intersect-11: " + pass(p.intersect(LineSegment.new(0.0,0.0,2.2,4.4))  == p)

puts "====================================== Line Tests ======================================"
ln = Line.new(3.2,4.1)

puts "Preprocess: " + pass(ln.preprocess_prog == ln)
puts "Eval: " + pass(ln.eval_prog([])  == ln)
puts "Shift: " + pass(ln.shift(1.1,2.2)  == Line.new(3.2, 2.78))

puts "Intersect-1: " + pass(ln.intersect(NoPoints.new())  == NoPoints.new())
puts "Intersect-2: " + pass(ln.intersect(Point.new(1.1,7.62))  == Point.new(1.1,7.62))
puts "Intersect-3: " + pass(ln.intersect(Point.new(5,10))  == NoPoints.new())
puts "Intersect-4: " + pass(ln.intersect(ln)  == ln)
puts "Intersect-5: " + pass(ln.intersect(Line.new(3.2,8.1))  == NoPoints.new())
puts "Intersect-6: " + pass(ln.intersect(Line.new(2.2,4.1))  == Point.new(0,4.1))
puts "Intersect-7: " + pass(ln.intersect(VerticalLine.new(0.8))  == Point.new(0.8,6.66))
puts "Intersect-8: " + pass(ln.intersect(LineSegment.new(1.1,2.2,3.3,4.4)) == NoPoints.new)
puts "Intersect-9: " + pass(ln.intersect(LineSegment.new(0,4.1,1,7.3)) == LineSegment.new(0,4.1,1,7.3))
puts "Intersect-10: " + pass(ln.intersect(LineSegment.new(-1,3.2,1,5)) == Point.new(0,4.1))

puts "================================== VerticalLine Tests ==================================="
vl = VerticalLine.new(3.2)
puts "Preprocess: " + pass(vl.preprocess_prog == vl)
puts "Eval: " + pass(vl.eval_prog([])  == vl)
puts "Shift: " + pass(vl.shift(1.1,2.2)  == VerticalLine.new(4.3))

puts "Intersect-1: " + pass(vl.intersect(NoPoints.new())  == NoPoints.new())
puts "Intersect-2: " + pass(vl.intersect(Point.new(3.2,2.1))  == Point.new(3.2,2.1))
puts "Intersect-3: " + pass(vl.intersect(Point.new(5,10))  == NoPoints.new())
puts "Intersect-4: " + pass(vl.intersect(Line.new(1.0,1.1))  == Point.new(3.2,4.3))
puts "Intersect-5: " + pass(vl.intersect(vl) == vl)
puts "Intersect-6: " + pass(vl.intersect(VerticalLine.new(0.8)) == NoPoints.new())
puts "Intersect-7: " + pass(vl.intersect(LineSegment.new(1.1,1.1,2.2,2.2)) == NoPoints.new)
puts "Intersect-8: " + pass(vl.intersect(LineSegment.new(1.1,2.2,5.5,6.6)) == Point.new(3.2, 4.3))
puts "Intersect-9: " + pass(vl.intersect(LineSegment.new(3.2,0,3.2,5)) == LineSegment.new(3.2,0,3.2,5))

puts "================================== LineSegment Tests ==================================="
ls = LineSegment.new(1.1,2.2,3.3,4.4)
vls = LineSegment.new(2,3,2,9)

puts "Preprocess-1: " + pass(LineSegment.new(3.2,4.1,3.2,4.1).preprocess_prog == Point.new(3.2,4.1))
puts "Preprocess-2: " + pass(LineSegment.new(2.1,4.1,3.2,4.1).preprocess_prog == 
                            LineSegment.new(2.1,4.1,3.2,4.1))
puts "Preprocess-3: " + pass(LineSegment.new(2.1,4.1,2.1,5.1).preprocess_prog == 
                            LineSegment.new(2.1,4.1,2.1,5.1))
puts "Preprocess-4: " + pass(LineSegment.new(5.1,4.1,3.2,8.1).preprocess_prog == 
                            LineSegment.new(3.2,8.1,5.1,4.1))
puts "Preprocess-5: " + pass(LineSegment.new(2.1,8.1,2.1,5.1).preprocess_prog == 
                            LineSegment.new(2.1,5.1,2.1,8.1))

puts "Eval: " + pass(ls.eval_prog([])  == ls)
puts "Shift: " + pass(ls.shift(1.1,2.2)  == LineSegment.new(2.2,4.4,4.4,6.6))

puts "Intersect-1: " + pass(ls.intersect(NoPoints.new)  == NoPoints.new)
puts "Intersect-2: " + pass(ls.intersect(Point.new(0,0))  == NoPoints.new)
puts "Intersect-3: " + pass(ls.intersect(Point.new(1.1,2.2))  == Point.new(1.1,2.2))
puts "Intersect-4: " + pass(ls.intersect(Point.new(3.3,4.4))  == Point.new(3.3,4.4))
puts "Intersect-5: " + pass(ls.intersect(Point.new(2.2,3.3))  == Point.new(2.2,3.3))
puts "Intersect-6: " + pass(ls.intersect(Line.new(1.0,8.1))  == NoPoints.new)
puts "Intersect-7: " + pass(ls.intersect(Line.new(1.0,1.1))  == ls)
puts "Intersect-8: " + pass(ls.intersect(Line.new(-1,4.5))  == Point.new(1.7,2.8))
puts "Intersect-9: " + pass(ls.intersect(VerticalLine.new(0))  == NoPoints.new)
puts "Intersect-10: " + pass(ls.intersect(VerticalLine.new(2))  == Point.new(2, 3.1))
puts "Intersect-11: " + pass(vls.intersect(VerticalLine.new(2))  == vls)
puts "Intersect-12: " + pass(ls.intersect(ls) == ls)
puts "Intersect-13: " + pass(ls.intersect(LineSegment.new(1,1,3,3)) == NoPoints.new)
puts "Intersect-14: " + pass(ls.intersect(LineSegment.new(1.5,2,2.5,4)) == Point.new(2.1,3.2))

puts "Intersect-15: " + pass(ls.intersect(LineSegment.new(0,1.1,1,2.1)) == NoPoints.new)
puts "Intersect-16: " + pass(ls.intersect(LineSegment.new(4,5.1,6,7.1)) == NoPoints.new)

puts "Intersect-17: " + pass(ls.intersect(LineSegment.new(0,1.1,1.1,2.2)) == Point.new(1.1,2.2))
puts "Intersect-18: " + pass(ls.intersect(LineSegment.new(3.3,4.4,6,7.1)) == Point.new(3.3,4.4))

puts "Intersect-19: " + pass(ls.intersect(LineSegment.new(0,1.1,2,3.1)) == 
                                          LineSegment.new(1.1,2.2,2,3.1))
;
puts "Intersect-20: " + pass(ls.intersect(LineSegment.new(2,3.1,6,7.1)) == 
                                          LineSegment.new(2,3.1,3.3,4.4))
;
puts "Intersect-21: " + pass(ls.intersect(LineSegment.new(2,3.1,3,4.1)) == 
                                          LineSegment.new(2,3.1,3,4.1))
;

puts "Intersect-22: " + pass(ls.intersect(LineSegment.new(0,1.1,6,7.1)) == 
                                          LineSegment.new(1.1,2.2,3.3,4.4))
;

puts "================================== Var Tests ========================================="
puts "Preprocess: " + pass(Var.new("x").preprocess_prog == Var.new("x"))
puts "Eval: " + pass(Var.new("x").eval_prog([["x", NoPoints.new()]]) == NoPoints.new())

puts "================================== Let Tests ========================================="
lt = Let.new("x", LineSegment.new(3.2,4.1,3.2,4.1), LineSegment.new(2.1,8.1,2.1,5.1))
lt_proc = Let.new("x", Point.new(3.2,4.1), LineSegment.new(2.1,5.1,2.1,8.1))
lt_var = Let.new("x", Point.new(3.2,4.1), Var.new("x"))

puts "Preprocess: " + pass(lt.preprocess_prog == lt_proc)
puts "Eval-1: " + pass(lt_proc.eval_prog([]) == LineSegment.new(2.1,5.1,2.1,8.1));
puts "Eval-2: " + pass(lt_var.eval_prog([]) == Point.new(3.2,4.1))
puts "Eval-3: " + pass(lt_var.eval_prog([["x", NoPoints.new()]]) == Point.new(3.2,4.1))

puts "================================== Shift Tests ======================================="
sh = Shift.new(1.1, 2.2, LineSegment.new(3.2,4.1,3.2,4.1))
sh_proc = Shift.new(1.1, 2.2, Point.new(3.2,4.1))

puts "Preprocess: " + pass(sh.preprocess_prog == sh_proc)
puts "Eval-1: " + pass(Shift.new(1.1,2.2, NoPoints.new).eval_prog([]) == NoPoints.new())
puts "Eval-2: " + pass(Shift.new(1.1,2.2, Point.new(3.2,4.1)).eval_prog([]) == 
                        Point.new(4.3, 6.3))
puts "Eval-3: " + pass(Shift.new(1.1,2.2, Line.new(3.2,4.1)).eval_prog([]) == 
                        Line.new(3.2, 2.78))
puts "Eval-4: " + pass(Shift.new(1.1,2.2, VerticalLine.new(3.2)).eval_prog([]) == 
                        VerticalLine.new(4.3))
puts "Eval-5: " + pass(Shift.new(1.1,2.2, LineSegment.new(1.1,2.2,3.3,4.4)).eval_prog([])  == 
                        LineSegment.new(2.2,4.4,4.4,6.6))


puts "================================== Intersect Tests ==================================="
ins = Intersect.new(LineSegment.new(3.2,4.1,3.2,4.1), LineSegment.new(5.1,4.1,3.2,8.1))
ins_proc = Intersect.new(Point.new(3.2,4.1), LineSegment.new(3.2,8.1,5.1,4.1))

puts "Preprocess: " + pass(ins.preprocess_prog == ins_proc)
puts "Eval-1: " + pass(Intersect.new(np,np).eval_prog([]) == np)
puts "Eval-2: " + pass(Intersect.new(p,p).eval_prog([]) == p)
puts "Eval-3: " + pass(Intersect.new(ln,ln).eval_prog([]) == ln)
puts "Eval-4: " + pass(Intersect.new(vl,vl).eval_prog([]) == vl)
puts "Eval-5: " + pass(Intersect.new(ls,ls).eval_prog([]) == ls)