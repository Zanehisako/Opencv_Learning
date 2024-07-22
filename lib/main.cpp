
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <cstdio>
#include <opencv2/opencv.hpp>


int main() {
	//caputre the webcame
	cv::VideoCapture cap(0);
	// Create a named window
	cv::namedWindow("HUE/SATURATION/VALUE");

	int iLowH = 0;
	int iHighH = 179;

	int iLowS = 0; 
	int iHighS = 255;

	int iLowV = 0;
	int iHighV = 255;

	// Create a trackbar

	cv::createTrackbar("Low HUE", "HUE/SATURATION/VALUE", &iLowH, 179);
	cv::createTrackbar("High HUE", "HUE/SATURATION/VALUE", &iHighH, 179);


	cv::createTrackbar("Low SATURATION", "HUE/SATURATION/VALUE", &iLowS, 255 );
	cv::createTrackbar("High SATURATION", "HUE/SATURATION/VALUE", &iHighS, 255);
	cv::createTrackbar("Low VALUE", "HUE/SATURATION/VALUE", &iLowV, 255);
	cv::createTrackbar("High VALUE", "HUE/SATURATION/VALUE", &iHighV, 255);
	// Main loop

	while (true) {
		// Display the image and wait for a keypress
		cv::Mat image;
		bool succes = cap.read(image);
		if (!succes){
			printf("error reading the webcame footage");
			break;
		}
		cv::imshow("Video Original",image);
		cv::Mat imageHSV;
		cv::cvtColor(image,imageHSV,cv::COLOR_BGR2HSV);
		cv::imshow("Video HVS",imageHSV);
		cv::Mat imgThresholded;
		inRange(imageHSV, cv::Scalar(iLowH, iLowS, iLowV), cv::Scalar(iHighH, iHighS, iHighV), imgThresholded);

		//morphological opening (remove small objects from the foreground)
		cv::erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5, 5)) );
		dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) ); 

		//morphological closing (fill small holes in the foreground)
		dilate( imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(5, 5)) ); 
		erode(imgThresholded, imgThresholded, getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5)) );
		cv::imshow("Video Thresholded",imgThresholded);

		char key = cv::waitKey(1);
		if (key == 27) break; // Exit on ESC key
	}

	return 0;
}
