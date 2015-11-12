#include "ros/ros.h"
#include <std_msgs/String.h>
#include <stdio.h>
#include <string.h>
  
bool resultAvailable = true;
char finalResult;
ros::Publisher ttsPublisher;

void rockpaperscissorResultHandler(const std_msgs::String &msg) {
  if (resultAvailable == true) {
    if (msg.data == "1")
      finalResult = 1;
    else if(msg.data == "2")
      finalResult = 2;
    else if(msg.data == "3")
      finalResult = 3;
    else
      finalResult = 0;
  }
}

void messageToTTS(const std::string &message) {
    std_msgs::String msg;
    msg.data = message;//ss.str();
    ttsPublisher.publish(msg);

}

void judgeResult(char computerGesture, char playerGesture) {
  if(playerGesture == 0) {
    messageToTTS("看不清楚");
  }
  else {
    if(computerGesture == playerGesture)
      messageToTTS("平手");
    else if ((computerGesture == 1 && playerGesture == 3) || (computerGesture == 2 && playerGesture == 1) || (computerGesture == 3 && playerGesture == 2))
      messageToTTS("你輸了");
    else
      messageToTTS("你贏了");
  }

}


int main(int argc, char **argv)
{ 
  ros::init(argc, argv, "rockpaperscissor_main");

  ros::NodeHandle n;
  ttsPublisher = n.advertise<std_msgs::String>("ttsText", 10);
  ros::Publisher ericaCmdPublisher = n.advertise<std_msgs::String>("cmd_erica", 10);
  ros::Subscriber sub = n.subscribe("/rockpaperscissor_node/result", 10, rockpaperscissorResultHandler);

  char gesture;

  ROS_INFO("Rockpaperscissor main system started.");

  ros::Rate rate(0.2);

    //   std_msgs::String msg;
    // msg.data = message;//ss.str();

  while(ros::ok()) {
    // ttsPublisher.publish("你好");
    messageToTTS("剪刀  石頭  布");
    // take a random gesture
    sleep(1);
    gesture = rand()%3 + 1;
    // detect player's gesture
    sleep(1);
    resultAvailable = true;
    ros::spinOnce();
    judgeResult(gesture, finalResult);
    printf("your gueture:%d, computer gesture:%d \n", finalResult, gesture);
    resultAvailable = false;
    rate.sleep();

  }

  return 0;
}