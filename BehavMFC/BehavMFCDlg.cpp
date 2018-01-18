#include "stdafx.h"
#include "BehavMFC.h"
#include "BehavMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//************************************************************************* about 
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	enum { IDD = IDD_ABOUTBOX };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    
protected:
	DECLARE_MESSAGE_MAP()
};
CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}
void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}
BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()
CBehavMFCDlg::CBehavMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBehavMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CBehavMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//****************************************************************************************************************map 
BEGIN_MESSAGE_MAP(CBehavMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RCamera, &CBehavMFCDlg::OnBnClickedRCamera)
	ON_BN_CLICKED(IDC_RLocal, &CBehavMFCDlg::OnBnClickedRLocal)
	ON_BN_CLICKED(IDC_BOpen, &CBehavMFCDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDC_BClose, &CBehavMFCDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BPause, &CBehavMFCDlg::OnBnClickedBpause)
	ON_BN_CLICKED(IDC_BContinue, &CBehavMFCDlg::OnBnClickedBcontinue)
	ON_BN_CLICKED(IDC_BSaveBegin, &CBehavMFCDlg::OnBnClickedBsave)
	ON_BN_CLICKED(IDC_BSaveDone, &CBehavMFCDlg::OnBnClickedBsavedone)
	ON_BN_CLICKED(IDC_BCatch, &CBehavMFCDlg::OnBnClickedCatch)
	ON_BN_CLICKED(IDC_BProcess, &CBehavMFCDlg::OnBnClickedProcess)

END_MESSAGE_MAP()

//******************************************************************************************************system hander
BOOL CBehavMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//default choice
	CButton* radio = (CButton*)GetDlgItem(IDC_RCamera);
	radio->SetCheck(1);
	choose = "Camera";
	
	return TRUE;  
}
void CBehavMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();  
		////����Դ�м���ͼƬ  
		//CBitmap bitmap;
		////����ָ��λͼ��Դ BmpͼƬID  
		//bitmap.LoadBitmap(IDB_BitBackgroud);
		////��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID  
		//CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
		////���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ  
		//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
		////��ͼƬ���õ�Picture�ؼ���  
		//p->SetBitmap(bitmap);
	}
}
void CBehavMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
HCURSOR CBehavMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//***************************************************************************************************DIY hander
void CBehavMFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//��ȡ��ʾ�ؼ��ľ��

	frame = cvQueryFrame(Capture); //������ͷ�����ļ���ץȡ������һ֡
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��   
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���
	CDialogEx::OnTimer(nIDEvent);
	//save video
	cvWriteFrame(writer, frame);
}

void CBehavMFCDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO:  �ڴ˴������Ϣ����������
	//release resource
	if (Capture)
		cvReleaseCapture(&Capture);
	if (writer)
		cvReleaseVideoWriter(&writer);
	KillTimer(1);
}

