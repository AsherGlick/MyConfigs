#!/bin/bash
fdisk -l 2>/dev/null | grep Disk | grep -v identifier