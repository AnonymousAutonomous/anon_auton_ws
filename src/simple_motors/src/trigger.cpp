#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>

int main(int argc, char** argv) {
  ros::init(argc, argv, "trigger");
  ros::NodeHandle nh;

  ros::Publisher trigger_pub = nh.advertise<std_msgs::String>("timmy", 1000);

  ros::Rate delay_rate(10);

  std::string cmd;

  while (ros::ok() && std::cin >> cmd && cmd != "exit") {
    std_msgs::String msg;
    msg.data = cmd;

    trigger_pub.publish(msg);
    ros::spinOnce();

    delay_rate.sleep();
  }

  return 0;
}
