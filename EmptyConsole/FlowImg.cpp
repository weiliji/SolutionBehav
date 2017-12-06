#include "FlowImg.h"

FlowImg::FlowImg()
{
}

FlowImg::~FlowImg()
{
}

int FlowImg::funOne()
{
	Mat I1;
	Mat I2;
	vector<Point2f> features;
	int maxCout = 50;//����������
	double minDis = 20;//������С����
	double qLevel = 0.01;//��������ˮƽ

	I1 = imread("MySrc/lena.jpg",0);//��ȡΪ�Ҷ�ͼ��
	goodFeaturesToTrack(I1, features, maxCout, qLevel, minDis);
	for (int i = 0; i<features.size(); i++)
	{
		//�������㻭һ��СԲ����--��ϸΪ2
		circle(I1, features[i], 3, Scalar(255), 2);
	}
	imshow("features", I1);
	waitKey(0);
	return 0;
}

int FlowImg::funTwo()
{
	Mat I1;
	Mat I2;
	vector<Point2f> features;
	vector<Point2f> features_after;
	vector<uchar> status;
	vector<float> err;
	int maxCout = 300;//����������
	double minDis = 10;//������С����
	double qLevel = 0.01;//��������ˮƽ
	//��ȡ����ͼ��---����֡
	I1 = imread("I1.jpg", 0);//��ȡΪ�Ҷ�ͼ��
	I2 = imread("I2.jpg", 0);
	//����һ֡��������
	goodFeaturesToTrack(I1, features, maxCout, qLevel, minDis);
	//������ڶ�֡��������
	calcOpticalFlowPyrLK(I1, I2, features, features_after, status, err);
	//�б���Щ�����˶���������
	int k = 0;
	for (int i = 0; i<features_after.size(); i++)
	{
		//״̬Ҫ��1����������Ҫ�ƶ��µ���Щ��
		if (status[i] && ((abs(features[i].x - features_after[i].x) +
			abs(features[i].y - features_after[i].y))>4))
		{
			features_after[k++] = features_after[i];
		}
	}
	features_after.resize(k);//��ȡ
	cout << k << endl;
	for (int i = 0; i<features_after.size(); i++)
	{
		//�������㻭һ��СԲ����--��ϸΪ2
		circle(I1, features_after[i], 3, Scalar(255), 2);
	}
	imshow("features", I1);
	waitKey(0);
	return 0;
}