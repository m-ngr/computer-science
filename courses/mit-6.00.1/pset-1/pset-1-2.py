#Name: Mahmoud Elnagar
#Date: 27/08/2021
s = 'azcbobobegghakl' #don't provide this variable, it is provided by the grader.
count = 0
for i in range(len(s)-2):
    if(s[i:i+3]=="bob"): count+=1

print("Number of times bob occurs is:",count)