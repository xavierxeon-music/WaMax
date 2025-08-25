#!/bin/bash

OS_TEST=$(uname -s)
if [ $OS_TEST != "Darwin" ]; then
   echo "This script is for Mac OS X only"
   exit 1
fi

SIZE_IN_MB=$1
if [ -z $SIZE_IN_MB ]; then
   SIZE_IN_MB=32
fi

echo SIZE_IN_MB = $SIZE_IN_MB 

shmall=$(( ($SIZE_IN_MB / 4) *  1024))
sudo sysctl -w kern.sysv.shmall=$shmall 

shmmax=$(($shmall * 4096))
sudo sysctl -w kern.sysv.shmmax=$shmmax


