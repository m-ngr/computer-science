#NAME: Mahmoud Elnagar
#DATE: 01/11/2021
# 6.00.2x Problem Set 1: Space Cows 

from ps1_partition import get_partitions
import time

#================================
# Part A: Transporting Space Cows
#================================

def load_cows(filename):
    """
    Read the contents of the given file.  Assumes the file contents contain
    data in the form of comma-separated cow name, weight pairs, and return a
    dictionary containing cow names as keys and corresponding weights as values.

    Parameters:
    filename - the name of the data file as a string

    Returns:
    a dictionary of cow name (string), weight (int) pairs
    """

    cow_dict = dict()

    f = open(filename, 'r')
    
    for line in f:
        line_data = line.split(',')
        cow_dict[line_data[0]] = int(line_data[1])
    return cow_dict


# Problem 1
def greedy_cow_transport(cows,limit=10):
    """
    Uses a greedy heuristic to determine an allocation of cows that attempts to
    minimize the number of spaceship trips needed to transport all the cows. The
    returned allocation of cows may or may not be optimal.
    The greedy heuristic should follow the following method:

    1. As long as the current trip can fit another cow, add the largest cow that will fit
        to the trip
    2. Once the trip is full, begin a new trip to transport the remaining cows

    Does not mutate the given dictionary of cows.

    Parameters:
    cows - a dictionary of name (string), weight (int) pairs
    limit - weight limit of the spaceship (an int)
    
    Returns:
    A list of lists, with each inner list containing the names of cows
    transported on a particular trip and the overall list containing all the
    trips
    """
    trips = []
    sorted_cows = []
    for name, weight in cows.items(): sorted_cows.append((weight,name))
    sorted_cows.sort(reverse=True)
    while len(sorted_cows) != 0:
        left_cows = []
        current_trip = []
        current_weight = 0
        for weight, name in sorted_cows:
            if weight <= (limit-current_weight):
                current_trip.append(name)
                current_weight += weight
            else:
                left_cows.append((weight,name))
        trips.append(current_trip)
        sorted_cows = left_cows
    return trips    



# Problem 2
def brute_force_cow_transport(cows,limit=10):
    """
    Finds the allocation of cows that minimizes the number of spaceship trips
    via brute force.  The brute force algorithm should follow the following method:

    1. Enumerate all possible ways that the cows can be divided into separate trips
    2. Select the allocation that minimizes the number of trips without making any trip
        that does not obey the weight limitation
            
    Does not mutate the given dictionary of cows.

    Parameters:
    cows - a dictionary of name (string), weight (int) pairs
    limit - weight limit of the spaceship (an int)
    
    Returns:
    A list of lists, with each inner list containing the names of cows
    transported on a particular trip and the overall list containing all the
    trips
    """
    result = None
    for trips in get_partitions(cows):
        valid_trips = True
        for trip in trips:
            current_weight = 0
            for cow in trip: current_weight += cows[cow]
            if current_weight > limit: 
                valid_trips = False
                break
        if valid_trips and (result == None or len(result) > len(trips)) :
            result = trips
    return result




        
# Problem 3
def compare_cow_transport_algorithms(cows):
    """
    Using the data from ps1_cow_data.txt and the specified weight limit, run your
    greedy_cow_transport and brute_force_cow_transport functions here. Use the
    default weight limits of 10 for both greedy_cow_transport and
    brute_force_cow_transport.
    
    Print out the number of trips returned by each method, and how long each
    method takes to run in seconds.

    Returns:
    Does not return anything.
    """
    print("Greedy Algorithm test:")
    start = time.time()
    print(greedy_cow_transport(cows))
    end = time.time()
    print("Greedy Time:",(end - start))

    print("Brute Force test:")
    start = time.time()
    print(brute_force_cow_transport(cows))
    end = time.time()
    print("Brute Time:",(end - start))

"""
Here is some test data for you to see the results of your algorithms with. 
Do not submit this along with any of your answers. Uncomment the last two
lines to print the result of your problem.
"""

cows = load_cows("ps1_cow_data.txt")
compare_cow_transport_algorithms(cows)