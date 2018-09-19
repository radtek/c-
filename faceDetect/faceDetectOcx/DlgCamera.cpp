// DlgCamera.cpp : 实现文件
//

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "DlgCamera.h"
#include "afxdialogex.h"

CDlgCamera g_dlgCamera;
// CDlgCamera 对话框

IMPLEMENT_DYNAMIC(CDlgCamera, CDialogEx)

CDlgCamera::CDlgCamera(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgCamera::IDD, pParent)
{

}

CDlgCamera::~CDlgCamera()
{
}

void CDlgCamera::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CAMERA, m_staticPic);
}


BEGIN_MESSAGE_MAP(CDlgCamera, CDialogEx)
END_MESSAGE_MAP()


// CDlgCamera 消息处理程序


void CDlgCamera::showCameraPic(cv::Mat &img, Size &size)
{

	
	Mat showImg;
	resize(img, showImg, size);
	if (showImg.empty())
	{
		TrcWritef( EC_INFO,"showImg.empty()");
		return;
	}

	imshow("DISPLAY", showImg);
	
}



BOOL CDlgCamera::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化


	namedWindow("DISPLAY", WINDOW_AUTOSIZE);
	HWND hWnd = (HWND)cvGetWindowHandle("DISPLAY");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(IDC_STATIC_CAMERA)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	/*
	CRect pic_rect;
	int width;
	int	height;
	GetDlgItem(IDC_STATIC_CAMERA)->GetClientRect(&pic_rect);
	width = pic_rect.right;
	height = pic_rect.bottom;
	g_theApp.setCameraSize(width,height);
	*/

	//CLogger::Instance()->TraceInfo("m_ocxCameraWidth = %d, m_ocxCameraHeight = %d",m_ocxCameraWidth,m_ocxCameraHeight);
	//cv::Mat mat = imread("d:\\1.jpg");
	//imshow("",mat);
	//showCameraPic(mat);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

