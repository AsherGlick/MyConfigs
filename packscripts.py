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
import sys
import filecmp

json_data=open('scriptconfig')

data = json.load(json_data)

json_data.close()

# some simple colors for the terminal
OKGREEN = '\033[92m' # green
WARNING = '\033[93m' # yellow
FAIL = '\033[91m'    # red
LINKED = '\033[94m'  # blue
ENDC = '\033[0m'


############################### GET TERMINAL SIZE ##############################
# This function gets the size of the terminal and returns a width and a        #
# height as ints                                                               #
################################################################################
def getTerminalSize():
    import os
    env = os.environ
    def ioctl_GWINSZ(fd):
        try:
            import fcntl, termios, struct, os
            cr = struct.unpack('hh', fcntl.ioctl(fd, termios.TIOCGWINSZ,
        '1234'))
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


################################## CYCLE FILES #################################
# This function cycles through all of the files, checks if they are the same   #
# file or a different file and then runs the specified function on the two     #
# files                                                                        #
################################################################################
def cycleFiles(beginningComment, functiontorun, verbose=False, sucessMessage="Sucess"):
    (width, height) = getTerminalSize()

    print beginningComment

    for element in data:
        output = ""
        info = ""
        path = os.path.expanduser(data[element])
        localPath = os.path.expanduser("configs/"+element)
        localPath = os.path.abspath(localPath)

        #print localPath

        symbolicLink = ""
        try:
            symbolicLink = os.readlink(path)
        except OSError:
            #print "NOT LINK"
            pass

        filesDiffer = True

        try:
            filesDiffer = not filecmp.cmp(path,localPath)
        except:
            pass

        # if the target file is not a symbolic link
        if symbolicLink == "":
            info = sucessMessage
            color = OKGREEN
            try:
                if filesDiffer:
                    functiontorun(localPath,path)
                else:
                    info = "No Change"
                    color = WARNING
            except IOError:
                info = "Failed, Error opening file"
                color = FAIL
            except:
                info = "Failed, Unknown Error"
                color = FAIL
        
        # the taget is a symbolic link
        else:
            if symbolicLink == localPath:
                info = "Linked"
                color = LINKED
            else:
                info = "Bad Link to:" + symbolicLink
                color = LINKED
                
        

        output = " " +color + element + ENDC + " " +("."*(width-6-len(element)-len(info)) ) + color + " ["+info+"]" + ENDC 
        if(verbose): output += "\n  " + path
        print output

################################# RESTORE COPY #################################
# This function copys the file from the the local file to the target file in   #
# effect "restoring" the file to the machine                                   #
################################################################################
def restoreCopy(localFile,targetFile):
    shutil.copyfile(localFile,targetFile)

################################## BACKUP COPY #################################
# This function copys the target file into the local directory in order to     #
# back it up so that it is saved in the repo                                   #
################################################################################
def backupCopy(localFile, targetFile):
    shutil.copyfile(targetFile,localFile)
################################# NULL FUNCTION ################################
# This function does nothing and is ment to be used for when the user just     #
# wants to check to see if the file is different or the same                   #
################################################################################
def nullFunction(localFile,targetFile):
    pass

def linkFiles(localFile,targetFile):
    os.symlink(localFile,targetFile)


## the main statement that handles the arguments and calls the apropriate functions
if __name__ == "__main__":
    if len(sys.argv) == 2:
        if sys.argv[1] == "backup":
            cycleFiles("Beginning Backup", backupCopy, verbose=False, sucessMessage="Backed Up")
        elif sys.argv[1] == "restore":
            cycleFiles("Beginning Restore", restoreCopy, verbose=False, sucessMessage="Extracted")
        elif sys.argv[1] == "link":
            if len(sys.argv == 3):
                cycleFiles("Linking Files", linkFile, verbose=True, sucessMessage="Linked")
            else:
                print "You need to specify a file you want to have linked"
    else:
        cycleFiles("Beginning Check", nullFunction, verbose=False, sucessMessage="Different Files")