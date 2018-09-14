dpkg -l 'linux-image*' | awk '/^ii/{ print $2 }' | grep -e '-[0-9][0-9]*-generic' | grep -v `uname -r` | xargs xargs sudo dpkg --force-all --purge
sudo apt install -f -y
sudo apt autoremove