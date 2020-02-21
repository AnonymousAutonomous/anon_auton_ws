#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash

anonautonN=$(whoami)

echo ${anonautonN}

cd ~/Desktop


# LIST IP ADDRESS

touch ~/Desktop/list_ip_address.desktop

cat > ~/Desktop/list_ip_address.desktop <<- EOM
[Desktop Entry]
Version=1.0
Exec=/home/$anonautonN/anon_auton_ws/src/bash_scripts/debug/ip_address.sh
Icon=/home/$anonautonN/anon_auton_ws/src/bash_scripts/icons/ip.png
Name=IP Address
GenericName=IP Address
Comment=List the IP Address of this Pi
Encoding=UTF-8
Terminal=true
Type=Application
Categories=Application;
EOM

chmod +x ~/Desktop/list_ip_address.desktop


# SIMPLE MOTORS

touch ~/Desktop/simple_motors.desktop

cat > ~/Desktop/simple_motors.desktop <<- EOM
[Desktop Entry]
Version=1.0
Exec=/home/$anonautonN/anon_auton_ws/src/bash_scripts/run/simple_motors.sh
Icon=/home/$anonautonN/anon_auton_ws/src/bash_scripts/icons/gear.png
Name=Simple Motors
GenericName=Simple Motors
Comment=Start roscore, simple motors trigger and listener, connect to Arduino
Encoding=UTF-8
Terminal=true
Type=Application
Categories=Application;
EOM

chmod +x ~/Desktop/simple_motors.desktop
