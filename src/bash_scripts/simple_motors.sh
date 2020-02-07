#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash


if ! pgrep -f "rosout" > /dev/null; then 
	##/bin/bash ~/anon_auton_ws/src/bash_scripts/roscore.sh;
	xterm -geometry 75x25+0+300 -e "roscore" &
	## roscore
fi

while ! pgrep -f "rosout" > /dev/null; do
	sleep 0.1
done

pgrep -f "rosout"
echo "ROSCORE IS RUNNING"

USBS=$((dmesg | grep 'now attached to ttyUSB') | tail -2)

ar=($USBS)


echo "${USBS%ttyUSB0*}"
echo "${USBS%*ttyUSB0}"

##cd ~/anon_auton_ws
##source devel/setup.bash
###xterm -e echo "-----Simple Motors Trigger-----"; rosrun simple_motors trigger;
###xterm -e echo "-----Simple Motors LISTENER-----"; rosrun simple_motors listener;
xterm -geometry 75x25+500+300 -e "echo '-----Simple Motors TRIGGER-----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun simple_motors trigger;" &
xterm -geometry 75x25+1000+300 -e "echo '-----Simple Motors LISTENER----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun simple_motors listener;" &


read -p "Press any key to exit >" -n1 junk
echo
