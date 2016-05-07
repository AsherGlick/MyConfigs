# renamer .py takes all the files in the directory and opens up a text file with the names
# change all the names in the text file an then it goes back and updates all of the name of the actual files

# Why was this script written? because sometime you need a little more power then regex or you dont want to deal with
# creating a regex this one time

import os
import sys


def main(argc, argv):

    # Get the list of files
    directory = os.getcwd()
    if argc == 2:
        if os.path.isabs(argv[1]):
            directory = argv[1]
        else:
            directory = os.path.abspath(os.path.join(directory, argv[1]))
    print directory
    fileList = os.listdir(directory)
    fileList.sort()
    
    # create the directory if it does not exist
    tempPath = os.path.expanduser("~/.renamer/CURRENTDIR")
    tempDir = os.path.dirname(tempPath)
    if not os.path.exists(tempDir):
        os.makedirs(tempDir)


    # write all the filenames to the temporary file
    tempFile = open(tempPath, 'w')
    for filename in fileList:
        tempFile.write(filename+"\n")
    tempFile.close()

    # open sublime to modify the files
    os.system("subl -n -w "+tempPath)

    # read in the new names
    newnames = []
    tempFile = open(tempPath, 'r')
    newnames = tempFile.read().splitlines()

    # rename the old files
    for (i, newname) in enumerate(newnames):
        if (fileList[i] != newname):
            print fileList[i], " --> ", newname
            oldFile = os.path.join(directory, fileList[i])
            newFile = os.path.join(directory, newname)
            newDirectory = os.path.dirname(newFile)
            if not os.path.exists(newDirectory):
                print "making directory", newDirectory
                os.makedirs(newDirectory)
            os.rename(oldFile, newFile)

if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
