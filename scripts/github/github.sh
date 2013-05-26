#!/bin/sh

# The line help function is a single line help file for each script / funciton
if [ $1 = "--line-help" ]; then
	echo "github - wrapping function, use this to call all the other functions"
	exit
fi

# Actual code
BASEDIR=$(dirname $(readlink -f $0))
SH=".sh"
SLASH="/"
TARGET=$BASEDIR$SLASH$1$SH
if [ -f $TARGET ]
then 
	shift
	$TARGET $@
else 
	# if the user does not give a valid command run thorugh all of the files
	# and print out their single line help descriptions
	echo $TARGET
	echo "$1 Is not a valid command"
	for f in $BASEDIR$SLASH*.sh
	do
		$f --line-help
	done
fi
