#!/usr/bin/env python3

import os
import sys


def createFolder(name):

    folder = os.getcwd() + '/objects/' + name
    try:
        os.makedirs(folder)
    except FileExistsError:
        print(f'project {name} already exists')
        sys.exit(1)

    return folder


def createCMake(folder):

    fileName = folder + '/CMakeLists.txt'

    with open(fileName, 'w') as outfile:

        outfile.write('include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-pretarget.cmake)\n')
        outfile.write('\n')
        outfile.write('# ############################################################\n')
        outfile.write('# MAX EXTERNAL\n')
        outfile.write('# ############################################################\n')
        outfile.write('include_directories(\n')
        outfile.write('   "${MAX_SDK_INCLUDES}"\n')
        outfile.write('   "${MAX_SDK_MSP_INCLUDES}"\n')
        outfile.write('   "${MAX_SDK_JIT_INCLUDES}"\n')
        outfile.write(')\n')
        outfile.write('\n')
        outfile.write('file(GLOB PROJECT_SRC\n')
        outfile.write('   "*.h"\n')
        outfile.write('   "*.c"\n')
        outfile.write('   "*.cpp"\n')
        outfile.write(')\n')
        outfile.write('add_library(\n')
        outfile.write('   ${PROJECT_NAME}\n')
        outfile.write('   MODULE\n')
        outfile.write('   ${PROJECT_SRC}\n')
        outfile.write(')\n')
        outfile.write('\n')
        outfile.write('include(${CMAKE_CURRENT_SOURCE_DIR}/../../max-sdk-base/script/max-posttarget.cmake)\n')


def createHeader(folder, name):

    fileName = folder + '/' + name + '.h'

    with open(fileName, 'w') as outfile:

        outfile.write(f'#ifndef {name}H\n')
        outfile.write(f'#define {name}H\n')
        outfile.write('\n')
        outfile.write('extern "C"\n')
        outfile.write('{\n')
        outfile.write('#include "ext.h"      // standard Max include, always required\n')
        outfile.write('#include "ext_obex.h" // required for new style Max object\n')
        outfile.write('}\n')
        outfile.write('\n')
        outfile.write(f'#endif // {name}H\n')


def createSource(folder, name):

    fileName = folder + '/' + name + '.cpp'

    with open(fileName, 'w') as outfile:

        outfile.write(f'#include "{name}.h"\n')
        outfile.write('\n')
        outfile.write('void ext_main(void* r)\n')
        outfile.write('{\n')
        outfile.write('}\n')


def main():

    if 2 > len(sys.argv):
        print('you need to specify a project name')
        sys.exit(1)

    name = sys.argv[1]
    folder = createFolder(name)
    print(f'create project {name} @ {folder}')

    createCMake(folder)
    createHeader(folder, name)
    createSource(folder, name)


if __name__ == '__main__':
    main()
