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

using namespace cv;
using namespace std;

int main(int argc, char* argv[]){
	if (argc < 3){
		printf("bgSub videoFile historyLength [=100] learningRate [=0.1]\n");
		exit(0);
	}
	Mat frame;
	Mat foregroundFrame;
	Ptr<BackgroundSubtractor> bgMod;	//MOG2 bg subtractor
	namedWindow("Frame", WINDOW_AUTOSIZE);	//Display the vide here
	namedWindow("Foreground", WINDOW_AUTOSIZE);	//Display the vide here
	printf("%s\n",argv[1]);
	bgMod = createBackgroundSubtractorMOG2(atoi(argv[2]),(double) 400.0,false);
	VideoCapture vc(argv[1]);	//First argument needs to be the vid name
	if (!vc.isOpened()){
		printf("Could not open vid\n");
	}else{
		int cnt = 0;
		while(vc.read(frame)){
			imshow("Frame",frame);
			bgMod->apply(frame,foregroundFrame,atof(argv[3]));	//Update bg model and extract foreground
			imshow("Foreground",foregroundFrame);
			waitKey(10);
			//printf("%d\n",++cnt);
		}
		vc.release();	//Delete capture object
		//imshow("Frame",frame);
	}
	waitKey(0); 
	
}
