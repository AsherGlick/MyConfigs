#!/bin/sh

# The line help function is a single line help file for each script / funciton
if [ $# != 0 ]; then
	if [ $1 = "--line-help" ]; then
		echo "install - Put a symlink for github.sh in /usr/bin/"
		exit
	fi
fi

BASEDIR=$(dirname $(readlink -f $0))
SH="/github.sh"
TARGET=$BASEDIR$SH

DESTINATION="/usr/bin/github"
ln -s $TARGET $DESTINATION