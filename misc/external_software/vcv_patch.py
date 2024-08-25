#!/usr/bin/env python3

import os
import shutil
import sys

sourcePatch = os.path.dirname(__file__)  + '/Untitled.vcv'

targetPath = sys.argv[1]  
index = targetPath.find(':')
if index > 0:
   front = targetPath[:index]
   back = targetPath[index + 1:]

   targetPath = '/Volumes/' + front + back

targetPatch = targetPath + sys.argv[2] 

#print(targetPath, targetPatch)

if os.path.exists(targetPatch):
   sys.exit(1)

shutil.copy(sourcePatch, targetPatch)
