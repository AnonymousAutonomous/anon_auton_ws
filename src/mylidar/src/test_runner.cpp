#include "ros/ros.h"
#include "std_msgs/Empty.h"

ros::Publisher chat_pub;

void chatterCallback(const std_msgs::Empty::ConstPtr& msg) {
  ROS_INFO("RUNNING!");

  std_msgs::Empty emsg;
  chat_pub.publish(emsg);
}

int main (int argc, char** argv) {
  ros::init(argc, argv, "runner");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe("chatter1", 1000, chatterCallback);

  ros::NodeHandle oi;
  chat_pub = oi.advertise<std_msgs::Empty>("chatter2", 1000);

  ros::spin();

  return 0;
}
