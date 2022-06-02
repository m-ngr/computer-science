#Name: Mahmoud Elnagar
#Date: 27/08/2021
s = 'azcbobobegghakl' #don't provide this variable, it is provided by the grader.
max_sub = ""
substr = ""
for char in s:
    if len(substr) == 0 or char >= substr[-1]:
        substr += char
    else:
        if(len(substr) > len(max_sub)): max_sub = substr #check substr length when the sequence is broken
        substr = char
if(len(substr) > len(max_sub)): max_sub = substr #check last substr
print("Longest substring in alphabetical order is:", max_sub )