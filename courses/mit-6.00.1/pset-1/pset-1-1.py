#Name: Mahmoud Elnagar
#Date: 27/08/2021
s = 'azcbobobegghakl' #don't provide this variable, it is provided by the grader.
vowel_num = 0
for char in s:
    if(char == "a" or char =="e"or char =="i"or char =="o"or char =="u"):
        vowel_num+=1
print("Number of vowels:",vowel_num)