#include "BackAver.h"

BackAver::BackAver()
{
}

BackAver::~BackAver()
{
}

int BackAver::funMain(string fileName)
{
	CvCapture* capture;
	if (fileName == "00")
	{
		capture = cvCreateCameraCapture(0);	//��ʼ����������л�ȡ��Ƶ
		//@J,judge if thera is a camera
		if (!capture)
		{
			printf("Couldn't Open the file.");
			return -1;
		}
	}
	else
	{
		char* myChar = const_cast<char*>(fileName.c_str());
		capture = cvCreateFileCapture(myChar);
		if (NULL == capture)
		{
			printf("û���ҵ�����Ƶ��\n");
			return -1;
		}

	}

	cvNamedWindow("raw");
	cvNamedWindow("result");
	IplImage* rawImage = cvQueryFrame(capture);		//������������Ǻ���cvGrabFrame�ͺ���cvRetrieveFrame��һ����õ����
	cvShowImage("raw", rawImage);					//��ʾ��һ֡ͼ��
	//@J,wait for camera to init
	while (rawImage == NULL)
	{
		cvWaitKey(1000);
		rawImage = cvQueryFrame(capture);
	}
	AllocateImages(rawImage);						//�Ե�һ֡ͼ�����ͼ���С		

	for (int i = 0;; i++)
	{
		if (i <= 30)
		{
			cout << i<<'\n';
			accumulateBackground(rawImage);		//ǰ30֡�����ۻ����㱳��ͼ��	
			if (i == 30)								//��ǰ30֡ת����һ������ͳ��ģ��	
				createModelsfromStats();
		}
		else
			backgroundDiff(rawImage);				//�����ñ���ģ�ͺ���ô˺�������ͼ��ָ�
		
		cvShowImage("result", Imask);					//���ŷָ���Ŀ��ͼ����

		if (cvWaitKey(30) == 'q')						//ÿ33ms ����һ֡		
			break;
		if (!(rawImage = cvQueryFrame(capture)))
			break;
		cvShowImage("raw", rawImage);				//��ʾԴͼ��
	}
	DeallocateImages();								//�����ڴ�

	return 1;
}

/*�ú���Ϊ��Ҫ��������ʱͼ������ڴ棬����������Ƶ����֡ͼ����Ϊ��С�ο�*/
void BackAver::AllocateImages(IplImage* I)
{
	CvSize sz = cvGetSize(I);
	/*Float 3-channel images*/
	IavgF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IdiffF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IprevF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IhiF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	IlowF = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	cvZero(IavgF);
	cvZero(IdiffF);
	cvZero(IprevF);
	cvZero(IhiF);
	cvZero(IlowF);

	/*Float 1-channel images*/
	Ilow1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ilow3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Ihi3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);

	Icount = 1e-5;

	/*Float 3-channel images*/
	Iscratch = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	Iscratch2 = cvCreateImage(sz, IPL_DEPTH_32F, 3);
	cvZero(Iscratch);
	cvZero(Iscratch2);

	/*Float 1-channel images*/
	Igray1 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray2 = cvCreateImage(sz, IPL_DEPTH_32F, 1);
	Igray3 = cvCreateImage(sz, IPL_DEPTH_32F, 1);

	/*Byte 1-channel image*/
	Imaskt = cvCreateImage(sz, IPL_DEPTH_8U, 1);
	Imask = cvCreateImage(sz, IPL_DEPTH_8U, 1);
}

//@J,calcu both addsum and diffs,to $IavgF and $IdiffF
/*�ۻ�����ͼ���ǰ��֡ͼ���ֵ�ľ���ֵ�����ۻ���һ��������ͽ���ת����һ������ͳ��ģ��*/
void BackAver::accumulateBackground(IplImage* I)
{
	static int first = 1;							//�ֲ���̬����,ֻ��ʼ��һ��,��˼���ǵ�һ�α���ֵΪ1
	cvCvtScale(I, Iscratch, 1, 0);				//��cvConvertScale��dst(I)=src(I)*scale+(shift,shift,...)���˴�Iscratch(I)=I(I)*1+0
	//��Iָ���ͼ���Ƹ�Iscratch ������cvCopy,��Ϊ���ص�λ��Ȳ�ͬ
	if (!first)										//�տ�ʼ��first=1��������
	{
		cvAcc(Iscratch, IavgF);					//�ۻ�ԭʼ�ĸ���ͼ��IIavgF
		cvAbsDiff(Iscratch, IprevF, Iscratch2);	//����ǰ��֡ͼ����Բ�ͼ��Iscratch2
		cvAcc(Iscratch2, IdiffF);					//��ǰ��֡��ֵͼ���ۼӵ�IdiffF��	
		Icount += 1.0;								//��¼�ۼӵĴ������ڱ���ͳ��ʱ�����ֵ		
	}
	first = 0;										//firstΪ�ֲ���̬����,���Ժ���øú��������ٳ�ʼ��Ϊ1����˼���ǳ��˵�һ�Σ��Ժ���øú���������if���
	cvCopy(Iscratch, IprevF);						//IprevF��������ǰһ֡ͼ��
}

