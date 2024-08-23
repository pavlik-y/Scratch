- To install python binding:
`sudo pip install rpi_ws281x`
- To setup scripts copy scripts into /home/pavely/lights and copy
  pi_display into parent folder
- To start script after boot add
`sudo python /home/pavely/lights/controller.py &`
to /etc/rc.local
- To install web server
`sudo apt-get install apache2`
- Need to