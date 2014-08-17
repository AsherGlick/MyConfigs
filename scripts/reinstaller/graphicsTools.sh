#!/bin/bash

if [ -z $parentscript ]; then
	parentscript='GRAPHICSTOOLS'
fi

################################################################################
################################# PACKAGE LIST #################################
################################################################################
packageList+=(
	'dia'             # Chart and graph editor
	'blender'         # 3D model editor (+more 3d stuff)
	'inkscape'        # Vector Image editing
	'gimp'            # Raster Image editing
	'vlc'             # Video playing
	'audacity'        # Audio recording
	'recordmydesktop' # As of 2012 the best desktop recorder for linux I have found
	'openshot'        # Video Editor
	'pavucontrol'     # Audo control to help with screen recording
)

################################################################################
################################### SETTINGS ###################################
################################################################################

################################################################################
############################# ADDITIONAL FUNCTIONS #############################
################################################################################

################################################################################
################################# SUB SCRIPTS ##################################
################################################################################
source headlessGraphicsTools.sh

################################################################################
################################## EXECUTION ###################################
################################################################################

if [ $parentscript == 'GRAPHICSTOOLS' ]; then
	# install all the packages on the list
	apt-get install `printf -- '%s\n' ${packageList[@]}`

	# run the additional functions
	${postFunctionList[@]}
fi

