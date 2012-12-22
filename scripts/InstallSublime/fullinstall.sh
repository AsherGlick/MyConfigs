#!/bin/sh
# This program will download and 'install' sublime text 2 on your linux machine

BITFLAVOR=`uname -m`
BITSIXTYFOUR="x86_64"
TEMPFILE=`mktemp`
#echo "$BITFLAVOR"
if [ "$BITFLAVOR" = "x86_64" ]
then
	echo "DETECTED 64bit Operating System"
	DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1%20x64.tar.bz2"
else
	echo "DETECTED 32bit Operating System"
	DOWNLOADLINK="http://c758482.r82.cf2.rackcdn.com/Sublime%20Text%202.0.1.tar.bz2"
fi

#read DOWNLOADLOCATION
#echo "$DOWNLOADLINK"
wget -O "$TEMPFILE" "$DOWNLOADLINK"
echo "EXTRACTING"
echo "DELEATING TEMP FILES"
rm "$TEMPFILE"
echo "CREATING DESKTOP ICON"
echo "OVERWRITING OLD TEXT EDITOR WITH SUBLIME"