//@J,IhiF = IavgF + IdiffF * scale
/*���ø���ֵ*/
void BackAver::setHighThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);		//��ͳ�Ƶľ��Բ��ͼ��ֵ�Ŵ�scale������Iscratch��Iscratch = IdiffF*scale
	cvAdd(Iscratch, IavgF, IhiF);					//IhiF = Iscratch + IavgF
	cvSplit(IhiF, Ihi1, Ihi2, Ihi3, 0);			//����ֵ���޷ָ�Ϊ��ͨ��	
}

//@J,IlowF = IavgF - IdiffF * scale
/*���õ���ֵ*/
void BackAver::setLowThreshold(float scale)
{
	cvConvertScale(IdiffF, Iscratch, scale);		//Iscratch = IdiffF*scale
	cvSub(IavgF, Iscratch, IlowF);				//IlowF = IavgF - Iscratch
	cvSplit(IlowF, Ilow1, Ilow2, Ilow3, 0);		//����ֵ���޷ָ�Ϊ��ͨ��
}

/*���ۻ��㹻���֡ͼ��,�ͽ���ת����һ������ͳ��ģ�ͣ��ú������ڼ���ÿ�����صľ�ֵ��ƽ�����Բ��*/
void BackAver::createModelsfromStats()
{
	cvConvertScale(IavgF, IavgF, (double)(1.0 / Icount));	//����ƽ��ԭʼͼ�� IavgF��IavgF = IavgF/Icount
	cvConvertScale(IdiffF, IdiffF, (double)(1.0 / Icount));	//������Բ��ͼ�� IdiffF��IdiffF = IdiffF/Icount
	cvAddS(IdiffF, cvScalar(1.0, 1.0, 1.0), IdiffF);		//ʹ�õ��ľ��Բ��ͼ��ÿ������ֵ����Ϊ�գ���ÿ�����ؼ���1
	setHighThreshold(7.0);									//����ͳ�Ƶı���ģ���趨һ����ֵ���޺�����	
	setLowThreshold(6.0);									//��� IlowF<=Temp<IhiF ʱ��Ϊ��Ϊ����,����Ϊ��Ƶ�г��ֵ��˶�Ŀ������	
	cout << "background done";
}

void BackAver::backgroundDiff(IplImage* I)
{
	cvCvtScale(I, Iscratch, 1, 0);				//��Iָ���ͼ���Ƹ�Iscratch ������cvCopy, ��Ϊ���ص�λ��Ȳ�ͬ
	cvSplit(Iscratch, Igray1, Igray2, Igray3, 0);	//�õ��ĵ�ǰ֡�ָ��3����ͨ��ͼ��	
	cvInRange(Igray1, Ilow1, Ihi1, Imask);		//�����Щ��ͨ��ͼ���Ƿ���ƽ���������ظߵͷ�ֵ֮��
	//         src     lower  upper dst				  ���src(I)�ڷ�Χ��(lower<=src<upper)��dst(I)������Ϊ0xff(ÿһλ����'1')������0
	cvInRange(Igray2, Ilow2, Ihi2, Imaskt);
	cvOr(Imask, Imaskt, Imask);					//������������ÿ��Ԫ�صİ�λ��ֵ��ֵ������������
	cvInRange(Igray3, Ilow3, Ihi3, Imaskt);
	cvOr(Imask, Imaskt, Imask);					//���ImaskΪ�������ǰ����ֵͼ
	cvSubRS(Imask, cvScalar(255), Imask);			//��������������֮��Ĳ�,��Imask���ദ��
}

/*���������ڴ�*/
void BackAver::DeallocateImages()
{
	cvReleaseImage(&IavgF);
	cvReleaseImage(&IdiffF);
	cvReleaseImage(&IprevF);
	cvReleaseImage(&IhiF);
	cvReleaseImage(&IlowF);
	cvReleaseImage(&Ilow1);
	cvReleaseImage(&Ilow2);
	cvReleaseImage(&Ilow3);
	cvReleaseImage(&Ihi1);
	cvReleaseImage(&Ihi2);
	cvReleaseImage(&Ihi3);
	cvReleaseImage(&Iscratch);
	cvReleaseImage(&Iscratch2);
	cvReleaseImage(&Igray1);
	cvReleaseImage(&Igray2);
	cvReleaseImage(&Igray3);
	cvReleaseImage(&Imaskt);
	cvReleaseImage(&Imask);
}

