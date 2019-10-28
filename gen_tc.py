#!/usr/bin/python3

import random
import math
random.seed()

bitsize = [int(math.pow(2, i)) for i in range(3, 8)]

NUM_BN = 3
bn = [[] for i in range(NUM_BN)]


for size in bitsize:
    for i in range(NUM_BN):
        n = random.getrandbits(size)
        bn[i].append(n)
