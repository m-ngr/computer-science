#Name: Mahmoud Elnagar
#Date: 15/10/2021
# Solving the knapsack problem using 3 algorithms
#   1- Brute Force
#   2- Greedy Algorithm
#   3- Dynamic Programming

class Item(object):
    def __init__(self,name,value,cost) -> None:
        self.name = name
        self.value = value
        self.cost = cost
    def getValue(self): return self.value
    def getCost(self): return self.cost
    def getRatio(self): return self.getValue()/self.getCost()
    def __str__(self) -> str:
        return str(self.name)
    def __repr__(self) -> str:
        return str(self.name)
#Greedy Algorithm
def greed(items:list,greedKey,maxCost):
    sorted_items = sorted(items, key = greedKey, reverse=True)
    cost, value = 0,0
    selected_items = []
    for item in sorted_items:
        if maxCost - cost >= item.getCost():
            cost += item.getCost()
            value += item.getValue()
            selected_items.append(item)
    return value,selected_items
#Brute Force Algorithm
def getMax(lst:list,maxCost:int):
    if len(lst) == 0 or maxCost == 0: return (0,[])
    if lst[0].getCost()> maxCost: # we can't take it
        return getMax(lst[1:],maxCost)
    else:
        takeValue, takeItems = getMax(lst[1:],maxCost - lst[0].getCost())
        takeValue += lst[0].getValue()
        noTakeValue, noTakeItems = getMax(lst[1:],maxCost)
        if(takeValue > noTakeValue):
            return (takeValue,[lst[0]]+takeItems)
        else:
            return (noTakeValue,noTakeItems)
#Dynamic Programming Algorithm
def fastGetMax(lst:list,maxCost:int, memo:dict = {}):
    if len(lst) == 0 or maxCost == 0: return (0,[])
    memKey = (len(lst),maxCost)
    if memKey in memo.keys(): return memo[memKey]

    if lst[0].getCost()> maxCost: # we can't take it
        memo[memKey] = fastGetMax(lst[1:],maxCost,memo)
    else:
        takeValue, takeItems = fastGetMax(lst[1:],maxCost - lst[0].getCost(),memo)
        takeValue += lst[0].getValue()
        noTakeValue, noTakeItems = fastGetMax(lst[1:],maxCost,memo)
        if(takeValue > noTakeValue):
            memo[memKey] = (takeValue,[lst[0]]+takeItems)
        else:
            memo[memKey] = (noTakeValue,noTakeItems)
    return memo[memKey]
import random
#List Generator
def genList(length:int, maxValue:int, maxCost:int):
    res = []
    for i in range(length):
        res.append(Item(str(i), random.randint(1,maxValue), random.randint(1,maxCost)))
    return res
#test list on different algorithms
def test(lst:list, maxCost:int):
    #Greed on Value
    print("Running Greedy Algorithm on value :",end = " ")
    val,sel = greed(lst, Item.getValue, maxCost)
    print("value =",val,", From:",sel)
    #Greed on Cost
    print("Running Greedy Algorithm on cost :",end = " ")
    val,sel = greed(lst, lambda x: 1/Item.getCost(x), maxCost)
    print("value =",val,", From:",sel)
    #Greed on Ratio
    print("Running Greedy Algorithm on ratio :",end = " ")
    val,sel = greed(lst, Item.getRatio, maxCost)
    print("value =",val,", From:",sel)
    #Dynamic Programming
    print("Running Dynamic Programming Algorithm :",end = " ")
    val,sel = fastGetMax(lst,maxCost,{})
    print("value =",val,", From:",sel)
    #Brute Force
    print("Running Brute Force Algorithm :",end = " ")
    val,sel = getMax(lst,maxCost)
    print("value =",val,", From:",sel)
#===================================== main ===============================
#small manual list test
lst = [Item("clock",175,10),Item("picture",90,9),Item("radio",20,4),Item("vase",50,2),
Item("book",10,1),Item("computer",200,20)]
test(lst,20)
#large random list test
test(genList(80,500,100),100)
