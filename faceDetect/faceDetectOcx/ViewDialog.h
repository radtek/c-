#pragma once
#include "afxwin.h"


// CViewDialog 对话框

class CViewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDialog)

public:
	CViewDialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CViewDialog();

// 对话框数据


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cameraImage;
	void showCamera(cv::Mat mat);
	virtual BOOL OnInitDialog();
};
