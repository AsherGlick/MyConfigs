#!/bin/sh

# The path that figlet stores it's font files
figletFontPath='/usr/share/figlet/'


# $packages .+ " ffmpeg" # video converter and manipulator
# $packages .+ " imagemagick" # image converter and manipulator
# $packages .+ " jp2a" # jpeg to ascii art converter


# # Generate ASCII / ANCII art text and banners
# $packages .= " figlet"

installExtraFigletFonts() {
	figletFontTempPath=`mktemp`
	wget -O "$figletFontTempPath" "http://www.jave.de/figlet/figletfonts40.zip"
	echo "Matching Checksum"
	echo "1154e685b36a07f98c92162bd243910c  $figletFontTempPath" | md5sum -c - 2> /dev/null 1> /dev/null
	if [ $? -eq 0 ]; then
		# Unzip the contents of the fonts folder into the figlet font path
		unzip -jn $figletFontTempPath fonts/* -d $figletFontPath
	else
		echo "WARNING: Extra Figlet Fonts md5 does not match!"
	fi 
	echo "Cleaning Tempfile"
	rm $figletFontTempPath
}



# Install all the packages that are specified 
## uncomment this next line
# apt-get install $packages

installExtraFigletFonts


# echo "1154e685b36a07f98c92162bd243910a  /tmp/tmp.eHxN87KPd8" | md5sum -c -

# GUI based tools
#(dia, blender, inkscape, audacity, vlc, gimp, recordMyDesktop, openShot)



	# 1154e685b36a07f98c92162bd243910c  figletfonts40.zip


	# echo -e "1154e685b36a07f98c92162bd243910c  figletfonts40.zip" | md5sum -c -


	# echo -e "1154e685b36a07f98c92162bd243910c  figletfonts40.zip" | md5sum -c -