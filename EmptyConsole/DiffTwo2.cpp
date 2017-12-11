#include "DiffTwo2.h"

DiffTwo2::DiffTwo2()
{
}

DiffTwo2::~DiffTwo2()
{
}

int DiffTwo2::script(string fileName)
{
	VideoCapture videoCap(fileName);
	if (!videoCap.isOpened())
	{
		return -1;
	}
	double videoFPS = videoCap.get(CV_CAP_PROP_FPS);  //��ȡ֡��
	double videoPause = 1000 / videoFPS;

	Mat framePre; //��һ֡
	Mat frameNow; //��ǰ֡
	Mat frameDet; //�˶�����
	videoCap >> framePre;
	if (framePre.depth() != 1)
		cvtColor(framePre, framePre, CV_RGB2GRAY);
	while (true)
	{
		videoCap >> frameNow;
		if (frameNow.empty() || waitKey(30) == 'q')
		{
			break;
		}
		if (framePre.depth() != 1)
			cvtColor(frameNow, frameNow, CV_RGB2GRAY);
		absdiff(frameNow, framePre, frameDet);
		framePre = frameNow;
		imshow("Video", frameNow);
		imshow("Detection", frameDet);
	}
	return 1;
}
