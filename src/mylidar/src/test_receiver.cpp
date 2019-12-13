#include "ros/ros.h"
#include "std_msgs/Empty.h"

void chatterCallback(const std_msgs::Empty::ConstPtr& msg) {
  ROS_INFO("RECEIVED!");
}

int main (int argc, char** argv) {
  ros::init(argc, argv, "receiver");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("chatter2", 1000, chatterCallback);

  ros::spin();

  return 0;
}
