# ================================ Classes ==========================
class A
  def m1
    34
  end

  def m2 (x,y)
    z = 7
    if x > y
      false
    else
      x + y * z
    end
  end

  def m3 x
    x.abs * 2 + self.m1
  end
  
  def m4 x
    puts x
    self # returns the current object
  end

end
# Objects
a = A.new # Make a new Object
puts a.m1 # Call method m1
puts a.m2(2,3) 
puts a.m3(2) 
puts a.m4("Hello").m4("World").m1 #call concat
# ====================== More Class Features ===========================
class A

  Const_C1 = 40 #Const Public Field

  def initialize(a=0)
    @obj_a = a
    @@shared_b = 0
  end

  def a
    @obj_a
  end

  def a= x
    @obj_a = x
  end

  def b
    @@shared_b
  end

  def b= x
    @@shared_b = x
  end
  #Same Access Modifiers as C++
public
protected
private
end

def show (name, obj)
  print "Obj-",name, " : A = " , obj.a , "\tB = " ,  obj.b , "\n"
end

def sets (obj, valA, valB)
  obj.a = valA
  obj.b = valB
end

a = A.new
b = A.new(2)

show("A", a)
show("B", b)

sets(a, 10, 15)

show("A", a)
show("B", b)

sets(b, 20, 25)

show("A", a)
show("B", b)
# ============================= Blocks & Procs ============================
#Blocks: are second-class closure-like. Methods can only receive one block
#{puts "Hi"} #block with no arguments
#{|x| puts (x*2) } #block with one argument
#{|x,y| puts (x+y)} #block with two arguments

#Procs: are first-class blocks. they are objects with a call method
c1 = lambda {|x| x * 2} #creating a Proc
puts c1.call(2) # Calling a Proc
# ================================ Collections ==============================
# Hashes
h1 = {"x"=>"Hello","y"=>[1,2,3],0=>{:x => "x"}}
puts h1["x"]
puts h1["z"]
puts h1["y"] = [4,5,6]
puts h1["z"] = "new Field"
# Ranges
r1 = (0..9)
# Dynamic Programming with duck-typing
def foo a
  a.count {|x| x > 0}
end
# foo is a function that accepts an object that has method called count
# the count method takes only a block
# this will work fine for arrays, ranges,..., any data structures that implements a count method (with an appropriate semantics)
# This is a greate way for seperation of concerns, data structures know how to iterate over data, and users know how to process the data
#=================================== sub-classing ===========================