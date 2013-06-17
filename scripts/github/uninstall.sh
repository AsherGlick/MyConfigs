#!/bin/sh

if [ $# != 0 ]; then
	if [ $1 = "--line-help" ]; then
		echo "uninstall - Remove the symlink for github.sh in usr/bin"
		exit
	fi
fi


DESTINATION="/usr/bin/github"
rm $DESTINATION