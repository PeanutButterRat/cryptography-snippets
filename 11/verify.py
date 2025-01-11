# verify.py by Eric Brown for CSC 152 Fall 2024.
# Programmed by myself.

# Resources used:
# https://www.geeksforgeeks.org/random-getrandbits-in-python/
# https://stackoverflow.com/questions/4798654/modular-multiplicative-inverse-function-in-python

import sys


def main():
    if len(sys.argv) != 7:
        print('usage: python sign.py <p> <g> <gd> <x> <r> <s>')
        sys.exit(1)
    
    p, g, gd, x, r, s = map(int, sys.argv[1:])
    matches = verify(p, g, gd, x, r, s)

    print(1 if matches else 0)


def verify(p, g, gd, x, r, s):
    left = (pow(gd, r, p) * pow(r, s, p)) % p
    right = pow(g, x, p)

    return left == right


if __name__ == '__main__':
    main()
