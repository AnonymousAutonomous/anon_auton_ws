#include "ros/ros.h"
#include "std_msgs/Empty.h"

int main (int argc, char** argv) {
  ros::init(argc, argv, "blinker");
  ros::NodeHandle nh;
  ros::Publisher flash_pub = nh.advertise<std_msgs::Empty>("flash", 1000);
  ros::Rate loop_rate(10);
  
  while (ros::ok()) {
    std_msgs::Empty msg;
    flash_pub.publish(msg);
    
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
