#! /usr/bin/env python

from __future__ import print_function, division

def count_sort(a):
    temp = a[:]

    for i, ai in enumerate(a):
        count = 0
        for j, aj in enumerate(a):
            if aj < ai:
                count += 1
            elif (aj == ai) and (j < i):
                count += 1
        temp[count] = ai

    return temp


if __name__ == "__main__":
    tests = [
      [0,1,2,3],
      [3,2,1,0],
      [3,1,2,0],
    ]

    for a in tests:
        a_sort = count_sort(a)
        print(a_sort == list(sorted(a)))
