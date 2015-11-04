#ifndef ROCKPAPERSCISSOR_H
#define ROCKPAPERSCISSOR_H

#include "ros/ros.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv/cv.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <sensor_msgs/image_encodings.h>

using namespace cv;

class rockpaperscissor_node {

public:
    rockpaperscissor_node(int argc, char **argv);
    void run();
    bool Stop;


private:
    //HAND GESTURE
    int result;

    cv_bridge::CvImagePtr bridge;
    CvCapture *capture;
    IplImage *frame;
    CvMemStorage* storage;
    CvHaarClassifierCascade* cascade_paper;
    CvHaarClassifierCascade* cascade_rock;
    CvHaarClassifierCascade* cascade_scissor;

    const char* cascade_paper_name;
    const char* cascade_rock_name;
    const char* cascade_scissor_name;

    IplImage* detectAndDraw(IplImage *img);
    //QImage getQImageFromIplImage(IplImage *frame);

    ros::Publisher resultPublisher;
    ros::Subscriber imageSubscriber;
    void imageReceiveHandler(const sensor_msgs::ImageConstPtr &image);
};

#endif // ROCKPAPERSCISSOR_H
