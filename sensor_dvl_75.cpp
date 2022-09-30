#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <string>         // std::string
#include <sstream>
#include <iostream>
#include<vector>

serial::Serial ser;
using namespace std;


int main (int argc, char** argv){
    ros::init(argc, argv, "sensor_dvl_75");
    ros::NodeHandle nh;

    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000);

    try
    {
        ser.setPort("/dev/ttyUSB0");
        ser.setBaudrate(115200);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        ser.setTimeout(to);
        ser.open();
    }
    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open port ");
        return -1;
    }

    if(ser.isOpen()){
        ROS_INFO_STREAM("Serial Port initialized");
    }else{
        return -1;
    }

    ros::Rate loop_rate(5);
    while(ros::ok()){

        ros::spinOnce();

        if(ser.available()){
            ROS_INFO_STREAM("Reading from serial port");
            std_msgs::String result;
            result.data = ser.read(ser.available());

            string my_str =  result.data ;
            vector<string> result2;
            stringstream s_stream(my_str); //create string stream from the string
        while(s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma
            result2.push_back(substr);
            }
            //cout << result2.size() << endl;
        for(int i = 1; i < 13; i++) {    //print all splitted strings
          //cout << result2.at(i) << endl;
          }
          //for(int i = 0; result2.size(); i++) {    //print all splitted strings
          //    cout << result2.at(i) << endl;
          //  }
          ROS_INFO_STREAM("Read: " << result.data);
        }
        loop_rate.sleep();

    }
}
