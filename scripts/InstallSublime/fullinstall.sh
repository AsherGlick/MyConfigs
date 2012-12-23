#!/bin/sh
# This program will download and 'install' sublime text 2 on your linux machine

# Run the download and extract script
./downloadAndExtract.sh

# Run the linker Script
./createSimlink.sh

# Run the Launcher Script
sudo ./createLauncher.sh

echo ""

while true; do
    read -p "Do you wish to have sublime replace gedit?[Y/n] " yn
    case $yn in
        [Yy]* ) 
		# Run the replacement Script
		sudo ./overwriteTextEditor.sh;
		break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no.";;
    esac
done


