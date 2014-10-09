import math
import sys

procs = int(sys.argv[1])

m = int(math.log(procs, 2))
sender = 0
receiver = 0

for i in range(1, m + 1):
    for rank in range(procs):
        sender = 0
        receiver = 0
        divider = pow(2, i)
        offset = divider / 2

        if (rank % divider == 0):
            receiver = 1
            partner = rank + offset
        elif (rank % divider == offset):
            sender = 1
            partner = rank - offset

        if sender:
            print rank, "->", partner
        elif receiver:
            print rank, "<-", partner
    print
