#!/bin/bash

cd ../externals

for MXO_FILE in $(ls -1  | grep \\.mxo)
do
   echo $MXO_FILE
   codesign --force --deep -s - $MXO_FILE &> /dev/null
done

