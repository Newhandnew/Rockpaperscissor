#include "../include/rockpaperscissor_node.hpp"
#include "ros/ros.h"
#include <std_msgs/String.h>

using namespace std;
using namespace cv;
//namespace enc = sensor_msgs::image_encodings;

rockpaperscissor_node::rockpaperscissor_node(int argc, char **argv) {
  ros::init(argc, argv, "rockpaperscissor");
  ros::NodeHandle n;
  resultPublisher = n.advertise<std_msgs::String>("/rockpaperscissor_node/result", 10);
  imageSubscriber = n.subscribe("/camera/image_raw", 10, &rockpaperscissor_node::imageReceiveHandler, this);  ///usb_cam/image_raw"
  ROS_WARN("Rockpaperscissor_node started!");
  
  namedWindow("rockpaperscissor", CV_WINDOW_AUTOSIZE);
  cascade_paper_name = "/home/new/catkin_ws/src/rockpaperscissor/XML/paper.xml"; //PALM - Paper
  cascade_rock_name = "/home/new/catkin_ws/src/rockpaperscissor/XML/rock.xml"; //FIST - Rock
  cascade_scissor_name = "/home/new/catkin_ws/src/rockpaperscissor/XML/scissor.xml"; //SCISSOR

  storage = 0;
  storage = cvCreateMemStorage(0);

  cascade_paper = (CvHaarClassifierCascade*)cvLoad( cascade_paper_name, 0, 0, 0 );
  cascade_rock = (CvHaarClassifierCascade*)cvLoad( cascade_rock_name, 0, 0, 0 );
  cascade_scissor = (CvHaarClassifierCascade*)cvLoad( cascade_scissor_name, 0, 0, 0 );
}
// bool SHOW_CV_WINDOW;
// string IMAGE_TOPIC;

// cv_bridge::CvImagePtr bridge;
// IplImage * frame = 0;
// ImageScanner scanner;
// ros::Publisher code;
// uint message_sequence = 0;

void rockpaperscissor_node::imageReceiveHandler(const sensor_msgs::ImageConstPtr &image) {
  try {
    bridge = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
  }
  catch (cv_bridge::Exception& e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
    return;
  }  
  Mat cvMatrix = bridge->image;
  // IplImage *cv_image = NULL;
  // try
  // {
  //   cv_image = bridge_.imgMsgToCv(image, "bgr8");
  // }
  // catch (cv_bridge::Exception& e)
  // {
  //   ROS_ERROR("error");
  // }
 
  IplImage img = IplImage(cvMatrix);

  CvPoint pt1, pt2;
  float scale = 1.1;
  int i;
  int max_palm = 0, max_fist = 0, max_scissor = 0;
  int valueP =0, valueR = 0, valueS = 0;
  char currentResult;
  const CvFont fontText=cvFont(4, 3);
  char outputText[10];

  cvClearMemStorage( storage );
  /*** PALM ***/
   if(cascade_paper){
       CvSeq* faces = cvHaarDetectObjects(
               &img,
               cascade_paper,
               storage,
               scale, 2, CV_HAAR_DO_CANNY_PRUNING,
               cvSize(20, 20) );
       for( i = 0; i < (faces ? faces->total : 0); i++ )
       {
           CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
           pt1.x = r->x*scale;
           pt2.x = (r->x+r->width)*scale;
           pt1.y = r->y*scale;
           pt2.y = (r->y+r->height)*scale;
           if (abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y) > max_palm)
               max_palm = abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y);
           cvRectangle( &img, pt1, pt2, CV_RGB(200, 0, 0), 1, 8, 0 );
           // show score on image
           sprintf(outputText,"%d",max_palm);
           cvPutText(&img, outputText, pt1, &fontText, cvScalar(200, 0, 0));
       }
   }
   /*** FIST ***/
   if(cascade_rock){
       CvSeq* faces = cvHaarDetectObjects(
               &img,
               cascade_rock,
               storage,
               scale, 2, CV_HAAR_DO_CANNY_PRUNING,
               cvSize(20, 20) );
       for( i = 0; i < (faces ? faces->total : 0); i++ )
       {
           CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
           pt1.x = r->x*scale;
           pt2.x = (r->x+r->width)*scale;
           pt1.y = r->y*scale;
           pt2.y = (r->y+r->height)*scale;
           if (abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y) > max_fist)
               max_fist = abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y);
           //fprintf(stderr,"%d\n", max_fist);
           cvRectangle( &img, pt1, pt2, CV_RGB(64, 64, 255), 1, 8, 0 );

           sprintf(outputText,"%d",max_fist);
           cvPutText(&img, outputText, pt1, &fontText, cvScalar(100, 100, 0));
       }
   }

  // ** SCISSORS **
  if(cascade_scissor){
      CvSeq* faces = cvHaarDetectObjects(
              &img,
              cascade_scissor,
              storage,
              scale, 2, CV_HAAR_DO_CANNY_PRUNING,
              cvSize(20, 20));
      for( i = 0; i < (faces ? faces->total : 0); i++ )
      {
          CvRect* r = (CvRect*)cvGetSeqElem( faces, i );
          pt1.x = r->x*scale;
          pt2.x = (r->x+r->width)*scale;
          pt1.y = r->y*scale;
          pt2.y = (r->y+r->height)*scale;
          if (abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y) > max_scissor)
              max_scissor = abs(pt1.x - pt2.x) * abs(pt1.y - pt2.y);
          //fprintf(stderr,"%d\n", max_scissor);
          cvRectangle( &img, pt1, pt2, CV_RGB(255, 255, 128), 1, 8, 0 );

          sprintf(outputText,"%d",max_scissor);
          cvPutText(&img, outputText, pt1, &fontText, cvScalar(0, 100, 100));
      }
  }

  // evaluate result
  // 1:rock(>10000) 2:paper(>250000) 3:scissor(<60000)
  valueR = max_fist - 10000;
  valueP = max_palm - 25000;
  if(valueR > 0 && valueR < 11000) {
      if(valueP > 0) {
          if (valueR > valueP)
              currentResult = 1;
          else
              currentResult = 2;
      }
      else
          currentResult = 1;
  }
  else {
      if(valueP > 0)
          currentResult = 2;
      else if(abs(max_scissor - 90000) < 30000)
          currentResult = 3;
      else
          currentResult = 0;
  }
  // debounce process
  if(lastResult == currentResult) {
    result = currentResult;
  }
  else {
    lastResult = currentResult;
  }
  // publish result
  std_msgs::String msg;
  char message[7];
  sprintf(message, "%d", result);
  msg.data = message;//ss.str();
  resultPublisher.publish(msg);

 	cv::imshow("rockpaperscissor", cvMatrix);
  cv::waitKey(1);
}


