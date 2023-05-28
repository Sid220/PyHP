#!/bin/bash

echo "Starting the PyHP FastCGI Socket..."
#cgi-fcgi -connect 127.0.0.1:2019 cmake_build_debug/PyHP.fcgi
sudo rm /var/run/pyhp/pyhp-cgi.sock

cgi-fcgi -start -connect /var/run/pyhp/pyhp-cgi.sock /usr/bin/pyhp-cgi

sudo chown www-data:www-data /var/run/pyhp/pyhp-cgi.sock
sudo chmod 660 /var/run/pyhp/pyhp-cgi.sock

echo -n "PID: "
lsof -Ua | grep "pyhp-cgi" | awk -F '  ' '{print $2}' | head -n 1 | sudo tee /var/run/pyhp/pyhp-cgi.pid