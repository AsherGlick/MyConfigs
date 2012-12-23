#!/bin/sh
# This program will download and 'install' sublime text 2 on your linux machine

# Run the download and extract script
./downloadAndExtract.sh

# Run the linker Script
./createSimlink.sh

# Run the Launcher Script
sudo ./createLauncher.sh

# Run the replacement Script
sudo ./overwriteTextEditor.sh
