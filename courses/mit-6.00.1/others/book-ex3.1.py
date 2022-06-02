#Name: Mahmoud Elnagar
#Date: 01/09/2021
#ex3.1 : 
# Write a program that asks the user to enter an integer and prints
# two integers, root and pwr, such that 0 < pwr < 6 and root**pwr is equal to the integer entered by the user.
# If no such pair of integers exists, it should print a message to that effect.

#using exhaustive enumeration
x = int(input("Enter Int> "))
print("root =",x, ",pwr =",1) #there is always one pair
for pwr in range(2,6):
    if(x < 0 and pwr%2 == 0): continue #skip even pwr for -ve values of x
    root = 0
    while root**pwr < abs(x): root+=1
    if(root**pwr == abs(x)): 
        if(x < 0): root = -root
        print("root =",root, ",pwr =",pwr)