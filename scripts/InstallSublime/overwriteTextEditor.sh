#!/bin/sh
echo "OVERWRITING OLD TEXT EDITOR WITH SUBLIME in default list"
DEFAULTAPPS="/usr/share/applications/defaults.list"
TEMPLOCATION=`mktemp`
sed -e "s/gedit.desktop/sublime.desktop/ig" "$DEFAULTAPPS" > "$TEMPLOCATION"
mv "$TEMPLOCATION" "$DEFAULTAPPS"
rm "$TEMPLOCATION"
