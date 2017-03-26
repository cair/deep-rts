

width = 30
height = 30

big_list = []


for x in range(height):
    for y in range(width):
        big_list.append((x,y))

big_set = set(big_list)


little_list = [(2,2), (5,2), (3,1), (18,19)]
little_set = set(little_list)

import time
start_time = time.time()
start_time = time.time()
for i in range(100000):
    ee = set(big_list) - set(little_list)

print("Set: --- %s seconds ---" % (time.time() - start_time))

start_time = time.time()
start_time = time.time()
for i in range(100000):
    ee = big_set - little_set

print("SetPre: --- %s seconds ---" % (time.time() - start_time))

start_time = time.time()
start_time = time.time()

for i in range(100000):
    ss = [x for x in big_list if x not in little_list]

print("List: --- %s seconds ---" % (time.time() - start_time))
