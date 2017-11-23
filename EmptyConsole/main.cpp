#include "opencv2/opencv.hpp"
#include <iostream>
#include <cstdio>

#include "DiffTwo.h"
#include "DiffTwo2.h"
#include "DiffThree.h"
#include "DiffThree2.h"
#include "BackAver.h"
#include "BackAver2.h"
#include "BackAver3.h"
#include "BackAver4.h"
#include "BackAver5.h"

#include "ViBe_BGS.h"

int playFile();

int main()
{
	//playFile();

	//DiffTwo d1;
	//d1.funMain("MySrc/lobby.avi");

	//DiffTwo2 d22;
	//d22.script("MySrc/sample.avi");

	//DiffThree d3;
	//d3.script("MySrc/lobby.avi");

	//DiffThree2 d32;
	//d32.script("MySrc/sample.avi");

	//BackAver ba1;
	//ba1.funMain();

	//BackAver2 ba2;

	//BackAver3 ba3;
	//ba3.funMain("MySrc/curtain.avi");

	//BackAver4 ba4;
	//ba4.funMain("MySrc/lobby.avi");

	BackAver5 ba5;
	ba5.funMain("MySrc/sample.avi");

	//ViBe_BGS v1;
	//v1.script("MySrc/lobby.avi");
	return 0;
}

int playFile()
{
	VideoCapture capture;
	Mat frame;
	capture.open("MySrc/goout.flv");
	//capture.open(0);
	if (!capture.isOpened())
	{
		cout << "No camera or video input!\n" << endl;
		return -1;
	}
	while (true)
	{
		capture.read(frame);
		if (frame.empty())
			continue;
		imshow("File", frame);
		waitKey(30);
	}
	return 1;
}