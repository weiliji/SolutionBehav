#include "Link.h"

Link::Link()
{
}

Link::~Link()
{
}

int Link::funMain()
{
	IplImage *src = cvLoadImage("MySrc/lena.jpg", CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("ԭʼͼ��");
	cvShowImage("ԭʼͼ��", src);

	IplImage* dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
	cvCvtColor(src, dst, CV_BGR2GRAY);
	cvNamedWindow("�Ҷ�ͼ��");
	cvShowImage("�Ҷ�ͼ��", src);

	cvThreshold(dst, dst, 0.0, 255.0, CV_THRESH_BINARY | CV_THRESH_OTSU);//OTSU��ֵ��   
	IplConvKernel *element = cvCreateStructuringElementEx(5, 5, 0, 0, CV_SHAPE_ELLIPSE);
	cvMorphologyEx(dst, dst, NULL, element, CV_MOP_OPEN);//�����㣬ȥ���ȽṹԪ��С�ĵ�   

	cvReleaseStructuringElement(&element);
	cvNamedWindow("��ֵͼ��");
	cvShowImage("��ֵͼ��", dst);

	int w, h;
	CvSize sz = cvGetSize(dst);
	int color = 254;//����0������������Ϊ255������254   
	for(w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (color > 0)
			{
				if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == 255)
				{
					//����ͨ��������ɫ   
					cvFloodFill(dst, cvPoint(w, h), CV_RGB(color, color, color));
					color--;
				}
			}
		}
	}
	cvNamedWindow("�����ɫ���ͼ��");
	cvShowImage("�����ɫ���ͼ��", dst);

	int colorsum[255] = { 0 };
	for (w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (CV_IMAGE_ELEM(dst, unsigned char, h, w) > 0)
			{
				colorsum[CV_IMAGE_ELEM(dst, unsigned char, h, w)]++;//ͳ��ÿ����ɫ������   
			}
		}
	}

	vector<int> v1(colorsum, colorsum + 255);//�������ʼ��vector   
	//������������Ⱦɫ��ע��max_element��ʹ�÷���   
	int maxcolorsum = max_element(v1.begin(), v1.end()) - v1.begin();
	printf("%d\n", maxcolorsum);
	for (w = 0; w<sz.width; w++)
	{
		for (h = 0; h<sz.height; h++)
		{
			if (CV_IMAGE_ELEM(dst, unsigned char, h, w) == maxcolorsum)
			{
				CV_IMAGE_ELEM(dst, unsigned char, h, w) = 255;
			}
			else
			{
				CV_IMAGE_ELEM(dst, unsigned char, h, w) = 0;
			}
		}
	}
	cvNamedWindow("�����ͨ��ͼ");
	cvShowImage("�����ͨ��ͼ", dst);
	cvWaitKey(0);
	cvDestroyAllWindows();
	cvReleaseImage(&src);
	cvReleaseImage(&dst);
	return 0;
}