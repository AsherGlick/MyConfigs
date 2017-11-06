#!/usr/bin/python

#################################### LICENCE ###################################
# Copyright (c) 2012, Asher Glick                                              #
# All rights reserved.                                                         #
#                                                                              #
# Redistribution and use in source and binary forms, with or without           #
# modification, are permitted provided that the following conditions are met:  #
#                                                                              #
# * Redistributions of source code must retain the above copyright notice,     #
# this                                                                         #
#   list of conditions and the following disclaimer.                           #
# * Redistributions in binary form must reproduce the above copyright notice,  #
#   this list of conditions and the following disclaimer in the documentation  #
#   and/or other materials provided with the distribution.                     #
#                                                                              #
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  #
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    #
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   #
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE    #
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR          #
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         #
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS     #
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      #
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)      #
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   #
# POSSIBILITY OF SUCH DAMAGE.                                                  #
################################################################################

import json
import shutil
import os
import filecmp

json_data = open('scriptconfig')

data = json.load(json_data)

json_data.close()

# some simple colors for the terminal
OKGREEN = '\033[1;32m'  # green 32
WARNING = '\033[1;33m'  # yellow 33
FAIL = '\033[1;31m'     # red 31
LINKED = '\033[1;34m'   # blue 34
ENDC = '\033[0m'        # reset


############################### GET TERMINAL SIZE ##############################
# This function gets the size of the terminal and returns a width and a        #
# height as ints                                                               #
################################################################################
def getTerminalSize():
    import os
    env = os.environ

    def ioctl_GWINSZ(fd):
        try:
            import fcntl
            import termios
            import struct
            cr = struct.unpack('hh', fcntl.ioctl(fd, termios.TIOCGWINSZ, '1234'))
        except:
            return None
        return cr
    cr = ioctl_GWINSZ(0) or ioctl_GWINSZ(1) or ioctl_GWINSZ(2)
    if not cr:
        try:
            fd = os.open(os.ctermid(), os.O_RDONLY)
            cr = ioctl_GWINSZ(fd)
            os.close(fd)
        except:
            pass
    if not cr:
        try:
            cr = (env['LINES'], env['COLUMNS'])
        except:
            cr = (25, 80)
    return int(cr[1]), int(cr[0])


################################## PRINT FILE ##################################
# Given a single file in the config it prints out a line giving the status of  #
# that file depending of if the files exist, are linked together, are          #
# different, or are identical                                                  #
################################################################################
def printFile(repo_file, os_file):
    (width, height) = getTerminalSize()

    color = ""
    text = ""

    # OS File Missing
    if not os.path.exists(os_file):
        color = FAIL
        text = "OS File Missing"

    # Repo File Missing
    elif not os.path.exists(repo_file):
        color = FAIL
        text = "Repo File Missing"

    # Linked Files
    elif is_linked(repo_file, os_file):
        color = LINKED
        text = "Linked"

    # Same File on both repo and os (but not linked)
    elif not is_different(repo_file, os_file):
        color = OKGREEN
        text = "Identical Files"

    # Differnet Files
    else:
        color = WARNING
        text = "Different Files"

    # Print row
    # print color, text, ENDC

    file = os.path.basename(repo_file)
    print " " + color + file + ENDC + " " + ("." * (width - 6 - len(file) - len(text))) + color + " [" + text + "]" + ENDC


################################### IS LINKED ##################################
# Checks to see if the os file is a symlink and if it is checks to make sure   #
# it points to the correct repo file                                           #
################################################################################
def is_linked(repo_file, os_file):
    try:
        link_target = os.readlink(os_file)
        if link_target == repo_file:
            return True
        else:
            return False

    except OSError:
        # Probably not a symlink
        return False


################################# IS DIFFERENT #################################
# Checks to see if the two files listed are identical or if they have changes  #
# between them                                                                 #
################################################################################
def is_different(repo_file, os_file):
    try:
        return not filecmp.cmp(os_file, repo_file)

    except:
        return True


################################## GET OS PATH #################################
# A helper function to get the path of the OS file based on the file name in   #
# the config                                                                   #
################################################################################
def get_os_path(file):
    return os.path.expanduser(data[file])


################################# GET REPO PATH ################################
# A helper function to get the path of the repo file based on the file name    #
# in the config                                                                #
################################################################################
def get_repo_path(file):
    return os.path.abspath(os.path.join(os.path.dirname(__file__), "configs", file))


############################## ACTION ON ALL FILES #############################
# Runs a function on all the files in the config. Each function takes in two   #
# arguments, the first is the repo file path, the second is the os file path   #
################################################################################
def aciton_on_all_files(action_function):
    for file in data:
        os_path = get_os_path(file)
        repo_path = get_repo_path(file)

        action_function(repo_path, os_path)


################################# RESTORE COPY #################################
# This function copys the file from the the local file to the target file in   #
# effect "restoring" the file to the machine                                   #
################################################################################
def restoreFile(repo_file, os_file, force=False):
    # Dont overwrite symlinks unless you want to
    if os.path.islink(os_file) and force is False:
        return

    shutil.copyfile(repo_file, os_file)


################################## BACKUP COPY #################################
# This function copys the target file into the local directory in order to     #
# back it up so that it is saved in the repo                                   #
################################################################################
def backupFile(repo_file, os_file):
    shutil.copyfile(os_file, repo_file)


################################## LINK FILES ##################################
# This function creates a symlink at the target file pointing at the file      #
# stored in the configurations directory allowing for the files to allways be  #
# synced                                                                       #
################################################################################
def linkFile(repo_file, os_file):
    if os.path.exists(os_file):
        os.remove(os_file)
    os.symlink(repo_file, os_file)


## the main statement that handles the arguments and calls the apropriate functions
if __name__ == "__main__":
    import argparse

    parser = argparse.ArgumentParser(description='Linking and syncromizing config files')

    parser.add_argument('--backup', type=str, nargs="+", help='Attempt to copy an unlinked file from their os locaiton to their repo location')
    parser.add_argument('--restore', type=str, nargs="+", help="Attempt to copy a file from their repo location to their OS location, overwrites simlinks")
    parser.add_argument('--link', type=str, nargs="+", help="Attempt to create a symlink for a particular repo file at the OS file location")

    parser.add_argument('--backup-all', action="store_true", help='Attempt to copy all unlinked files from their OS locaiton to their repo location')
    parser.add_argument('--restore-all', action="store_true", help="Attempt to copy all unlinked files from their repo location to their OS location")
    parser.add_argument('--link-all', action="store_true", help="Attempt to create a symlink for all OS files pointing to the repo files")

    args = parser.parse_args()

    # Backup
    if args.backup:
        for file in args.backup:
            backupFile(get_repo_path(file), get_os_path(file))
    if args.backup_all:
        aciton_on_all_files(backupFile)

    # Restore
    if args.restore:
        for file in args.restore:
            restoreFile(get_repo_path(file), get_os_path(file), force=True)
    if args.restore_all:
        aciton_on_all_files(restoreFile)

    # Link
    if args.link:
        for file in args.link:
            linkFile(get_repo_path(file), get_os_path(file))
    if args.link_all:
        aciton_on_all_files(linkFile)

    # Print File State
    aciton_on_all_files(printFile)
