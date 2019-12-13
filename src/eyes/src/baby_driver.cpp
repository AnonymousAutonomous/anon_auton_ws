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


void chatterCallBackCamera(const std_msgs::String& commands)
{
	jimothy.second = commands;
	/*
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
	*/
	std::stringstream ss;
	//ss << "< " << jimothy.first.data << " | " << jimothy.second.data << " >";
	if (jimothy.first.data.length() < 2 && jimothy.second.data.length() < 2) ss /*<< " COMMAND: "*/ << "NULL";
	else if (jimothy.first.data.length() < 2) {
        if (jimothy.second.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            in_pivot = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.second.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.second.data[6] != jimothy.second.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.second.data[6] == 'f') {
                            pivot_direction = 'r';
                      }
                      else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.second.data;
                }

            }
            
        }
        
    }
	else if (jimothy.second.data.length() < 2) {
        if (jimothy.first.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.first.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (max_elapsed * 3)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.first.data[6] != jimothy.first.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.first.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }
                else {
                    ss /*<< " COMMAND: "*/ << jimothy.first.data;
                }

            }
            
        }



	} else if (jimothy.first.data[1] < jimothy.second.data[1]) {
        if (jimothy.first.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.first.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.first.data[6] != jimothy.first.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.first.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.first.data;
                }

            }
            
        }
        
        
    }
	else {
        if (jimothy.second.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.second.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.second.data[6] != jimothy.second.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.second.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.second.data;
                }

            }
            
        }
    }

	std_msgs::String msg;
	msg.data = ss.str();
	chairles.publish(msg);
}

void chatterCallBackLidar(const std_msgs::String& commands)
{
	jimothy.first = commands;
	/*
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
	*/
        std::stringstream ss;
	//ss << "< " << jimothy.first.data << " | " << jimothy.second.data << " >";
	if (jimothy.first.data.length() < 2 && jimothy.second.data.length() < 2) ss /*<< " COMMAND: "*/ << "cQstomf050f050";
	else if (jimothy.first.data.length() < 2) {
        if (jimothy.second.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.second.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.second.data[6] != jimothy.second.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.second.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.second.data;
                }

            }
            
        }
        
    }
	else if (jimothy.second.data.length() < 2) {
        if (jimothy.first.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.first.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.first.data[6] != jimothy.first.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.first.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }
                else {
                    ss /*<< " COMMAND: "*/ << jimothy.first.data;
                }

            }
            
        }



	} else if (jimothy.first.data[1] < jimothy.second.data[1]) {
        if (jimothy.first.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.first.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.first.data[6] != jimothy.first.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.first.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.first.data;
                }

            }
            
        }
        
        
    }
	else {
        if (jimothy.second.data[1] == 'A') {
            // stop! reset in bwd
            in_bwd = false;
            counter = 0;

            ss /*<< " COMMAND: "*/ << jimothy.second.data;
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
                if (pivot_direction == 'r') {
                        ss << PIVOTR;
                    } else {
                        ss << PIVOTL;
                    }
                if (counter > (3 * max_elapsed)) {
                    counter = 0;
                    in_bwd = false;
                    in_pivot = false;
                }
            }
            if (!(in_bwd || in_pivot)) {
                    // if pivot command - back up and then pivot
                if (jimothy.second.data[6] != jimothy.second.data[10]) {
                      in_bwd = true;
                      ss << BWD;     
                      if (jimothy.second.data[6] == 'f') {
                            pivot_direction = 'r';
                      } else {
                            pivot_direction = 'l';
                      } 
                }

                else {
                    ss /*<< " COMMAND: "*/ << jimothy.second.data;
                }

            }
            
        }
    }

	std_msgs::String msg;
	msg.data = ss.str();
	chairles.publish(msg);
}
