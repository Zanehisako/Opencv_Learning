
#include "opencv2/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
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
		/*cv::Mat image;
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
*/
		//tamplate matching ie find an object in an image
		//the image that we are looking in 
		cv::Mat dino_game = cv::imread("E:/C_projects/bot/assets/dino game.png");
		//the image that we are looking for
		cv::Mat dino = cv::imread("E:/C_projects/bot/assets/dino.png");
		//the result of the tamplate matching with will be black and white 
		cv::Mat result ;

		cv::matchTemplate(dino_game,dino,result,cv::TM_CCOEFF_NORMED);
		//value to create the rectangel ie where is the result
		double minValue,maxValue;
		cv::Point minLoc,maxLoc;
		//this function returns the min/max value in the result matrix and the their position 
		cv::minMaxLoc(result,&minValue,&maxValue,&minLoc,&maxLoc);
		//creating the rect
		cv::Rect boundingBox(maxLoc, cv::Point(maxLoc.x + dino.cols, maxLoc.y + dino.rows));
		//the first parameter is where we are displaying the rect,the second is the color,lastly the thickness 
		cv::rectangle(dino_game, boundingBox, cv::Scalar(0, 255, 0), 2);

		cv::imshow("Match result ",dino_game);
		char key = cv::waitKey(1);
		if (key == 27) break; // Exit on ESC key
	}

	return 0;
}
