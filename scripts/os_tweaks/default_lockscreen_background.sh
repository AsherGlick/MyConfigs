#!/bin/sh

# This script is to prevent the user background from being used in the lock
# screen or the login screen of ubuntu

# Set the dconf value of draw-user-backgrounds to false
dconf write /com/canonical/unity-greeter/draw-user-backgrounds false

# Change the setting for the login screen as well
sudo xhost +SI:localuser:lightdm
sudo -H -u lightdm dconf write /com/canonical/unity-greeter/draw-user-backgrounds false

# A UI editor for these properties can be used called `dconf-editor`

