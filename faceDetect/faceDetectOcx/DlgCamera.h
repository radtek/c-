#pragma once
#include "stdafx.h"
#include "afxwin.h"

// CDlgCamera �Ի���

class CDlgCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCamera)

public:
	CDlgCamera(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgCamera();

// �Ի�������
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void showCameraPic(cv::Mat &img,Size &size);
	CStatic m_staticPic;

};
extern CDlgCamera g_dlgCamera;