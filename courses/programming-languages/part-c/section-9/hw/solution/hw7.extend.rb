# Mahmoud Elnagar - 07/07/2022

# this file extent classes in hw7.rb and the standard class Float for testing purposes

class Float
  def == x
    (self - x).abs <  0.00001
  end
end

class NoPoints
  def == p
    p.is_a? NoPoints
  end
end

class Point
  def == p
    p.is_a? Point and x == p.x and y == p.y
  end
end

class Line
  def == p
    p.is_a? Line and m == p.m and b == p.b
  end
end

class VerticalLine
  def == p
    p.is_a? VerticalLine and x == p.x
  end
end

class LineSegment
  def == p
    p.is_a? LineSegment and x1 == p.x1 and y1 == p.y1 and x2 == p.x2 and y2 == p.y2
  end
end

class Var
  def s
    @s
  end 
  def == p
    p.is_a? Var and @s == p.s
  end
end

class Let
  def s
    @s
  end
  def e1
    @e1
  end
  def e2
    @e2
  end

  def == p
    p.is_a? Let and @s == p.s and @e1 == p.e1 and @e2 == p.e2
  end
end

class Shift
  def dx
    @dx
  end
  def dy
    @dy
  end
  def e
    @e
  end
  def == p
    p.is_a? Shift and @dx == p.dx and @dy == p.dy and @e == p.e
  end
end

class Intersect
  def e1
    @e1
  end
  def e2
    @e2
  end
  def == p
    p.is_a? Intersect and @e1 == p.e1 and @e2 == p.e2
  end
end

def pass x
  if x then "Passed" else "Failed" end
end