#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/String.h"
#include <string>
#include <sstream>

const std::string BWD =     "cBstomr055r055";
const std::string PIVOTR =  "cBstomf060r060";
const std::string PIVOTL =  "cBstomr060f060";
int counter = 0;
const int max_elapsed = 11;
bool in_bwd = false;
bool in_pivot = false;
char pivot_direction = 'r';
std::string pivot_command = PIVOTR;

ros::Publisher chairles;
void chatterCallBackCamera(const std_msgs::String& commands); 
void chatterCallBackLidar(const std_msgs::String& commands); 
std::pair<std_msgs::String, std_msgs::String> jimothy;

int main(int argc, char** argv)
{
    ros::init(argc, argv, "baby_driver");

    ros::NodeHandle bill; //subscriber
    ros::NodeHandle ted; //publisher

    ros::Subscriber Stick = bill.subscribe("cameron", 1000, chatterCallBackCamera);
    ros::Subscriber Rope = bill.subscribe("larry", 1000, chatterCallBackLidar);
    chairles = ted.advertise<std_msgs::String>("timmy" , 1000);
    ros::spin();
}

void handleCommand(std_msgs::String& command, std::stringstream& ss) {
    //std::stringstream ss;

    if (command.data[1] == 'A') {
            // stop! reset in_bwd
            in_bwd = false;
            in_pivot = false;
            counter = 0;

            ss << command.data;
        }
        else {
            // in the process of moving backward
            if (in_bwd) {
                counter++; 
                
                // transition to pivot
                if (counter > max_elapsed) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = true;
                }
                
                // continue moving backward
                else {
                    ss << BWD;
                }
            }
            if (in_pivot) {
                counter++;
                ss << pivot_command;
                // if (pivot_direction == 'r') {
                //         ss << PIVOTR;
                //     } else {
                //         ss << PIVOTL;
                //     }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (command.data[6] != command.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      pivot_command = command.data;
                      // if (command[6] == 'f') {
                      //       pivot_direction = 'r';
                      // }
                      // else {
                      //       pivot_direction = 'l';
                      // } 
                }

                else {
                    ss << command.data;
                }

            }
            
        }
}

void chatterCallBackCamera(const std_msgs::String& commands)
{
    jimothy.second = commands;
    std::stringstream ss;

    if (jimothy.first.data.length() < 2 && jimothy.second.data.length() < 2){
        ss << "NULL";
    // second data only
    } else if (jimothy.first.data.length() < 2) { // data only in second
        handleCommand(jimothy.second, ss);
    // first data only
    } else if (jimothy.second.data.length() < 2) {
        handleCommand(jimothy.first, ss);
    // first data takes priority
    } else if (jimothy.first.data[1] < jimothy.second.data[1]) {
        handleCommand(jimothy.first, ss);
    // second data takes priority
    } else {
        handleCommand(jimothy.second, ss);
    }

    std_msgs::String msg;
    msg.data = ss.str();
    chairles.publish(msg);
}

void chatterCallBackLidar(const std_msgs::String& commands)
{
    jimothy.first = commands;

    std::stringstream ss;
    
    if (jimothy.first.data.length() < 2 && jimothy.second.data.length() < 2) {
        ss << "cQstomf050f050";
    } else if (jimothy.first.data.length() < 2) {
        handleCommand(jimothy.second, ss);
    } else if (jimothy.second.data.length() < 2) {
        handleCommand(jimothy.first, ss);
    } else if (jimothy.first.data[1] < jimothy.second.data[1]) {
        handleCommand(jimothy.first, ss);
    } else {
        handleCommand(jimothy.second, ss);
    }

    std_msgs::String msg;
    msg.data = ss.str();
    chairles.publish(msg);
}
