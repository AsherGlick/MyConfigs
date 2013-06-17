#!/bin/sh

# The line help function is a single line help file for each script / funciton
if [ $1 = "--line-help" ]; then
	echo "clone - clone a repo with just 'username/reponame'"
	exit
fi

URLBEGIN="git@github.com:"
DOTGIT=".git"
URL=$URLBEGIN$1$DOTGIT
git clone $URL