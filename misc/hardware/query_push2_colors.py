#!/usr/bin/env python3

import json
import mido

fromPush = mido.open_input('Ableton Push 2 User Port')
toPush = mido.open_output('Ableton Push 2 User Port')

colorList = list()
whiteList = list()

for index in range(128):

    request = mido.Message('sysex', data=[0x00, 0x21, 0x1D, 0x01, 0x01,  0x04, index])
    toPush.send(request)

    msg = fromPush.receive().bytes()
    if index != msg[7]:
        continue

    payload = msg[8:-1]
    red = payload[0] + (128 * payload[1])
    green = payload[2] + (128 * payload[3])
    blue = payload[4] + (128 * payload[5])
    white = payload[6] + (128 * payload[7])

    color = f'{red:0>2x}{green:0>2x}{blue:0>2x}'
    colorList.append(color)

    whiteList.append(white)

    print(index, color, white)


fromPush.close()
toPush.close()

with open('../../patchers/hardware/push2_colors.json', 'w') as outfile:
    json.dump(colorList, outfile, indent=3)

with open('../../patchers/hardware/push2_whites.json', 'w') as outfile:
    json.dump(whiteList, outfile, indent=3)
