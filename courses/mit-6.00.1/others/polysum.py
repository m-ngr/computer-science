#Name: Mahmoud Elnagar
#Date: 03/09/2021
import math
def polysum(n,s):
    '''
    Input: 
    - n (int) => number of sides
    - s (int/float) => length of each side

    Output: float => sum the area and square of the perimeter of the regular polygon.
    '''
    def area(n,s): return(0.25*n*s*s)/math.tan(math.pi / n)
    def perimeter(n,s): return n*s
    return round(area(n,s) + (perimeter(n,s)**2),4)
#========================= main ===================================================
print(polysum(5,4.5))