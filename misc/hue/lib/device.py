import colorsys

# https://developers.meethue.com/develop/application-design-guidance/color-conversion-formulas-rgb-to-xy-and-back/#xy-to-rgb-color
# https://developers.meethue.com/develop/hue-api-v2/api-reference/#resource_light_get


class Device:

   def __init__(self, bridge, deviceId):

      self.bridge = bridge
      self.deviceId = deviceId

   def isOn(self):

      state = self.bridge.getState(self.deviceId)

      on = state['on']
      if on:
         return 1
      else:
         return 0

   def swtich(self, on):

      payload = {"on": {"on": on}}
      self.bridge.setState(self.deviceId, payload)

   def _compileHSV(self, hexColor):

      # sat (uint8)
      # hue (int16)
      # bri (uint8)

      red = int(hexColor[0:2], 16) / 255
      green = int(hexColor[2:4], 16) / 255
      blue = int(hexColor[4:6], 16) / 255

      print('RGB', int(red * 255), int(green * 255), int(blue * 255))

      [hue, sat, bright] = colorsys.rgb_to_hsv(red, green, blue)

      hue = int(hue * 65535)
      sat = int(sat * 255)
      bright = int(bright * 255)

      print('HSL', hue, sat, bright)

      return [hue, sat, bright]

   def setColor(self, hexColor):

      [hue, sat, _] = self._compileHSV(hexColor)

      payload = {"hue": hue, "sat": sat, "transitiontime": 0, "alert": "none"}
      self.bridge.setState(self.deviceId, payload)

   def setColorBrightness(self, hexColor):

      [hue, sat, bright] = self._compileHSV(hexColor)

      payload = {"hue": hue, "sat": sat, "bri": bright, "transitiontime": 0, "alert": "none"}
      self.bridge.setState(self.deviceId, payload)

   def setBrightness(self, value):

      payload = {"bri": int(value), "transitiontime": 0, "alert": "none"}
      self.bridge.setState(self.deviceId, payload)
