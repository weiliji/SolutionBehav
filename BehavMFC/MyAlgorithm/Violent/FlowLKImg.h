//c-type;
//optical flow,LK,2 images
//http://blog.csdn.net/gdut2015go/article/details/46043821
#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
#include "../../GlobalSetting.h"
using namespace std;
using namespace cv;

const int MAX_CORNERS = 500;

class FlowLKImg
{
public:
	FlowLKImg();
	~FlowLKImg();
	int funMain(Mat mat1,Mat mat2);
	//outputs
	float out_lkValue;
};

