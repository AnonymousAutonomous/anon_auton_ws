#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash

USBS=$(ls -l /dev/serial/by-id)

echo "$USBS"

echo "\n --- GOT THE USBS --\n"


arr=$(echo $USBS | tr " " "\n")


echo "HIIIIII"

$arr[10]
$arr[12]

$arr[21]
$arr[23]

for x in $arr
do 
    echo "\"$x\""
done
# IFS=',/n ' read -r -a array <<< "$USBS[2]"

#for element in "${array[@]}"
#do
    echo "NEW ELEMENT $element"
#done


if [[ $USBS == *"USB0"* ]]; then
    echo "usb0!"
else
    echo "no usb0"
fi


