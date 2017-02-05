import numpy as np

n = 100
dist = [np.random.random() for x in range(n) ]
counter = [0 for x in range(n)]
it_debug = [0 for x in range(n*2)]
print("Lets start!")



def selection(distribution):
    while True:
        index = int(n * np.random.uniform())
        if np.random.uniform() < distribution[index] / np.sum(distribution):
            break
    return index

# Test algorithm
for x in range(500):
    selection(dist)

print(counter)
print("Sum: %s" % np.sum(counter))

