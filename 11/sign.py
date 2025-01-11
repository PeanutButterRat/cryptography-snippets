# sign.py by Eric Brown for CSC 152 Fall 2024.
# Programmed by myself.

# Resources used:
# https://www.geeksforgeeks.org/random-getrandbits-in-python/
# https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python

import sys
import random
import math


def main():
    if len(sys.argv) != 6:
        print('usage: python sign.py <p> <g> <gd> <d> <x>')
        sys.exit(1)
    
    p, g, gd, d, x = map(int, sys.argv[1:])
    r, s = sign(p, g, gd, d, x)

    print(r)
    print(s)


def sign(p, g, gd, d, x):
    # Choose random 0 < e < p.
    e = 0
    bits = len(bin(p)) - 2  # Don't count the '0b'.

    while not (0 < e < p and math.gcd(e, p - 1) == 1):
        e = random.getrandbits(bits)

    # Calculate r and s.
    r = pow(g, e, p)
    s = ((x - d * r) * pow(e, -1, p - 1)) % (p - 1)

    return r, s


if __name__ == '__main__':
    main()
