#include "../include/rockpaperscissor_node.hpp"
#include "ros/ros.h"

int main(int argc, char **argv)
{
  rockpaperscissor_node test(argc, argv);

  
  // scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1); 

  // code = n.advertise<zbar_ros::Marker>("markers", 10);
  // ros::Subscriber sub = n.subscribe(IMAGE_TOPIC, 10, imageReceiver);
  ros::Rate rate(1);
  while(ros::ok()) {
  	ros::spinOnce();
  	rate.sleep();
  }

  return 0;
}