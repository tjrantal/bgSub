//OpenCV
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
//C
#include <stdio.h>
//C++
#include <iostream>
#include	<sstream>

//LBP
#include "lbp/LBP.h"

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){
	if (argc < 3){
		printf("bgSub videoFile historyLength [=100] learningRate [=0.1]\n");
		exit(0);
	}
	Mat frame;
	Mat scaled;
	Mat gScale;
	Mat *LBPFig;

	//double** lbpFigData;
	Mat foregroundFrame;
	Ptr<BackgroundSubtractor> bgMod;	//MOG2 bg subtractor
	namedWindow("Frame", WINDOW_AUTOSIZE);	//Display the vide here
	namedWindow("Foreground", WINDOW_AUTOSIZE);	//Display the vide here
	namedWindow("Canny", WINDOW_AUTOSIZE);	//Display the vide here
	namedWindow("LBP", WINDOW_AUTOSIZE);	//Display the vide here

	moveWindow("Frame", 10, 10);
	moveWindow("Foreground", 800, 10);
	moveWindow("Canny", 10, 500);
	moveWindow("LBP", 800, 500);
	printf("%s\n",argv[1]);
	bgMod = createBackgroundSubtractorMOG2(atoi(argv[2]),(double) 400.0,false);
	VideoCapture vc(argv[1]);	//First argument needs to be the vid name
	if (!vc.isOpened()){
		printf("Could not open vid\n");
	}else{
		int cnt = 0;

		
		//Get matrix size
		LBP lbp(16, 2, 3);
		uchar* lbpData;
		while(vc.read(frame)){
			resize(frame, scaled, Size(), 0.4, 0.4, INTER_CUBIC);	//Scale size down
			//Get image size
			/*
			if (cnt == 0) {
				lbpFigData = new double*[scaled.cols];
				for (int i = 0; i < scaled.cols; ++i) {
					lbpFigData[i] = new double[scaled.rows];
				}
			}
			//Copy data to LBP double
			

			for (int i = 0; i < scaled.cols; ++i) {
				for (int j = 0; j < scaled.rows; ++j) {
					lbpFigData[i][j] = (double)scaled.at<uchar>(j, i);	//Note that lbp has column, row (=x,y) instead of row, column
				}
			}
			*/
			
			cvtColor(scaled, gScale, CV_BGR2GRAY);
			//Filter gScale
			GaussianBlur(gScale, gScale, Size(7, 7), 1.5, 1.5);
			lbpData = lbp.getLBP(gScale.ptr<uchar>(), scaled.cols, scaled.rows);
			//Take canny edges
			Canny(gScale, gScale, 0, 30, 3);
						
			//imshow("Frame",frame);
			imshow("Frame", scaled);
			//bgMod->apply(frame,foregroundFrame,atof(argv[3]));	//Update bg model and extract foreground
			bgMod->apply(scaled, foregroundFrame, atof(argv[3]));	//Update bg model and extract foreground
			imshow("Foreground",foregroundFrame);
			LBPFig = new Mat(scaled.rows, scaled.cols, CV_8UC1, lbpData);
			
			imshow("Canny", gScale);
			imshow("LBP", *LBPFig);
			//Calc LBP image
			/*
			if (cnt == 0) {
				moveWindow("Frame", 10, 10);
				moveWindow("Foreground", 800, 10);
			}
			*/
			waitKey(10);
			//printf("%d\n",++cnt);
			++cnt;
		}
		vc.release();	//Delete capture object
		//imshow("Frame",frame);
	}
	//waitKey(0); 
	
}
