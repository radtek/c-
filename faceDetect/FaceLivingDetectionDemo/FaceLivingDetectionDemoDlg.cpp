
// FaceLivingDetectionDemoDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "FaceLivingDetectionDemo.h"
#include "FaceLivingDetectionDemoDlg.h"
#include "afxdialogex.h"
#include "FaceLivingDetectionDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CFaceLivingDetectionDemoDlg �Ի���




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


// CFaceLivingDetectionDemoDlg ��Ϣ�������

BOOL CFaceLivingDetectionDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (0 == FaceLoadLibrary())
	{
		//MessageBox("loadDll true");
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFaceLivingDetectionDemoDlg::OnPaint()
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
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFaceLivingDetectionDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CFaceLivingDetectionDemoDlg::OnBnClickedButton1()
{
	//ѵ��
	SvmTrain("C:\\Users\\Administrator\\Desktop\\FACE\\faceTrue","C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse","C:\\Users\\Administrator\\Desktop\\FACE\\svm.xml");
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton2()
{
	//����
	LoadSvmModel("C:\\Users\\Administrator\\Desktop\\FACE\\svm.xml");
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton3()
{
	//ʶ��
	Mat face = imread("C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse\\4.bmp");
	int nRet = nLivingDetector(face);
	CString str;
	if (nRet == -1 )
	{
		str = "δ����ģ��";
	}
	else
	{

		str = nRet == 0 ? "����":"�ǻ���";
	}
	MessageBox(str);
}


void CFaceLivingDetectionDemoDlg::OnBnClickedButton4()
{
	// ��ȡ����
	FaceDetectionSave("C:\\Users\\Administrator\\Desktop\\FACE\\true","C:\\Users\\Administrator\\Desktop\\FACE\\faceTrue");
	//Sleep(1000);
	FaceDetectionSave("C:\\Users\\Administrator\\Desktop\\FACE\\false","C:\\Users\\Administrator\\Desktop\\FACE\\faceFalse");

}
