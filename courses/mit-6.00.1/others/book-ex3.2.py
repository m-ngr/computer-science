#Name: Mahmoud Elnagar
#Date: 01/09/2021
#ex3.2 : 
# Let s be a string that contains a sequence of decimal numbers
# separated by commas, e.g., s = '1.23,2.4,3.123'. Write a program that prints the
# sum of the numbers in s.

s = '1.23,2.4,3.123,'
sum = 0.0
for num in s.split(","): 
    try:
        sum += float(num)
    except:
        sum
print(sum)