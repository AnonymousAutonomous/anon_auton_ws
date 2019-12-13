#include "ros/ros.h"
#include "std_msgs/Empty.h"

int main (int argc, char** argv) {
  ros::init(argc, argv, "sender");
  ros::NodeHandle nh;
  ros::Publisher chat_pub = nh.advertise<std_msgs::Empty>("chatter1", 1000);
  ros::Rate loop_rate(10);

  while (ros::ok()) {
    std_msgs::Empty msg;

    ROS_INFO("SENT!");

    chat_pub.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
