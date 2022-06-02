#Name: Mahmoud Elnagar
#Date: 04/09/2021
#Problem 1 - Paying Debt off in a Year

#================ provided by grader ============
balance = 484
annualInterestRate = 0.2
monthlyPaymentRate = 0.04
#=============== Solution =======================
for month in range(12):
    balance -= balance * monthlyPaymentRate
    balance += balance * annualInterestRate /12
print("Remaining balance:",round(balance,2))
