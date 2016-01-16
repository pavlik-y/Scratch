#!/usr/bin/env python

import sys
# import cgi
# import cgitb; cgitb.enable()  # for troubleshooting
# v = cgi.FieldStorage()

f = open("/home/pi/christmas-lights/christmas.py", "wt")
f.write(sys.stdin.read())
f.close()
