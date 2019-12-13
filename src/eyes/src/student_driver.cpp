#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/String.h"
#include <string>

ros::Publisher chairles;
void chatterCallBackCamera(const std_msgs::String& commands); 
void chatterCallBackLidar(const std_msgs::String& commands); 
std::pair<std_msgs::String, std_msgs::String> jimothy;

int main(int argc, char** argv)
{
	ros::init(argc, argv, "baby_driver");

	ros::NodeHandle bill; //subscriber
	ros::NodeHandle ted; //publisher

	ros::Subscriber Stick = bill.subscribe("/cameron", 1000, chatterCallBackCamera);
	ros::Subscriber Rope = bill.subscribe("/larry", 1000, chatterCallBackLidar);
	chairles = ted.advertise<std_msgs::String>("timmy" , 1000);
	ros::spin();
}


void chatterCallBackCamera(const std_msgs::String& commands)
{
	jimothy.second = commands;
	if(jimothy.first.data == "s")
	{
		chairles.publish(jimothy.first);
	}
	else if (jimothy.second.data != "")
	{
		chairles.publish(jimothy.second);
	}
	else
	{
		chairles.publish(jimothy.first);
	}
}

void chatterCallBackLidar(const std_msgs::String& commands)
{
	jimothy.first = commands;
	if(jimothy.first.data == "s")
	{
		chairles.publish(jimothy.first);
	}
	else if (jimothy.second.data != "")
	{
		chairles.publish(jimothy.second);
	}
	else
	{
		chairles.publish(jimothy.first);
	}
}
