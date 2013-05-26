#!/bin/sh

# The line help function is a single line help file for each script / funciton
if [ $1 = "--line-help" ]; then
	echo "newkey - push a pubkey to your github account (automaticly create one)"
	exit
fi

# this script will add your computer's pubkey to your github account
# if you dont have a key yet it will prompt you to generate one

# First checkto see if the ssh.pub file exists
FILE="$HOME/.ssh/id_rsa.pub"

if [ -f "$FILE" ];
then
	echo "Found Public Key"
else
	echo "Cannot find public key: generating one for you"
	read -p "What email would you like the key to have? " EMAIL
	ssh-keygen -t rsa -C "$EMAIL"
fi

# Grab the title the user would like to use for the key
read -p "Computer Title: " TITLE
# Prompt the user for their github username
read -p "Github username: " USERNAME
# Read the public key into memory
KEY=`cat $HOME/.ssh/id_rsa.pub`
# send the post request to the github api to add the key
curl -i -u "$USERNAME" -d "{\"title\":\"$TITLE\",\"key\":\"$KEY\"}" https://api.github.com/user/keys
#echo "{\"title\":\"$TITLE\",\"key\":\"$KEY\"}"
