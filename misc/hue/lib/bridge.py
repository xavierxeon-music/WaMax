#

import json
import os
import requests
import sys
import urllib3

from pathlib import Path

from .device import Device


class Bridge:

   def __init__(self):

      settingsFileName = str(Path.home()) + '/.hue_v2.json'
      if not os.path.exists(settingsFileName):
         settings = self.compileCredentialsV2()
         with open(settingsFileName, 'w') as outfile:
            json.dump(settings, outfile, indent=3)
      else:
         with open(settingsFileName, 'r') as infile:
            settings = json.load(infile)

      self.baseUrl = f'https://{settings["bridge"]}/clip/v2/resource/'
      self.header = {'hue-application-key': settings['username']}

      # get rid of warning tghat connectin is insecure
      urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

      #response = requests.get(self.baseUrl + 'light', verify=False, headers=self.header)
      self.lights = response.json()
      print(self.lights)
      sys.exit(0)

      self.devices = dict()
      for device in self.lights['data']:
         id = device['id']
         name = device['metadata']['name']
         self.devices[name] = id

   def compileCredentialsV2(self):

      location = requests.get('https://discovery.meethue.com').json()
      location = location[0]  # can be several bridges
      ip = location['internalipaddress']
      id = location['id']

      settings = {'bridge': ip, 'devicetype': 'odense_hue', 'id': id}
      print('bridge @', ip, id)

      keyUrl = f'http://{ip}/api'
      keyRequest = {'devicetype': 'odense_hue', 'generateclientkey': True}

      data = requests.post(keyUrl, json=keyRequest, headers=self.header, verify=False).json()
      data = data[0]

      if 'error' in data:
         description = data['error']['description']
         print(description)
         sys.exit()
      elif 'success' in data:
         settings['username'] = data['success']['username']
         settings['clientkey'] = data['success']['clientkey']

      return settings

   def listDevices(self, deviceName=None):

      if deviceName and deviceName in self.devices:
         key = self.devices[deviceName]
         device = str(self.lights[key])
         device = device.replace("'", '"')
         print(device)
      else:
         nameList = list(self.devices.keys())
         print(nameList)

   def getDeviceId(self, name):

      if not name in self.devices:
         return None
      else:
         deviceId = self.devices[name]
         device = Device(self, deviceId)
         return device

   def getState(self, deviceId):

      data = requests.get(self.baseUrl + 'lights/' + deviceId, headers=self.header, verify=False).json()
      return data['state']

   def setState(self, deviceId, payload):

      response = requests.put(self.baseUrl + 'light/' + deviceId, json=payload, headers=self.header, verify=False)
      print(self.baseUrl + 'light/' + deviceId, payload, response.text)
