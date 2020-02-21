#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash

LIDAR_STR="usb-Silicon_Labs_CP2102_USB_to_UART_Bridge_Controller_0001-if00-port0"
ARDUINO_STR="usb-1a86_USB2.0-Serial-if00-port0"

if ! pgrep -f "rosout" > /dev/null; then 
	##/bin/bash ~/anon_auton_ws/src/bash_scripts/roscore.sh;
	xterm -geometry 75x25+0+300 -e "roscore" &
    xtermroscoreid=$(pgrep --newest xterm)
    echo "PID of xterm w/ roscore is $xtermroscoreid"
	## roscore
fi

while ! pgrep -f "rosout" > /dev/null; do
	sleep 0.1
done

pgrep -f "rosout"
echo "ROSCORE IS RUNNING"


USBS=$(ls -l /dev/serial/by-id)

if [ -z "$USBS" ]
then
    echo "ERROR: No USB device connected."
else
    arr=($USBS)
    n=${#arr[@]}


    for (( i=0; i<${#arr[@]}; i++ ))
    do 
        echo "$i: ${arr[$i]}"
        
        if [ ${arr[$i]} == $ARDUINO_STR ]
        then
            
            usb_num="${arr[(($i + 2))]: -1}"
            echo "Arduino is connected to USB$usb_num"
            xterm -geometry 75x25+1000+300 -e "echo '-----ARDUINO CONNECTION----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun rosserial_python serial_node.py /dev/ttyUSB$usb_num;" & 
            xtermarduinoid=$(pgrep --newest xterm)
            echo "PID of xterm w/ Arduino serial is $xtermarduinoid"
            
        fi
    done

    #itemi=9
    #usbi=11
    
    #while [ $usbi -lt $n ]; do
        #echo "$usbi : ${arr[$itemi]}"
        #if [ ${arr[$itemi]} == $ARDUINO_STR ]
        #then
            #usb_num="${arr[$usbi]: -1}"
            #echo "Arduino is connected to USB$usb_num"
            #xterm -geometry 75x25+1000+300 -e "echo '-----ARDUINO CONNECTION----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun rosserial_python serial_node.py /dev/ttyUSB$usb_num;" &
#xtermarduinoid=$(pgrep --newest xterm)
        #fi
        #itemi=$((itemi+10))
       # usbi=$((usbi+10))        
    
    #done    
fi






#echo "${USBS%ttyUSB0*}"
#echo "${USBS%*ttyUSB0}"

##cd ~/anon_auton_ws
##source devel/setup.bash
###xterm -e echo "-----Simple Motors Trigger-----"; rosrun simple_motors trigger;
###xterm -e echo "-----Simple Motors LISTENER-----"; rosrun simple_motors listener;
xterm -geometry 75x25+500+300 -e "echo '-----Simple Motors TRIGGER-----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun simple_motors trigger;" &
xtermtrigid=$(pgrep --newest xterm)
echo "PID of xterm w/ SimpleMotors Trigger is $xtermtrigid"
xterm -geometry 75x25+1000+300 -e "echo '-----Simple Motors LISTENER----'; cd ~/anon_auton_ws; source devel/setup.bash; rosrun simple_motors listener;" &
xtermlistenid=$(pgrep --newest xterm)
echo "PID of xterm w/ SimpleMotors Listener is $xtermlistenid"


read -p "Press any key to exit >" -n1 junk
echo
kill $xtermlistenid
kill $xtermtrigid
kill $xtermroscoreid
kill $xtermarduinoid

