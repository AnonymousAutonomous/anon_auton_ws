#include "ros/ros.h"
#include "std_msgs/String.h"

void dirCallback(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("%s", msg->data.c_str());
}

int main (int argc, char** argv) {
  ros::init(argc, argv, "bystander");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("larry", 1000, dirCallback);

  ros::spin();

  return 0;
}
