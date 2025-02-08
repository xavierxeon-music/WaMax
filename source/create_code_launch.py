#!/usr/bin/env python3

import os
import json
import platform


def addExplainer(fileName):

   with open(fileName, 'r') as launchfile:
      content = launchfile.readlines()

   content.insert(1, '   // Use IntelliSense to learn about possible attributes.\n')
   content.insert(2, '   // Hover to view descriptions of existing attributes.\n')
   content.insert(3, '   // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387\n')

   with open(fileName, 'w') as launchfile:
      launchfile.writelines(content)


def ceateCodeLaunch(fileName):

   config = {
       'name': 'Debug',
       'type': 'cppdbg',
       'request': 'launch',
       'cwd': '${workspaceRoot}',
       'stopAtEntry': False
   }

   match(platform.system()):
      case 'Darwin':
         config['program'] = '/Applications/Max.app/Contents/MacOS/Max'
         config['osx'] = {'MIMode': 'lldb'}
      case 'Windows':
         config['program'] = '/Applications/Max.app/Contents/MacOS/Max'
         config['windows'] = {'MIMode': 'lldb'}

   config['args'] = []

   content = {
      'version': '0.2.0',
      'configurations': [config]
   }

   with open(fileName, 'w') as launchfile:
      json.dump(content, launchfile, indent=3)


def main():

   fileName = '.vscode/launch.json'
   if os.path.exists(fileName):
      print('launch file exists')
      return

   ceateCodeLaunch(fileName)
   addExplainer(fileName)


if __name__ == '__main__':
   main()
