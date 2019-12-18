#include <iostream>
#include <chrono> 
#include <future>

#include "ros/ros.h"
#include "sensor_msgs/Image.h"
#include "std_msgs/String.h"
#include <string>

ros::Publisher george; //the variable formerly known as signal
ros::Publisher debug;

// pivots handled as bwd then pivot in queue 
const std::string PIVOTR        = "cBstomf060r060"; 
const std::string PIVOTL        = "cBstomr060f060";
const std::string FAST_PIVOTR   = "cBstomf100r100"; 
const std::string FAST_PIVOTL   = "cBstomr100f100";
const std::string TURNR         = "cBstomf080f050";
const std::string TURNL         = "cBstomf050f080";
const std::string FWD           = "cBstomf055f055";
const std::string SLIGHTR       = "cBstomf070f050";
const std::string SLIGHTL       = "cBstomf050f070";
const std::string GO            = "cDstomf055f055";

uint old_data[640 * 480]; // TODO FIXME: switch to imagewidth * imageheight and make those const?

bool data_read = false;


bool inTop(int counter);
bool inLeft(int counter);
bool inRight(int counter);
bool inMiddle(int counter);
unsigned int topBandWidth = 120;
unsigned int sideBandWidth = 180;
unsigned int imageWidth = 640; // 640 INSTEAD OF 720 IT'S LYING DON'T TRUST IT
unsigned int imageHeight = 480;
unsigned int threshold = 200;
double side_percent_threshold = 0.20;
double top_percent_threshold = 0.20;
bool favorRight = false; 
double num_middle_pixels = 100800;

const int not_stuck_threshold = 550000; 

void chatterCallBack(const sensor_msgs::Image& view);

std::string GetLineFromCin() {
    std::string line; 
    std::getline(std::cin, line); 
    return line;
}


