#!/bin/bash

if [ -z $parentscript ]; then
	parentscript='HEADLESSGRAPHICSTOOLS'
fi

################################################################################
################################# PACKAGE LIST #################################
################################################################################
packageList+=(
	'ffmpeg'      # video converter and manipulator
	'imagemagick' # image converter and manipulator
	'jp2a'        # jpeg to ascii art converter
	'figlet'      # Generate ASCII / ANCII art text and banners
)


################################################################################
################################### SETTINGS ###################################
################################################################################
figletFontPath='/usr/share/figlet/'
figletFontMD5Hash="1154e685b36a07f98c92162bd243910c"


################################################################################
############################# ADDITIONAL FUNCTIONS #############################
################################################################################

########################## INSTALL EXTRA FIGLET FONTS ##########################
# This function pulls down some extra figlet fonts from jave.de and extracts   #
# them into the figlet font folder automatically                               #
################################################################################
installExtraFigletFonts() {
	figletFontTempPath=`mktemp`

	# Download the additional fonts to a temp-file
	wget -O "$figletFontTempPath" "http://www.jave.de/figlet/figletfonts40.zip"
	
	# Generate and check the checksum of the download file to make sure it is the right one
	echo "Matching Checksum"
	echo "$figletFontMD5Hash  $figletFontTempPath" | md5sum -c - 2> /dev/null 1> /dev/null
	
	# if it is unzip the file, otherwise throw a warning
	if [ $? -eq 0 ]; then
		# Unzip the contents of the fonts folder into the figlet font path
		unzip -jn $figletFontTempPath fonts/* -d $figletFontPath
	else
		echo "WARNING: Extra Figlet Fonts md5 does not match historical!"
	fi 

	# Clean up the tempfile
	echo "Cleaning Tempfile"
	rm $figletFontTempPath
}

# Add the additional fonts to 
postFunctionList+=(installExtraFigletFonts)


################################################################################
################################## EXECUTION ###################################
################################################################################

if [ $parentscript!='HEADLESSGRAPHICSTOOLS' ]; then
	# install all the packages on the list
	echo apt-get install `printf -- '%s\n' ${packageList[@]}`

	# run the additional functions
	echo ${postFunctionList[@]}
fi
