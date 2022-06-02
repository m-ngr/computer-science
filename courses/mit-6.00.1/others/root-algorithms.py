#Name: Mahmoud Elnagar
#Date: 02/09/2021
def rootEE(n, value):
    neg_flag = False
    if(value < 0):
        if(n%2 == 0) : 
            print("No",str(n)+"th","root for",value)
            return
        value = -value
        neg_flag = True
    #setup variable
    EPS = .001
    STEP = .0001
    limit = value
    if(value < 1): 
        EPS = .000001
        STEP = .000001
        limit = 1
    ans = 0.0
    loops = 0
    while abs(ans**n-value) > EPS and ans < limit:
        ans+=STEP
        loops+=1
    #Show Results    
    print("loops =",loops)
    if(neg_flag):
        value = -value
        ans = -ans
    if(abs(ans**n-value) > EPS):
        print("Failed to get",str(n)+"th","root of",value)
    else:
        print("root =",ans)

def rootBI(n, value):
    neg_flag = False
    if(value < 0):
        if(n%2 == 0) : 
            print("No",str(n)+"th","root for",value)
            return
        value = -value
        neg_flag = True
    high = value
    low = 1.0
    EPS = 0.00000000001
    if(value < 1):
        high = 1.0
        low = value
    ans = (low+high)/2
    loops = 0
    while abs(ans**n-value) > EPS :
        if(ans**n > value): high = ans
        elif(value > ans**n): low = ans
        ans = (low+high)/2
        loops+=1
    print("loops =",loops)
    if(neg_flag):
        value = -value
        ans = -ans
    if(abs(ans**n-value) > EPS):
        print("Failed to get",str(n)+"th","root of",value)
    else:
        print("root =",ans)

def rootNR(n,value):
    neg_flag = False
    if(value < 0):
        if(n%2 == 0) : 
            print("No",str(n)+"th","root for",value)
            return
        value = -value
        neg_flag = True
    def p(x,n,value): return ((x**n) - value)
    def dp(x,n): return (n*(x**(n-1)))
    ans = value/2
    EPS = 0.00000000001
    loops = 0
    while abs(p(ans,n,value)) > EPS: 
        ans = ans - (p(ans,n,value)/dp(ans, n))
        loops+=1
    print("loops =",loops)
    if(neg_flag):
        value = -value
        ans = -ans
    if(abs(ans**n-value) > EPS):
        print("Failed to get",str(n)+"th","root of",value)
    else:
        print("root =",ans)

while True:
    n = int(input("n = "))
    value = float(input("value = "))
    print("Using Exhaustive Enumeration","With precision of 0.001")
    rootEE(n,value)
    print("Using Binary Search","With precision of 0.00000000001")
    rootBI(n,value)
    print("Using Newton Raphson","With precision of 0.00000000001")
    rootNR(n,value)
    print("======================================================")
