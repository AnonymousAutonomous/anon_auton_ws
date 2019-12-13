#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

#define RAD2DEG(x) ((x)*180./M_PI)

void callback(const sensor_msgs::LaserScan::ConstPtr& scan) {
  int count = scan->scan_time / scan->time_increment;
  for (int i = 0; i < count; ++i) {
    float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
    ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
  }
}

int main (int argc, char** argv) {
  ros::init(argc, argv, "processor");
  ros::NodeHandle nh;
  ros::Subscriber sub = nh.subscribe<sensor_msgs::LaserScan>("scan_filtered", 1000, callback);

  ros::spin();

  return 0;
}
