#!/usr/bin/env python3

import os
import shutil

def compileFolderMap():

    folderMap = dict()
    for folder in os.scandir('../patchers'):
        for patch in os.scandir(folder.path):
            name = patch.name
            if not name.endswith('.maxpat'):
                continue

            name = name.replace('.maxpat', '')
            folderMap[name] = folder.name
    
    return folderMap

def sortNames(rootFolder, suffix, folderMap):

    rootFolder =  os.path.abspath(rootFolder)

    for file in os.scandir(rootFolder):
        
        if not file.name.endswith(suffix):
            continue

        name = file.name.replace(suffix, '')
        if not name in folderMap:
            print('! ' + name)
        else:
            subFolder = rootFolder + '/' + folderMap[name]
            if not os.path.exists(subFolder):
                os.mkdir(subFolder)
            targetFile = subFolder + '/' + file.name                
            shutil.move(file.path, targetFile)


def main():

    folderMap = compileFolderMap()
    sortNames('../docs', '.maxref.xml', folderMap)
    sortNames('../help', '.maxhelp', folderMap)
    sortNames('../init', '.txt', folderMap)

if __name__ == '__main__':
    main()
