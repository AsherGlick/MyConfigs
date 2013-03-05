#!/bin/bash

##################################### ABOUT ####################################
# This script is meant for use when configuring Debian servers, it opens up    #
# the sources list and comments out the lines that contain sources from the    #
# CD-ROM, leaving only sources from the net. This enables packages to be       #
# updated fully. This script may need to be run with root permissions          #
################################################################################

# find all lines that begin with 'deb cdrom:' and replace them with '# deb cdrom:'
sed -e 's/^deb cdrom:/# deb cdrom:/g' < /etc/apt/sources.list > /etc/apt/sources.list


