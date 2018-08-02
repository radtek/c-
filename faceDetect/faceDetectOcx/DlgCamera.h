#pragma once
#include "stdafx.h"
#include "afxwin.h"

// CDlgCamera 对话框

class CDlgCamera : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCamera)

public:
	CDlgCamera(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgCamera();

// 对话框数据
	enum { IDD = IDD_DLG_CAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void showCameraPic(cv::Mat &img,Size &size);
	CStatic m_staticPic;

};
extern CDlgCamera g_dlgCamera;