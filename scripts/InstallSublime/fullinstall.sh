#!/bin/sh
# This program will download and 'install' sublime text 2 on your linux machine

#BITFLAVOR=`uname -m`
BITFLAVOR="JJJ"
BITSIXTYFOUR="x86_65"
echo "$BITFLAVOR"
if [ "$BITFLAVOR" = "$BITSIXTYFOUR" ]
then
	echo "HELLO WORLD"
	#DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1%20x64.tar.bz2"
else
	echo "ELSE CASE!"
	#DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1.tar.bz2"
fi
echo "$DOWNLOADLINK"
