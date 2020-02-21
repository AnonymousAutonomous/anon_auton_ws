#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash


USB0='none'
USB1='none'

#check_usb () {

#}


#assign_usb () {

#


USBS=$(ls -l /dev/serial/by-id)

echo "$USBS"

echo "\n --- GOT THE USBS --\n"


#arr=$(echo $USBS | tr " " "\n")
#arr=$USBS
IFS=' '
arr=($USBS)

echo "HIIIIII"

echo $arr
echo $arr[0]
echo ${arr[0]}
echo ${arr[1]}
echo ${arr[2]}
echo "HELP?"

echo "SIZE OF ARR"
echo ${#arr[@]}

# first USB port
echo ${arr[9]}  # item
echo ${arr[11]} # port number: ttyUSB0 or ttyUSB1

if [[ ${arr[11]} == *"USB0"* ]]
then
    if  [[ ${arr[9]} == "usb-1a86_USB2.0-Serial-if00-port0" ]]
    then
        echo "USB0 is ARDUINO"
    else
        echo "USB0 is LIDAR"
    fi
else 
    if  [[ ${arr[9]} == "usb-1a86_USB2.0-Serial-if00-port0" ]]
    then
        echo "USB1 is ARDUINO"
    else
        echo "USB1 is LIDAR"
    fi
fi 

# second USB port
echo ${arr[19]}
echo ${arr[21]}

#for (( i=0; i<${#arr[@]}; i++ ))
#do 
#    echo "$i: ${arr[$i]}"
#done



if [[ $USBS == *"USB0"* ]]; then
    echo "usb0!"
else
    echo "no usb0"
fi


