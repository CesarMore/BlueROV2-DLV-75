#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

serial::Serial ser;
using namespace std;

string extractor(string trama1, int num_val, string inicio, char separador);
string extractor(string trama1, int num_val, string inicio ="X,0310", char separador = ',')
{
  int pos = 0, init = 0, new_pos = 0, end_pos = 0;
  int i = 0, occurrence = 0, len = 0;
  string tramaFinal;
  num_val += 1;
  pos = trama1.find(inicio);
  if(pos != -1){
    init = pos + inicio.size();
    end_pos = init;
    for(i = 0;i < num_val ;i++){
      end_pos = trama1.find(separador, end_pos + 1);
      if (init != -1){
        occurrence ++;
      }
      if(occurrence == (num_val-1)){
        len = end_pos - init;
        tramaFinal = trama1.substr(init+1, len-1);
        return(tramaFinal);
        break;
      }
    };
  }
  return("error");
}

int main (int argc, char** argv){
    ros::init(argc, argv, "dvl_75");
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
            ROS_INFO_STREAM("Read: " << extractor(result.data, 8)<<endl);
        }
        loop_rate.sleep();
    }
}
