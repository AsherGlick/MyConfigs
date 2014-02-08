#! /usr/bin/python3

print ("Standard Colors")

for x in range (30, 38):
    print ("\033[" + str(x) + "m##", sep='', end='')
print("\033[0m")

for x in range (30, 38):
    print ("\033[1;" + str(x) + "m##", sep='', end='')
print("\033[0m")

for x in range (40, 48):
    print ("\033[" + str(x) + "m  ", sep='', end='')
print("\033[0m")

# for y in range(0, 110):
#     print(str(y).zfill(3), " ", end='')
#     for x in range (0, 100):
#         print ("\033[" + str(y) + ";" + str(x) + "m##", sep='', end='')
#         print("\033[0m", end='')
#     print("\033[0m")

print ("\nxterm 256 background colors")

for x in range(0, 8):
    print ("\033[48;5;" + str(x) + "m  ", sep='', end='')
print("\033[0m")
for x in range(8, 16):
    print ("\033[48;5;" + str(x) + "m  ", sep='', end='')
print("\033[0m\n")

for x in range(0, 6):
    for y in range(0, 6):
        for z in range(0, 6):
            print ("\033[48;5;" + str(16+(y*36)+(x*6)+z) + "m" + "  ", sep='', end='')
        print("\033[0m  ", end='')
    print("\033[0m")

print("\033[0m")

for x in range(232, 256):
    print ("\033[48;5;" + str(x) + "m  ", sep='', end='')

print("\033[0m")


print ("\nxterm 256 forground colors")

for x in range(0, 8):
    print ("\033[38;5;" + str(x) + "m##", sep='', end='')
print("\033[0m")
for x in range(8, 16):
    print ("\033[38;5;" + str(x) + "m##", sep='', end='')
print("\033[0m\n")

for x in range(0, 6):
    for y in range(0, 6):
        for z in range(0, 6):
            print ("\033[38;5;" + str(16+(y*36)+(x*6)+z) + "m" + "##", sep='', end='')
        print("\033[0m  ", end='')
    print("\033[0m")

print("\033[0m")

for x in range(232, 256):
    print ("\033[38;5;" + str(x) + "m##", sep='', end='')

print("\033[0m\n")