
# THIS SCRIPT SHOULD NOT BE USED (ONLY WORKS FOR VERSION 1.14)
echo "Generating Temp FIle"
TMPFILE=`mktemp`
echo "Downloading Jetpack SDK"
wget -O $TMPFILE https://ftp.mozilla.org/pub/mozilla.org/labs/jetpack/jetpack-sdk-latest.tar.gz
SDKPATH=`tar -tzf $TMPFILE | sed -e 's@/.*@@' | uniq`
echo "Extracting Jetpack SDK to /etc/opt"
sudo tar -xzf $TMPFILE -C /etc/opt/
echo "Symlinking extracted cfx as /usr/bin/cfx"
sudo ln -s /etc/opt/$SDKPATH/bin/cfx /usr/bin/cfx
echo "Cleaning Up Tempfiles"
rm $TMPFILE