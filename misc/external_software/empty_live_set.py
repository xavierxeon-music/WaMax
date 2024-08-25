#!/usr/bin/env python3

import os
import shutil
import sys

sourceSet = os.path.dirname(__file__)  + '/Untitled.als'

targetPath = sys.argv[1]  
index = targetPath.find(':')
if index > 0:
   front = targetPath[:index]
   back = targetPath[index + 1:]

   targetPath = '/Volumes/' + front + back

targetSet = targetPath + sys.argv[2] 

#print(targetPath, targetSet)

if  os.path.exists(targetSet):
   sys.exit(1)

os.makedirs(targetPath + '/Ableton Project Info')
shutil.copy(sourceSet, targetSet)
