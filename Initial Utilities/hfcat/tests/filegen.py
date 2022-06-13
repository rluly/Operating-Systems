#! /usr/bin/env python3

import random
import string

for i in range(1000000):
    x = ''
    for i in range(40):
        x += random.choice(string.ascii_letters + ' ')
    print(x)
