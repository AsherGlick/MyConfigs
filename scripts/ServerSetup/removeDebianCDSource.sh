#!/bin/bash



sed -e 's/^deb cdrom:/# deb cdrom:/g' /etc/apt/sources.list > /etc/apt/sources.list


