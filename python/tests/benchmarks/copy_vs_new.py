

n = 5000
predefined = [1 for x in range(n)]

ite = 50000
import time


start = time.time()
start = time.time()
for _ in range(ite):
    x = predefined.copy()
    for idx, y in enumerate(x):
        x[idx] += 1
end = time.time()
print("Copy Took: %s" %  (end - start))

start = time.time()
start = time.time()
for _ in range(ite):
    x = [1 for x in range(n)]
    for idx, y in enumerate(x):
        x[idx] += 1
end = time.time()
print("New Took: %s" %  (end - start))

"""
Copy Took: 7.03062891960144
New Took: 8.617604494094849
"""