#!/bin/bash

if [ -z $parentscript ]; then
	parentscript='DEVTOOLS'
fi

################################################################################
################################# PACKAGE LIST #################################
################################################################################
packageList+=(
	'arduino'
	'eclipse'

	'libsdl1.2-dev'
	'libsdl-mixer1.2-dev'
	'libsdl-image1.2-dev'
	'libsdl-ttf2.0-dev'
	'python-qt4-dev'
	'libqt4-dev'
	'qt4-dev-tools'
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
source headlessDevTools.sh

################################################################################
################################## EXECUTION ###################################
################################################################################

if [ $parentscript == 'DEVTOOLS' ]; then
	# install all the packages on the list
	apt-get install `printf -- '%s\n' ${packageList[@]}`

	# run the additional functions
	${postFunctionList[@]}
fi
