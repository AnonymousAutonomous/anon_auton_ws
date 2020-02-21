#!/usr/bin/env bash

## Launch ip_address.sh and capture its standard error output
/home/anonymous4/anon_auton_ws/src/bash_scripts/ip_address.sh 2>/home/anonymous4/anon_auton_ws/src/bash_scripts/ip_address.log
read -p "Press any key to exit > " -n1 junk
echo
