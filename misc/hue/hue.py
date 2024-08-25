#!/usr/bin/env python3

import sys

from lib import Bridge


def main():

   if len(sys.argv) > 1:
      command = sys.argv[1]
   else:
      command = None

   if len(sys.argv) > 2:
      deviceName = sys.argv[2]
   else:
      deviceName = None

   if len(sys.argv) > 3:
      parameter = sys.argv[3]
   else:
      parameter = None

   bridge = Bridge()
   device = bridge.getDeviceId(deviceName)

   commandMap = {'list': [bridge.listDevices, deviceName, 'list devices']}
   commandMap['state'] = [device.isOn if device else None, None, 'check id device is on or off']
   commandMap['on'] = [device.swtich if device else None, True, 'turn device on']
   commandMap['off'] = [device.swtich if device else None, False, 'turn device off']
   commandMap['color'] = [device.setColor if device else None, parameter, 'set color, ignore brightness, PARAM hexColor']
   commandMap['bright'] = [device.setBrightness if device else None, parameter, 'set brightness, PARAM hexColor']
   commandMap['colorbright'] = [device.setColorBrightness if device else None, parameter, 'set color and brightness, PARAM brightness (uchar)']

   if not command in commandMap:
      print('hue.py COMMAND DEVICE PARAM\n')
      for name, item in commandMap.items():
         help = item[2]
         print(name, ': ', help)
      sys.exit(1)
   else:
      function = commandMap[command][0]
      if not function:
         print('dervice error')
         sys.exit(1)
      fp = commandMap[command][1]
      if None == fp:
         function()
      else:
         function(fp)


if __name__ == '__main__':
   main()
