#Name: Mahmoud Elnagar
#Date: 04/09/2021
#Problem 2 - Paying Debt Off in a Year
#================= provided by the grader ================
balance = 348
annualInterestRate = 0.25
#===================== Solution ==========================
def last_month_balance(balance, payment):
    for month in range(11):
        balance -= payment
        balance += balance*annualInterestRate/12
    return balance

high = balance
low = 0
lowest = int(balance)
while high - low > 10:
    payment = (low + high)/2
    payment =  payment//10 * 10 #convert to multiple of tens
    last_payment = last_month_balance(balance,payment)

    if(last_payment > payment): low = payment
    else: 
        lowest = min(lowest, payment)
        high = lowest
print("Lowest Payment:",int(lowest))