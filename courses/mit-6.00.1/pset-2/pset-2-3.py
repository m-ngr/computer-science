#Name: Mahmoud Elnagar
#Date: 04/09/2021
#Problem 3 - Using Bisection Search to Make the Program Faster
#================= provided by the grader ================
balance = 999999
annualInterestRate = 0.18
#===================== Solution ==========================
def last_month_balance(balance, payment):
    for month in range(11):
        balance -= payment
        balance += balance*annualInterestRate/12
    return balance

high = (balance*(1+(annualInterestRate/12))**12)/12
low = balance/12
EPS = 0.0000001
MIN_DIFF = 0.01 #minimum payment 0.01 (1 cent)
while high - low > EPS:
    payment = (low + high)/2
    last_payment = last_month_balance(balance,payment)
    if(abs(last_payment-payment) < MIN_DIFF):
        print("Lowest Payment:",round(payment,2))
        break
    if(last_payment > payment): low = payment
    else: high = payment
