#!/bin/bash

echo "Starting the PyHP FastCGI Socket..."
#cgi-fcgi -connect 127.0.0.1:2019 cmake_build_debug/PyHP.fcgi
sudo rm /tmp/phyp-fcgi.sock
cgi-fcgi -start -connect /tmp/phyp-fcgi.sock ./cmake-build-debug/PyHPCGI
sudo chmod 777 /tmp/phyp-fcgi.sock
sudo chown www-data:www-data /tmp/phyp-fcgi.sock