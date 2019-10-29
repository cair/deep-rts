
import timeit

min = 0
max = 1000

res = timeit.timeit(f"import random; int({max} * random.random()) + {min}", number=1000000)
res2 = timeit.timeit(f"import random; random.randint({min}, {max})", number=1000000)

print(f"Method 1: {res}")
print(f"Method 2: {res2}")
