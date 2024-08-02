
#include "opencv2/core.hpp"
#include "opencv2/core/hal/interface.h"
#include "opencv2/core/mat.hpp"
#include "opencv2/core/types.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <cstdio>
#include <ctime>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <ostream>
#include <thread>
#include <windows.h>
#include <wingdi.h>
#include <winuser.h>


#define COLOR_RED cv::Scalar(0,0,255)
#define COLOR_GREEN cv::Scalar(0,255,0)

//does a tamplate match then displays a rectangale over the object
void FindAndShowObject(cv::Mat image,cv::Mat object,cv::OutputArray result,double threshold,cv::Scalar color){// output array is already a refrence so no need for a pointer

	cv::matchTemplate(image,object,result,cv::TM_CCOEFF_NORMED);

	double minValue,maxValue;
	cv::Point minLoc,maxLoc;

	//this function returns the min/max value in the result matrix and the their position 
	cv::minMaxLoc(result,&minValue,&maxValue,&minLoc,&maxLoc);

	//check if the match is the best one

	if (threshold <= maxValue){
		//creating the rect
		cv::Rect boundingBox(maxLoc, cv::Point(maxLoc.x + object.cols, maxLoc.y + object.rows));
		//the first parameter is where we are displaying the rect,the second is the color,lastly the thickness 

		cv::rectangle(image, boundingBox, color, 2);
	}
}

//gets the content of the window
cv::Mat GetWindowContent(HWND hWND,int height,int width,BITMAPINFOHEADER bi){


	HDC hdcScreen = GetDC(hWND);
	HDC hdcMem = CreateCompatibleDC(hdcScreen);
	HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, width, height);
	SelectObject(hdcMem, hBitmap);

	BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);


	cv::Mat frame(height, width, CV_8UC4);
	GetDIBits(hdcScreen, hBitmap, 0, height, frame.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	DeleteDC(hdcMem);
	DeleteObject(hBitmap);
	return frame;
}

void SendKey(WORD key) {
	// Create a KEYBDINPUT structure
	INPUT input = {0};
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = key; // Virtual key code

	// Send key down event
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));

	// Send key up event
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

int main() {
	/*
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

	cv::Mat dino_game = cv::imread("E:/C_projects/bot/assets/dino game.png");
	//the image that we are looking for

	//cv::imshow("Match result ",dino_game);
	 */
	cv::Mat dino = cv::imread("E:/C_projects/bot/assets/dino.png",cv::IMREAD_UNCHANGED);
	cv::Mat small_cactus = cv::imread("E:/C_projects/bot/assets/1small cactus.png",cv::IMREAD_UNCHANGED);
	cv::namedWindow("Dino bot");

	HWND hWND= FindWindowA(NULL,"brave://dino/ - Brave");	

	if (!hWND){
		printf("\n\n error reading the window \n\n");
		return -1;
	}
	RECT windowRect;
	GetClientRect(hWND, &windowRect);


	int width = windowRect.right ;
	int height= windowRect.bottom;

	//info about the bitmap

	BITMAPINFOHEADER bi;
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = width;
	bi.biHeight = -height;  // Use negative height to ensure top-down bitmap
	bi.biPlanes = 1;
	bi.biBitCount = 32;  // Adjust bit count based on screen capture format
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	std::thread findDino([]{
			while (true) {

			}
			});

	double threshold = 0.41;

	cv::Mat result_dino ;
	cv::Mat result_cactus;

	while (true) {

		cv::Mat frame = GetWindowContent(hWND,height, width,bi);

		FindAndShowObject(frame, dino,result_dino,0,COLOR_GREEN);
		FindAndShowObject(frame,small_cactus,result_cactus,0.42,COLOR_RED);

		cv::imshow("Dino bot", frame);
		char key = cv::waitKey(1);

		if (key == 27){
			break;
		}



	}

	return 0;
}
