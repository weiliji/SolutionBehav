#pragma once
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

#define threshold_diff1 10 //���ü�֡���ֵ
#define threshold_diff2 10 //���ü�֡���ֵ

class DiffThree
{
public:
	DiffThree();
	~DiffThree();
	int script(string fileName);
private:

};

