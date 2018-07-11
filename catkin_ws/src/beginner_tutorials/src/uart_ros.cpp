#include <ros/ros.h> 
#include <ros/time.h>
#include <serial/serial.h>  //ROS已经内置了的串口包 
#include <std_msgs/String.h> 
#include <std_msgs/Empty.h> 

serial::Serial ser; //声明串口对象

/*
void write_callback(const std_msgs::String::ConstPtr& msg) 
{ 
    ROS_INFO_STREAM("Writing to serial port" <<msg->data); 
    ser.write(msg->data);   //发送串口数据 
} 

*/

int main (int argc, char** argv) 
{ 

	
    //初始化节点 
    ros::init(argc, argv, "uart_ros"); 
    //声明节点句柄 
    ros::NodeHandle nh; 
  
  
    //订阅主题，并配置回调函数 
//    ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback); 
    //发布主题 
    ros::Publisher read_pub = nh.advertise<std_msgs::String>("read", 1000); 
  
	

try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort("/dev/ttyTHS2"); 
        ser.setBaudrate(9600); 
        serial::Timeout to = serial::Timeout::simpleTimeout(1000); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("Unable to open port "); 
        return -1; 
    } 

 if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("Serial Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 

 ros::Rate loop_rate(50); 
    while(ros::ok()) 
    { 
  
        if(ser.available()){ 
        
            ROS_INFO_STREAM("Reading from serial port"); 
            std_msgs::String result; 
            result.data = ser.read(ser.available()); 
            ROS_INFO_STREAM("Read:\n " << result.data); 
            read_pub.publish(result); 
        } 

        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
  
    } 
} 





























































