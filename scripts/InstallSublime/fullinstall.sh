# 32 bit
#http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1.tar.bz2
# 64 bit
#http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1%20x64.tar.bz2



#!/bin/sh
# This program will download and 'install' sublime text 2 on your linux machine

BITFLAVOR=`uname -m`
if [ "$BITFLAVOR" == "x86_64" ]; then
	DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1%20x64.tar.bz2"
else
	DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1.tar.bz2"
fi
echo "$DOWNLOADLINK"
