#!/bin/bash

if [ -z $parentscript ]; then
	parentscript='CORETOOLS'
fi

################################################################################
################################# PACKAGE LIST #################################
################################################################################
packageList+=(
	'man'     # an interface to the on-line reference manuals
	'git'     # Fast Scalable distributed revision control stystem
	'screen'  # Screen manager withe TC100/ANSI terminal emulation
	'tmux'    # Terminal Multiplexer 
	'sudo'    # Execute command as another user
	'zsh'     # the Z shell
	'grep'    # print lines matching a pattern
	'sed'     # stream editor for filtering and transforming text
	'dialog'  # display dialog boxes from shell scripts
	'less'    # Better more, view scrollable files
	'nano'    # Nano's ANOther editor, an enhanced free Pico clone
	'ssh'     # OPENSSH SSH client (remote login program)
	'wget'    # The non-interactive network downloader
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

################################################################################
################################## EXECUTION ###################################
################################################################################

if [ $parentscript == 'CORETOOLS' ]; then
	# install all the packages on the list
	apt-get install `printf -- '%s\n' ${packageList[@]}`

	# run the additional functions
	${postFunctionList[@]}
fi