//**********************************************************************************************************buttons
void CBehavMFCDlg::OnBnClickedRCamera()
{
	if (Capture)
	{
		MessageBox(_T("Video is open��\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
		if (choose == "Local")
		{
			CButton* radioR = (CButton*)GetDlgItem(IDC_RLocal);
			radioR->SetCheck(1);
			CButton* radioC = (CButton*)GetDlgItem(IDC_RCamera);
			radioC->SetCheck(0);
		}
		return;
	}
	choose = "Camera";
}

void CBehavMFCDlg::OnBnClickedRLocal()
{
	if (Capture)
	{
		MessageBox(_T("Video is open��\n-->Close"), _T("Waring"), MB_ICONEXCLAMATION);
		if (choose == "Camera")
		{
			CButton* radioR = (CButton*)GetDlgItem(IDC_RLocal);
			radioR->SetCheck(0);
			CButton* radioC = (CButton*)GetDlgItem(IDC_RCamera);
			radioC->SetCheck(1);
		}
		return;
	}
	choose = "Local";
}

void CBehavMFCDlg::OnBnClickedOpen()
{
	//release $Capture
	if (Capture)
		cvReleaseCapture(&Capture);
	//judge $choose
	if (choose == "Camera")
	{
		Capture = cvCreateCameraCapture(0);
		if (Capture == 0)
		{
			MessageBox(_T("Can't open camera!"), _T("Waring"), MB_ICONEXCLAMATION);
			return;
		}
	}
	else if (choose == "Local")
	{
		//open file box
		CFileDialog dlg(
			TRUE, _T("*.avi"), NULL,
			OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
			_T("Video files (*.avi) |*.avi; | All files (*.*) |*.*||"), NULL
			);
		dlg.m_ofn.lpstrTitle = _T("Open video");    // ���ļ��Ի���ı�����
		if (dlg.DoModal() != IDOK)                    // �ж��Ƿ���ͼƬ
			return;
		
		CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��
		CT2A stp(mPath);		
		Capture = cvCreateFileCapture(stp);
		if (NULL == Capture)
		{
			MessageBox(_T("No video file��"), _T("Waring"), MB_ICONEXCLAMATION);
			return ;
		}
	}
	else
	{
		MessageBox(_T("Wrong choose!"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	//get frame
	frame = cvQueryFrame(Capture); 
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); 
	//show frame
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��     
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���  
	ReleaseDC(pDC);

	SetTimer(1, FPS, NULL); //��ʱ��
}

void CBehavMFCDlg::OnBnClickedClose()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��\n-->Open"),_T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	cvReleaseCapture(&Capture);
	KillTimer(1);

	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc();//��ȡ��ʾ�ؼ��ľ��
	frame = cvLoadImage("MySrc/lena.jpg"); 
	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��   
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���
	ReleaseDC(pDC);
}

void CBehavMFCDlg::OnBnClickedBpause()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	KillTimer(1);
}

void CBehavMFCDlg::OnBnClickedBcontinue()
{
	if (!Capture)
	{
		MessageBox(_T("No open video��"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	SetTimer(1, 25, NULL);
}

void CBehavMFCDlg::OnBnClickedBsave()
{
	//CvSize size = cvSize(
	//	(int)cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_WIDTH),
	//	(int)cvGetCaptureProperty(cap, CV_CAP_PROP_FRAME_HEIGHT)
	//	);
	//double fps = 25;
	//CvSize size = cvSize(640, 480);
	//writer = cvCreateVideoWriter("MyOutput/SavedVideos/MyVideo.avi", CV_FOURCC('P', 'I', 'M', '1'), fps, size);
	//writer = cvCreateVideoWriter("MyOutput/MyVideo.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, size);
	//release $writer
	if (!Capture)
	{
		MessageBox(_T("No open video��\n-->Open"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (choose == "Local")
	{
		MessageBox(_T("It is local video��\n-->Camera"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	if (writer)
		cvReleaseVideoWriter(&writer);
	char VideosName[100];
	ImgNum = ImgNum + 1;
	sprintf_s(VideosName, "%s%.2d%s", "MyOutput/SavedVideos/", ImgNum, ".avi");
	writer = cvCreateVideoWriter(VideosName, CV_FOURCC('X', 'V', 'I', 'D'), FPS, cvSize(VIDEO_WIDTH, VIDEO_HEIGHT));
}

void CBehavMFCDlg::OnBnClickedBsavedone()
{
	if (!writer)
	{
		MessageBox(_T("Not saving��\n-->SaveBegin"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	cvReleaseVideoWriter(&writer);
	//char VideosName[100];
	//ImgNum = ImgNum + 1;
	//sprintf_s(VideosName, "%s%.2d%s", "MyOutput/CatchedPictures/", ImgNum, ".avi");
	//IplImage* m_snap = cvCreateImage(cvGetSize(m_grabframe), m_grabframe->depth, m_grabframe->nChannels);
	//cvCopy(m_grabframe, m_snap, NULL);
	//cvSaveImage(VideosName, m_snap);
}

void CBehavMFCDlg::OnBnClickedCatch()
{
	//m_grabframe = cvQueryFrame(Capture);
	m_grabframe = frame;
	if (m_grabframe == 0)
	{
		MessageBox(_T("No frame!"), _T("Waring"), MB_ICONEXCLAMATION);
		return;
	}
	//CString ImagePath = TEXT("MyOutput/CatchedPictures/");
	////CString ImagePath = _T("D:\\Documents\\Visual Studio 2013\\Projects\\�궨ͼƬ\\");
	//if (!PathIsDirectory(ImagePath))
	//{
	//	CreateDirectory(ImagePath, 0);//�������򴴽�
	//	MessageBox(_T("Folder created"));
	//	return;
	//}
	char ImagesName[100];
	ImgNum = ImgNum + 1;
	sprintf_s(ImagesName, "%s%.2d%s", "MyOutput/CatchedPictures/", ImgNum, ".jpg");
	IplImage* m_snap = cvCreateImage(cvGetSize(m_grabframe), m_grabframe->depth, m_grabframe->nChannels);
	cvCopy(m_grabframe, m_snap, NULL);
	cvSaveImage(ImagesName, m_snap);
	////show Catshedpic in the rect
	//pDC = GetDlgItem(IDC_Picture)->GetDC();
	//GetDlgItem(IDC_Picture)->GetClientRect(&rect);
	//hDC = pDC->GetSafeHdc();
	//CvvImage m_CvvImage;
	//m_CvvImage.CopyOf(m_snap, 1); 
	//m_CvvImage.DrawToHDC(hDC, &rect); 
}

void CBehavMFCDlg::OnBnClickedProcess()
{
	//��ȡ�Ի����ϵľ�� ͼƬ�ؼ�ID  
	CStatic *p = (CStatic *)GetDlgItem(IDC_Back);
	//���þ�̬�ؼ����ڷ��Ϊλͼ������ʾ  
	p->MoveWindow(500,500,100,100);
	//p->ModifyStyle(0xf, SS_BITMAP | SS_CENTERIMAGE);
	////��ͼƬ���õ�Picture�ؼ���  
	//p->SetBitmap(bitmap);
	//int i = 10;
	//CString str;
	//str.Format(_T("this%s"),choose);
	//MessageBox(str);
	//MessageBox(choose);
	//MessageBox(_T("No frame !"));
	//IplImage *gray = 0, *edge = 0;
	//gray = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//edge = cvCreateImage(cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1);
	//cvCvtColor(TheImage, gray, CV_BGR2GRAY);
	//cvCanny(gray, edge, 30, 100, 3);
	//cvCvtColor(edge, TheImage, CV_GRAY2BGR);
	//ShowImage(TheImage, IDC_ShowImg);            // ������ʾͼƬ����

	//cvReleaseImage(&gray);
	//cvReleaseImage(&edge);
}

//**************************************************************************************************assistant moduels
void CBehavMFCDlg::ShowImage(IplImage* img, UINT ID)
{
	CDC* pDC = GetDlgItem(ID)->GetDC();        // �����ʾ�ؼ��� DC
	HDC hDC = pDC->GetSafeHdc();                // ��ȡ HDC(�豸���) �����л�ͼ����

	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	int rw = rect.right - rect.left;            // ���ͼƬ�ؼ��Ŀ�͸�
	int rh = rect.bottom - rect.top;
	int iw = img->width;                        // ��ȡͼƬ�Ŀ�͸�
	int ih = img->height;
	int tx = (int)(rw - iw) / 2;                    // ʹͼƬ����ʾλ�������ڿؼ�������
	int ty = (int)(rh - ih) / 2;
	SetRect(rect, tx, ty, tx + iw, ty + ih);

	CvvImage cimg;
	cimg.CopyOf(img);                            // ����ͼƬ
	cimg.DrawToHDC(hDC, &rect);                // ��ͼƬ���Ƶ���ʾ�ؼ���ָ��������

	ReleaseDC(pDC);
}
void CBehavMFCDlg::ResizeImage(IplImage* img)
{
	// ��ȡͼƬ�Ŀ�͸�
	int w = img->width;
	int h = img->height;

	// �ҳ���͸��еĽϴ�ֵ��
	int max = (w > h) ? w : h;

	// ���㽫ͼƬ���ŵ�TheImage��������ı�������
	float scale = (float)((float)max / 256.0f);

	// ���ź�ͼƬ�Ŀ�͸�
	int nw = (int)(w / scale);
	int nh = (int)(h / scale);

	// Ϊ�˽����ź��ͼƬ���� TheImage �����в�λ�������ͼƬ�� TheImage ���Ͻǵ���������ֵ
	int tlx = (nw > nh) ? 0 : (int)(256 - nw) / 2;
	int tly = (nw > nh) ? (int)(256 - nh) / 2 : 0;

	// ���� TheImage �� ROI ������������ͼƬ img
	//cvSetImageROI(TheImage, cvRect(tlx, tly, nw, nh));

	// ��ͼƬ img �������ţ������뵽 TheImage ��
	//cvResize(img, TheImage);

	// ���� TheImage �� ROI ׼��������һ��ͼƬ
	//cvResetImageROI(TheImage);
}

//*************************open a local picture part
//void CBehavMFCDlg::OnBnClickedOpen()
//{
//	//CFileDialog dlg(
//	//	TRUE, _T("*.jpg"), NULL,
//	//	OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
//	//	_T("image files (*.bmp; *.jpg) |*.bmp; *.jpg | All Files (*.*) |*.*||"), NULL
//	//	);   
//	CFileDialog dlg(
//		TRUE, _T("*.jpg"), NULL,
//		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
//		_T("Image Files (*.jpg) |*.jpg; | All Files (*.*) |*.*||"), NULL
//		);
//
//	dlg.m_ofn.lpstrTitle = _T("Open Image");    // ���ļ��Ի���ı�����
//	if (dlg.DoModal() != IDOK)                    // �ж��Ƿ���ͼƬ
//		return;
//
//	CString mPath = dlg.GetPathName();            // ��ȡͼƬ·��
//	CT2A stp(mPath);
//	IplImage* ipl = cvLoadImage(stp, 1);    // ��ȡͼƬ�����浽һ���ֲ����� ipl ��
//	if (!ipl)                                    // �ж��Ƿ�ɹ�����ͼƬ
//		return;
//	if (TheImage)                                // ����һ����ʾ��ͼƬ��������
//		cvZero(TheImage);
//
//	ResizeImage(ipl);    // �Զ����ͼƬ�������ţ�ʹ��������ֵ�߸պõ��� 256���ٸ��Ƶ� TheImage ��
//	ShowImage(TheImage, IDC_ShowImg);            // ������ʾͼƬ����    
//	cvReleaseImage(&ipl);                        // �ͷ� ipl ռ�õ��ڴ�
//}




