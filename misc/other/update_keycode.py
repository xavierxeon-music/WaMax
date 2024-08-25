#!/usr/bin/env python3

import sys
import json
import string

keyList = list(string.ascii_lowercase)
for number in range(10):
    keyList.append(str(number))
keyList.append(' ')

keyDict = dict()

for key in keyList:
    code = ord(key)
    if ' ' == key:
        key = 'space'
    keyDict[key] = code

with open('../../patchers/_other/keycode.json', 'w') as outfile:
    json.dump(keyDict, outfile, indent=3)