int main(int argc, char** argv)
{
    ros::init(argc, argv, "camera_process");
    
    ros::NodeHandle Iris; //subscriber
    ros::NodeHandle Cornea; //publisher
    ros::Subscriber Handle = Iris.subscribe("/cv_camera/image_mono", 10, chatterCallBack); // 10 was 1000
    //george = Cornea.advertise<std_msgs::String>("cameron", 1000);
    george = Iris.advertise<std_msgs::String>("cameron", 10); // 10 was 1000
    //ros::spin();
    debug = Iris.advertise<std_msgs::String>("cam_debug", 10);

    // Fee copied over from simple_motors trigger 
    ros::Rate delay_rate(5);

    std::string cmd = "";
    //int value; 

    auto future = std::async(std::launch::async, GetLineFromCin);

    while (ros::ok() &&cmd != "exit") {

        if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready) {
            auto cmd = future.get();

            future = std::async(std::launch::async, GetLineFromCin);
            std::cout << "CMD: " << cmd << std::endl;
            
if (cmd[0] == 't') {
                std::string value = "";
                value += cmd[1];
                value += cmd[2];
                value += cmd[3];
                threshold = std::stoi(value);
             } else if (cmd[0] == 'p') {
                std::string value = "";
                value += cmd[1];
                value += cmd[2];
                value += cmd[3];
                top_percent_threshold = std::stod(value) / 100.0; 
             } else if (cmd[0] == 's') {
                std::string value = "";
                value += cmd[1];
                value += cmd[2];
                value += cmd[3];
                side_percent_threshold = std::stod(value) / 100.0;
             } else {}

        // delay_rate.sleep(); // need this? 
}
            std::cout << "THRESHOLD: " <<   std::to_string(threshold) << std::endl;        
std::cout << "SIDE: " << std::to_string(side_percent_threshold) << std::endl;
std::cout << "TOP: " << std::to_string(top_percent_threshold) << std::endl;



            

            ros::spinOnce(); // ros::spin(); ?? 

//delay_rate.sleep();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void chatterCallBack(const sensor_msgs::Image& view)
{
    double topCount = 0;
    double leftCount = 0;
    double rightCount = 0;
    double middleCount = 0;
    
    imageWidth = view.width;
    imageHeight = view.height;

    int running_pixel_differences = 0;

    for(int i = 0; i < view.data.size(); i++) //grab top
    {
    //std::string command = "i: " + std::to_string(i);
    
        if(inTop(i))
        {
            //command += "T";
            topCount += (view.data[i] > threshold); 
        }
        if(inLeft(i))
        {
        //command += "L";
            leftCount += (view.data[i] > threshold);
        }
        if(inRight(i))
        {
        //command += "R";
            rightCount += (view.data[i] > threshold);
        }
        if (inMiddle(i)) {
            middleCount += (view.data[i] > threshold);
        }

        running_pixel_differences += (view.data[i] > old_data[i] ? view.data[i] - old_data[i] : old_data[i] - view.data[i]);
        // swap!
        old_data[i] = view.data[i];
        //command += "\n";
        //std_msgs::String stuff;
        //stuff.data = command;
        //george.publish(stuff); 
    }
    double leftAverage = leftCount/ (imageHeight*sideBandWidth);
    double rightAverage = rightCount / (imageHeight*sideBandWidth);
    double topAverage = topCount / (imageWidth*topBandWidth);
    double middleAverage = middleCount / num_middle_pixels;

    std::string result;


    /*
        forward = f
        backward = b
        stop = s 
        pivotr = R
        pivotl = L
        fwdl = l
        fwdr = r
        bwdl = e
        bwdr = i
        
    */

    if(topAverage > top_percent_threshold && leftAverage > side_percent_threshold && rightAverage > side_percent_threshold) // white in top, left, right
    {
        if (favorRight) {
            result = PIVOTR;
        } else {
            result = PIVOTL;
        }
        
    }
    else if(topAverage > top_percent_threshold && leftAverage > side_percent_threshold && rightAverage <= side_percent_threshold) // white in top, left
    {
        result = PIVOTR;
        favorRight = true;
    }
    else if(topAverage > top_percent_threshold && leftAverage <= side_percent_threshold && rightAverage > side_percent_threshold) // white in top, right
    {
        result = PIVOTL;
        favorRight = false;
    }
    else if(topAverage > top_percent_threshold && leftAverage <= side_percent_threshold && rightAverage <= side_percent_threshold) // white in top
    {
        if (favorRight) {
            result = PIVOTR;
        }
        else {
            result = PIVOTL;
        }
    }
    else if(topAverage <= top_percent_threshold && leftAverage > side_percent_threshold && rightAverage > side_percent_threshold) // white in left, right
    {
        result = FWD;
    }
    else if(topAverage <= top_percent_threshold && leftAverage > side_percent_threshold && rightAverage <= side_percent_threshold) // white in left
    {
        result = TURNR;
        favorRight = true;
    }
    else if(topAverage <= top_percent_threshold && leftAverage <= side_percent_threshold && rightAverage > side_percent_threshold) // white in right
    {
        result = TURNL;
        favorRight = false;
    }
    else // no white
    {
        result = GO; //f = fwd but with lower priority
    }
     
    if (middleAverage > top_percent_threshold) { // crossed over line --> backup and then pivot
        if (favorRight) {
            result = FAST_PIVOTR;
        }
        else {
            result = FAST_PIVOTL;
        }
    }


    std::string d;
    // TODO: comment out when ready to send to queue
    // 
    d = "Top: "; 
    d += std::to_string(topAverage);
    d += " Left: "; 
    d += std::to_string(leftAverage);
    d += " Right: "; 
    d += std::to_string(rightAverage);
    d += " Middle: "; 
    d += std::to_string(middleAverage);
    d += " Total Pixel Difference: "; 
    d += std::to_string(running_pixel_differences);
	d += "\nThreshold: "; 
    d += std::to_string(threshold);
    d += " Top Threshold: "; 
    d += std::to_string(top_percent_threshold);
    d += " Side Threshold: "; 
    d += std::to_string(side_percent_threshold);
    d += " Not Stuck Threshold: "; 
    d += std::to_string(not_stuck_threshold);

    if (running_pixel_differences < not_stuck_threshold) {
        result[2] = 'O'; // O for old image // TODO FIXME : add to the consts file
    }


    std_msgs::String stuff;
    stuff.data = result;
    george.publish(stuff);

    std_msgs::String other;
    other.data = d;
    debug.publish(other);

    data_read = true; // now compare values after the first frame 

}

bool inTop(int counter)
{
    return counter < (imageWidth*topBandWidth);
}

bool inLeft(int counter)
{
    return (counter % imageWidth) < sideBandWidth;
}

bool inRight(int counter)
{
    return (counter % imageWidth) > (imageWidth - sideBandWidth);
}

bool inMiddle(int counter)
{
    return !(inTop(counter) || inLeft(counter) || inRight(counter));
}
