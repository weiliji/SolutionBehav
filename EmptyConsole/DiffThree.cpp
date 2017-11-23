#include "DiffThree.h"

DiffThree::DiffThree()
{
}

DiffThree::~DiffThree()
{
}

int DiffThree::script(string fileName)
{
	Mat img_src1, img_src2, img_src3;//3֡����Ҫ3֡ͼƬ
	Mat img_dst, gray1, gray2, gray3;
	Mat gray_diff1, gray_diff2;//�洢2�������ͼƬ
	Mat gray;//������ʾǰ����
	bool pause = false;
	VideoCapture vido_file(fileName);//���������Ӧ���ļ���
	namedWindow("foreground", 0);
	for (;;)
	{
		if (!false)
		{
			vido_file >> img_src1;
			if (img_src1.empty())
				break;
			if (img_src1.depth() != 1)
				cvtColor(img_src1, gray1, CV_BGR2GRAY);
			waitKey(5);

			vido_file >> img_src2;
			if (img_src2.empty())
				break;
			if (img_src2.depth() != 1)
				cvtColor(img_src2, gray2, CV_BGR2GRAY);
			imshow("video_src", img_src2);//
			waitKey(5);

			vido_file >> img_src3;
			if (img_src3.empty())
				break;
			if (img_src3.depth() != 1)
				cvtColor(img_src3, gray3, CV_BGR2GRAY);

			subtract(gray2, gray1, gray_diff1);//�ڶ�֡����һ֡
			subtract(gray3, gray2, gray_diff2);//����֡���ڶ�֡

			for (int i = 0; i<gray_diff1.rows; i++)
				for (int j = 0; j<gray_diff1.cols; j++)
				{
					if (abs(gray_diff1.at<unsigned char>(i, j)) >= threshold_diff1)//����ģ�����һ��Ҫ��unsigned char�������һֱ����
						gray_diff1.at<unsigned char>(i, j) = 255;            //��һ�������ֵ����
					else gray_diff1.at<unsigned char>(i, j) = 0;

					if (abs(gray_diff2.at<unsigned char>(i, j)) >= threshold_diff2)//�ڶ��������ֵ����
						gray_diff2.at<unsigned char>(i, j) = 255;
					else gray_diff2.at<unsigned char>(i, j) = 0;
				}
			bitwise_and(gray_diff1, gray_diff2, gray);
			imshow("foreground", gray);
		}
		char c = (char)waitKey(10);
		if (c == 27)
		{
			break;
		}
		if (c == ' ')
			pause = !pause;//Ϊʲô��ͣ���ˣ���
	}
	return 1;
}

