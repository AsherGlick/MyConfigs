import json
import shutil
import os
import sys
#from pprint import pprint
json_data=open('scriptconfig')

data = json.load(json_data)
#pprint(data)
json_data.close()


OKGREEN = '\033[92m'
WARNING = '\033[93m'
FAIL = '\033[91m'
ENDC = '\033[0m'





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


def backup():
	(width, height) = getTerminalSize()

	print "Beginning Backup"

	for element in data:

		output = ""
		info = ""
		path = os.path.expanduser(data[element])

		#print element+ "\n--| " + path

		try:
			shutil.copyfile(path,"configs/"+element)
		except IOError:
			info = "Failed, Error opening file"
			color = FAIL
		except:
			info = "Failed, Unknown Error"
			color = FAIL
		else:
			info = "Sucess"
			#print " "*20 + " | " + "SUCESS"
			color = OKGREEN

		output = color + element + ENDC + " " +("."*(width-5-len(element)-len(info)) ) + color + " ["+info+"]" + ENDC + "\n  " + path
		print output 	
