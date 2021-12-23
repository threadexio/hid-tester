#!/bin/python

infile = open("keys.txt", "r")

# usb key codes start at 4
i = 4

for line in infile.readlines():
    line = line.strip()

    # empty lines
    if line == "":
        continue

    # comments
    if line.startswith("#"):
        continue

    # key does not exist
    if line == "---":
        i += 1
        continue

    # split at | for multiple key names
    for key_name in line.split("|"):
        print('{ "%s", %d },' % (key_name.strip(), i))
    i += 1

infile.close()
