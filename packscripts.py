#!/usr/bin/python

import json
import shutil
import os
import sys
import filecmp

json_data=open('scriptconfig')

data = json.load(json_data)

json_data.close()

# some simple colors for the terminal
OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
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



def cycleFiles(beginningComment, functiontorun, verbose=False, sucessMessage="Sucess"):
    (width, height) = getTerminalSize()

    print beginningComment

    for element in data:
        output = ""
        info = ""
        path = os.path.expanduser(data[element])

        filesDiffer = True

        try:
            filesDiffer = not filecmp.cmp(path,"configs/"+element)
        except:
            pass

        info = sucessMessage
        color = OKGREEN
        try:
            if filesDiffer:
                functiontorun("configs/"+element,path)
            else:
                info = "No Change"
                color = WARNING
        except IOError:
            info = "Failed, Error opening file"
            color = FAIL
        except:
            info = "Failed, Unknown Error"
            color = FAIL
                
        

        output = color + element + ENDC + " " +("."*(width-5-len(element)-len(info)) ) + color + " ["+info+"]" + ENDC 
        if(verbose): output += "\n  " + path
        print output

def restoreCopy(localFile,targetFile):
    shutil.copyfile(localFile,targetFile)
def backupCopy(localFile, targetFile):
    shutil.copyfile(targetFile,localFile)
def nullFunction(localFile,targetFile):
    pass

def backup():
    (width, height) = getTerminalSize()

    print "Beginning Backup"

    for element in data:

        output = ""
        info = ""
        path = os.path.expanduser(data[element])

        filesDiffer = True

        try:
            filesDiffer = not filecmp.cmp(path,"configs/"+element)
        except:
            pass

        info = "Sucess"
        color = OKGREEN
        try:
            if filesDiffer:
                shutil.copyfile(path,"configs/"+element)
            else:
                info = "No Change"
                color = WARNING
        except IOError:
            info = "Failed, Error opening file"
            color = FAIL
        except:
            info = "Failed, Unknown Error"
            color = FAIL

        output = color + element + ENDC + " " +("."*(width-5-len(element)-len(info)) ) + color + " ["+info+"]" + ENDC + "\n  " + path
        print output
    #os.system("git add configs/*")

def restore():
    (width, height) = getTerminalSize()

    print "Beginning Restore"

    for element in data:
        output = ""
        info = ""
        path = os.path.expanduser(data[element])

        filesDiffer = True

        try:
            filesDiffer = not filecmp.cmp(path,"configs/"+element)
        except:
            pass

        info = "Unpacked"
        color = OKGREEN
        try:
            if filesDiffer:
                shutil.copyfile("configs/"+element,path)
            else:
                info = "No Change"
                color = WARNING
        except IOError:
            info = "Failed, Error opening file"
            color = FAIL
        except:
            info = "Failed, Unknown Error"
            color = FAIL
                
        

        output = color + element + ENDC + " " +("."*(width-5-len(element)-len(info)) ) + color + " ["+info+"]" + ENDC #+ "\n  " + path
        print output

if __name__ == "__main__":
    if len(sys.argv) == 2:
        if sys.argv[1] == "backup":
            cycleFiles("Beginning Backup", backupCopy, verbose=False, sucessMessage="Backed Up")
        elif sys.argv[1] == "restore":
            cycleFiles("Beginning Restore", restoreCopy, verbose=False, sucessMessage="Extracted")
    else:
        cycleFiles("Beginning Check", nullFunction, verbose=False, sucessMessage="Different Files")