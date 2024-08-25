#!/usr/bin/env python3


import json

with open('../../patchers/hardware/launchpad_colors.json', 'r') as infile:
    data = json.load(infile)

luastring = 'launchpad_colors = {\n'
for index, color in enumerate(data):
    red = int(color[0:2], 16)
    green = int(color[2:4], 16)
    blue = int(color[4:6], 16)
    luastring += f'{{ red = {red}, green = {green}, blue = {blue} }},\n'

luastring += '}'

print(luastring)
