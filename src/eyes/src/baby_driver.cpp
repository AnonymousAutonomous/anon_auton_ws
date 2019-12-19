#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/String.h"
#include <string>
#include <sstream>

const std::string BWD =     "cBstomr055r055";
const std::string PIVOTR =  "cBstomf060r060";
const std::string PIVOTL =  "cBstomr060f060";
const std::string FAST_PIVOTR   = "cBstomf100r100"; 
const std::string FAST_PIVOTL   = "cBstomr100f100";

int counter = 0;
int old_image_counter = 0;

const int max_elapsed = 11;
const int lidar_stopped_max_elapsed = 10; // todo change me
bool in_bwd = false;
bool in_pivot = false;

bool in_beep = false;
bool bwd_after_beep = false;

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
    ss << jimothy.first.data << " AND " << jimothy.second.data;

    // check jimothy.first.data not just the command when it's passed in
    if (jimothy.first.data[1] == 'A') {
        ss << "I have the STOP command ";
        // stop the chair and start in beep
        if (!in_beep) {
            ss << "NOT in beep";
            // stop! reset in bwd
            in_bwd = false;
            in_pivot = false;
            in_beep = true;
            counter = 0;

            // keep the chair stopped 
            ss << jimothy.first.data; // STOP 
        }

        // in beep 
        else {
            ss << "IN BEEP";
            // count old images
            if (jimothy.second.data[2] == 'O') {
                old_image_counter++;
            }
            else {
                old_image_counter = 0; // otherwise reset? 
            }

            ss << " OLD IMAGE COUNTER: " << old_image_counter;
            // Start moving bwds
            if (old_image_counter > (2 * lidar_stopped_max_elapsed)) {
                // start moving bwd
                //in_beep = true; 
                in_bwd = true;
                in_pivot = false;
                // ss << command.data; 
                ss << " IN BWD SET ";
            }

            // beep but stay stopped
            else if (old_image_counter > lidar_stopped_max_elapsed) {
                // ss << command.data;
                system("aplay ~/anon_auton_ws/src/audio_files/car_horn.wav");
            }

            // stay stopped
            else { // TODO CHECK HERE
                //in_bwd = false;
                //in_pivot = false;
                //in_beep = false;
                ss << " stay stopped ";
                counter = 0;
            }

            // ONLY SET SS VALUES HERE
            // move backwards
            if (in_bwd) {
                ss << " IN BWD ";
                counter++;
                old_image_counter = 0;

                // transition to pivot
                if (counter > max_elapsed) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = true;
                    ss << " IN PIVOT SET ";
                }
                
                // continue moving backward
                else {
                    ss << BWD;
                }
            }

            // pivot
            if (in_pivot) {
                ss << " IN PIVOT ";
                counter++;
                ss << pivot_command;
                old_image_counter = 0;
                // if (pivot_direction == 'r') {
                //         ss << PIVOTR;
                //     } else {
                //         ss << PIVOTL;
                //     }
                // all done, reset!
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                    in_beep = false;
                    ss << " RESET ALL ";

                    old_image_counter = 0;
                }
            }

            if (!(in_bwd || in_pivot)) {
                ss << " NOT in bwd or pivot ";
                ss << jimothy.first.data; // send STOP still 
            }
        }
    }

    // not stopped so reset in_beep and old_image_counter
    else {
        ss << " NOT with STOP command ";
        in_beep = false;
        old_image_counter = 0;
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
