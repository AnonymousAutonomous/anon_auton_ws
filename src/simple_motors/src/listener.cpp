#include "ros/ros.h"
#include "std_msgs/String.h"

void callback(const std_msgs::String::ConstPtr& msg) {
  ROS_INFO("RECEIVED: [%s]", msg->data.c_str());
}

int main (int argc, char** argv) {
  ros::init(argc, argv, "listener");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("timmy", 1000, callback);

  ros::spin();

  return 0;
}
