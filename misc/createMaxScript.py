#!/usr/bin/env python3

import os
import argparse


def createHeader():

   text = 'autowatch = 1;\n'
   text += '\n'

   text += 'include("helper.js");\n'
   text += '\n'

   text += '// inlets and outlets\n'
   text += f'inlets = 1;\n'
   text += f'setinletassist(0, "text");\n'
   text += '\n'

   text += f'outlets = 1;\n'
   text += f'setoutletassist(0, "text"); \n'
   text += '\n'

   return text


def createStorage():

   text = '//////////////////////////////////////////\n'
   text += '\n'
   text += 'let settings = {};\n'
   text += '\n'

   text += 'function getvalueof() {\n'
   text += '   let text = JSON.stringify(settings);\n'
   text += '   return text;\n'
   text += '}\n'
   text += '\n'

   text += 'function setvalueof(value) {\n'
   text += '   settings = JSON.parse(value);\n'
   text += '}\n'
   text += '\n'

   text += '//////////////////////////////////////////\n'
   text += '\n'

   return text


def createBody():

   text = 'function bang(){\n'
   text += '   print("bang");\n'
   text += '}\n'
   text += '\n'

   return text


def main():

   parser = argparse.ArgumentParser(description='Create new Max javascript.')
   parser.add_argument('scriptnames', metavar='SCRIPTS', type=str, nargs='+', help='name of script to create')
   parser.add_argument('-s', '--storage', action='store_true', help='add settings persistance')

   args = parser.parse_args()  # will quit here if help is called

   scriptnames = args.scriptnames

   for scriptname in scriptnames:
      if not scriptname.endswith('.js'):
         scriptname += '.js'

      if os.path.exists(scriptname):
         print(f'script {scriptname} does already exist')
         return

      text = createHeader()
      if args.storage:
         text += createStorage()
      text += createBody()

      with open(scriptname, 'w') as outfile:
         outfile.write(text)


if __name__ == '__main__':
   main()
