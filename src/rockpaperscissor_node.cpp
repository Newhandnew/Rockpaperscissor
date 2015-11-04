#include "../include/rockpaperscissor_node.hpp"
#include "ros/ros.h"

using namespace std;
//namespace enc = sensor_msgs::image_encodings;

rockpaperscissor_node::rockpaperscissor_node(int argc, char **argv) {
  ros::init(argc, argv, "rockpaperscissor");
  ros::NodeHandle n;
  imageSubscriber = n.subscribe("/camera/image_raw", 10, &rockpaperscissor_node::imageReceiveHandler, this);  ///usb_cam/image_raw"
  ROS_WARN("Rockpaperscissor_node started!");
  printf("this is a test");
  namedWindow("rockpaperscissor", CV_WINDOW_AUTOSIZE);

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
  printf("this is a test");
  cv::Mat cv_matrix = bridge->image;
 //  int width = cv_matrix.cols; 
 //  int height = cv_matrix.rows;

	// uchar* raw = cv_matrix.ptr<uchar>(0);
	// Image scan_image(width, height, "Y800", raw, width * height);
	// int n = scanner.scan(scan_image); 

 //  if (n < 0) {
 //   	ROS_ERROR("Error occured while finding barcode");
 //   	return;
 //  }

 //  // extract results
 //  for(SymbolIterator symbol = scan_image.symbol_begin();
 //            symbol != scan_image.symbol_end();
 //            ++symbol) {
 //    std::stringstream ss;
 //  	//Publish msg on zbar topic 
	//   ss << symbol->get_data();
 //    msg.header.seq = message_sequence++;
 //    msg.header.stamp = ros::Time::now();
 //    msg.header.frame_id = image->header.frame_id;
	//   msg.data = ss.str();

 //    int x1 = width, y1 = height, x2 = 0, y2 = 0;

 //    for (int i = 0; i < symbol->get_location_size(); i++) {
 //      x1 = MIN(x1, symbol->get_location_x(i));
 //      y1 = MIN(y1, symbol->get_location_y(i));
 //      x2 = MAX(x2, symbol->get_location_x(i));
 //      y2 = MAX(y2, symbol->get_location_y(i));
 //    }

 //    msg.center_x = (x1 + x2) / 2; 
 //    msg.center_y = (y1 + y2) / 2; 
 //    msg.width = (x2 - x1); 
 //    msg.height = (y2 + y1); 

 //    if(SHOW_CV_WINDOW) {
 //      cv::rectangle(cv_matrix, cv::Point(x1,y1), cv::Point(x2,y2), cv::Scalar(255), 2);
 //    }

	//   code.publish(msg);
 //  }

 //  //Show image in CV window
 //  if(SHOW_CV_WINDOW) {
 //  //frame = new IplImage(bridge->image);
   	cv::imshow("rockpaperscissor", cv_matrix);
    cv::waitKey(1);
 //  	cv::waitKey(1);
	// //cvReleaseImage
 //  }

 // bridge.release();
}

int main(int argc, char **argv)
{
  rockpaperscissor_node test(argc, argv);
  ROS_ERROR("Rockpaperscissor_node started!");

  
  // scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1); 

  // code = n.advertise<zbar_ros::Marker>("markers", 10);
  // ros::Subscriber sub = n.subscribe(IMAGE_TOPIC, 10, imageReceiver);

  ros::spin();

  return 0;
}
