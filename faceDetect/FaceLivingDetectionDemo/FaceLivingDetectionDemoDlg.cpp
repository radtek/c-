
// FaceLivingDetectionDemoDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "FaceLivingDetectionDemo.h"
#include "FaceLivingDetectionDemoDlg.h"
#include "afxdialogex.h"
#include "FaceLivingDetectionDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFaceLivingDetectionDemoDlg 对话框




CFaceLivingDetectionDemoDlg::CFaceLivingDetectionDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFaceLivingDetectionDemoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFaceLivingDetectionDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFaceLivingDetectionDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFaceLivingDetectionDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFaceLivingDetectionDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CFaceLivingDetectionDemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CFaceLivingDetectionDemoDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CFaceLivingDetectionDemoDlg 消息处理程序

BOOL CFaceLivingDetectionDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (0 == FaceLoadLibrary())
	{
		//MessageBox("loadDll true");
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFaceLivingDetectionDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFaceLivingDetectionDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CFaceLivingDetectionDemoDlg::OnBnClickedButton1()
{
	//训练
	SvmTrain("C:\\Users\\Administrator\\Desktop\\FACE\\faceTrue","C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse","C:\\Users\\Administrator\\Desktop\\FACE\\svm.xml");
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton2()
{
	//加载
	LoadSvmModel("C:\\Users\\Administrator\\Desktop\\FACE\\svm.xml");
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton3()
{
	//识别
	Mat face = imread("C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse\\4.bmp");
	int nRet = nLivingDetector(face);
	CString str;
	if (nRet == -1 )
	{
		str = "未加载模型";
	}
	else
	{

		str = nRet == 0 ? "活体":"非活体";
	}
	MessageBox(str);
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton4()
{
	// 截取人脸
	FaceDetectionSave("C:\\Users\\Administrator\\Desktop\\FACE\\true","C:\\Users\\Administrator\\Desktop\\FACE\\faceTrue");
	//Sleep(1000);
	FaceDetectionSave("C:\\Users\\Administrator\\Desktop\\FACE\\false","C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse");

}
