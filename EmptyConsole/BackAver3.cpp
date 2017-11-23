#include "BackAver3.h"

BackAver3::BackAver3()
{
}

BackAver3::~BackAver3()
{
}

//OpenCV������ۼӶ��ͼ��ȡƽ��ֵ
int BackAver3::funMain(string fileName)
{
	int nframe = 50;//����ǰnfram֡���ƽ��ͼ
	char* myChar = const_cast<char*>(fileName.c_str());
	CvCapture *capture = cvCreateFileCapture(myChar);
	if (NULL == capture)
	{
		printf("û���ҵ�����Ƶ��\n");
		return -1;
	}
	IplImage * Img = cvQueryFrame(capture);
	cvNamedWindow("ԭͼ", 0);
	//cvShowImage("ԭͼ",Img);
	IplImage * img_sum = cvCreateImage(cvGetSize(Img), IPL_DEPTH_32F, 3);
	cvZero(img_sum);
	for (int i = 0; i <nframe; i++)//ͳ�Ʊ���ģ��
	{
		cvAcc(Img, img_sum);
		Img = cvQueryFrame(capture);
		cvWaitKey(100);
		cvShowImage("ԭͼ", Img);
		//printf(".");
	}
	IplImage * img_sum_gray = cvCreateImage(cvGetSize(Img), IPL_DEPTH_8U, 3);
	cvConvertScale(img_sum, img_sum_gray, 1.0 / nframe);

	cvNamedWindow("img_sum_gray", 0);
	cvShowImage("img_sum_gray", img_sum_gray);
	cvSaveImage("model.jpg", img_sum_gray);
	cvWaitKey(0);

	cvDestroyAllWindows();
	cvReleaseImage(&img_sum);
	cvReleaseImage(&img_sum_gray);

	return 1;
}
