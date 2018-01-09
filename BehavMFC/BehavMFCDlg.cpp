#include "stdafx.h"
#include "BehavMFC.h"
#include "BehavMFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//********************************************** about 
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
	, TheImage(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}
void CBehavMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//**********************************map 
BEGIN_MESSAGE_MAP(CBehavMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BOpen, &CBehavMFCDlg::OnBnClickedOpen)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//*******************************************system hander
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
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS);

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
		CDialog::OnPaint();                    // �ػ�Ի���
		CDialog::UpdateWindow();                // ����windows���ڣ�������ⲽ���ã�ͼƬ��ʾ�����������
		ShowImage(TheImage, IDC_ShowImg);
	}
}

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

//**************************************button functions
void CBehavMFCDlg::OnBnClickedOpen()
{
	Capture = cvCreateCameraCapture(0);  
	if (Capture == 0)
	{
		MessageBox(_T("�޷���������ͷ������"));
		return;
	}

	frame = cvQueryFrame(Capture); 
	pDC = GetDlgItem(IDC_Camera)->GetDC();
	GetDlgItem(IDC_Camera)->GetClientRect(&rect);
	hDC = pDC->GetSafeHdc(); 

	CvvImage m_CvvImage;
	m_CvvImage.CopyOf(frame, 1); //���Ƹ�֡ͼ��     
	m_CvvImage.DrawToHDC(hDC, &rect); //��ʾ���豸�ľ��ο���  
	ReleaseDC(pDC);

	SetTimer(1, 25, NULL); //��ʱ������ʱʱ���֡��һ��
}

//*************************************************assistant moduels
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
	cvSetImageROI(TheImage, cvRect(tlx, tly, nw, nh));

	// ��ͼƬ img �������ţ������뵽 TheImage ��
	cvResize(img, TheImage);

	// ���� TheImage �� ROI ׼��������һ��ͼƬ
	cvResetImageROI(TheImage);
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

