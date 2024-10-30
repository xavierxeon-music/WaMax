#!/bin/bash

SOURCE=$(pwd)
TARGET=~/Documents/Max\ 9/Packages

if [ ! -d "$TARGET" ]
then
   echo "No Max installation at $TARGET"
   exit
fi

if [ -L "$TARGET/WaMaxPackageBase" ]
then 
   rm  "$TARGET/WaMaxPackageBase" 
   echo "remove exising link"
fi

ln -s "$SOURCE" "$TARGET/WaMaxPackageBase"
echo "DONE"
