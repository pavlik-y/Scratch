#!/usr/bin/env python

import sys
# import cgi
# import cgitb; cgitb.enable()  # for troubleshooting
# v = cgi.FieldStorage()

print("Content-type: text/text")
print()

f = open("/home/pi/christmas-lights/christmas.py", "rt")
sys.stdout.write(f.read())
