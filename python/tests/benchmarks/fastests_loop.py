

import timeit
import os
import sys
f = open(os.devnull, 'w')
old = sys.stdout
sys.stdout = f


s = """
l = []
for x in range(10):
    l.append(x)
"""


s2 = """
[x for x in range(10)]
"""

res1 = timeit.timeit(stmt=s, number=100000)
res2 = timeit.timeit(stmt=s2, number=100000)

sys.stdout = old
print(res1)
print(res2)
