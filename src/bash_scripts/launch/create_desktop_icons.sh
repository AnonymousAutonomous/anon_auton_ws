#!/bin/bash

source ~/anon_auton_ws/devel/setup.bash

anonautonN=$(whoami)

echo ${anonautonN}

cd ~/Desktop

IP_FILE = $(cat <<-END
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
END
)

touch ~/Desktop/list_ip_address.desktop

echo ${IP_FILE} >> ~/Desktop/list_ip_address.desktop

chmod +x ~/Desktop/list_ip_address.desktop