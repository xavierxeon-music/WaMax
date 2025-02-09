#!/usr/bin/env python3

import os
import sys


def createFolder(name):

   folder = os.getcwd() + '/maxobjects/' + name
   try:
      os.makedirs(folder)
   except FileExistsError:
      print(f'project {name} already exists')
      sys.exit(1)

   return folder


def createCMake(folder):

   fileName = folder + '/CMakeLists.txt'

   with open(fileName, 'w') as outfile:

      outfile.write('include(${C74_MIN_API_DIR}/script/min-pretarget.cmake)\n')
      outfile.write('\n')
      outfile.write('include_directories("${C74_INCLUDES}")\n')
      outfile.write('\n')
      outfile.write('file(GLOB PROJECT_SRC\n')
      outfile.write('   "*.h"\n')
      outfile.write('   "*.cpp"\n')
      outfile.write(')\n')
      outfile.write('add_library(\n')
      outfile.write('   ${PROJECT_NAME}\n')
      outfile.write('   MODULE\n')
      outfile.write('   ${PROJECT_SRC}\n')
      outfile.write(')\n')
      outfile.write('force_cpp_version()\n')
      outfile.write('\n')
      outfile.write('include(${C74_MIN_API_DIR}/script/min-posttarget.cmake)\n')


def createHeader(folder, projectName, className):

   fileName = folder + '/' + projectName + '.h'

   with open(fileName, 'w') as outfile:

      outfile.write(f'#ifndef {className}H\n')
      outfile.write(f'#define {className}H\n')
      outfile.write('\n')

      outfile.write('#include "c74_min.h"\n')
      outfile.write('using namespace c74::min;\n')
      outfile.write('\n')

      outfile.write(f'class {className} : public object<{className}>\n')
      outfile.write('{\n')
      outfile.write('public:\n')
      outfile.write('   MIN_DESCRIPTION{"description"};\n')
      outfile.write('\n')
      outfile.write('public:\n')
      outfile.write(f'   {className}(const atoms& args = {{}});\n')
      outfile.write('};\n')

      outfile.write('\n')
      outfile.write(f'#endif // {className}H\n')


def createSource(folder, projectName, className):

   fileName = folder + '/' + projectName + '.cpp'

   with open(fileName, 'w') as outfile:

      outfile.write(f'#include "{projectName}.h"\n')
      outfile.write('\n')

      outfile.write(f'{className}::{className}(const atoms& args)\n')
      outfile.write(f'   : object<{className}>()\n')
      outfile.write('{\n')
      outfile.write('\n')
      outfile.write('}\n')
      outfile.write('\n')

      outfile.write(f'MIN_EXTERNAL({className});\n')
      outfile.write('\n')


def main():

   if 3 > len(sys.argv):
      print('you need to specify a project and class name')
      sys.exit(1)

   projectName = sys.argv[1]
   className = sys.argv[2]
   folder = createFolder(projectName)
   print(f'create project {projectName} @ {folder}')

   createCMake(folder)
   createHeader(folder, projectName, className)
   createSource(folder, projectName, className)


if __name__ == '__main__':
   main()
