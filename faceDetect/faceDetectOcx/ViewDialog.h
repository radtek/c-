#pragma once
#include "afxwin.h"


// CViewDialog �Ի���

class CViewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CViewDialog)

public:
	CViewDialog(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CViewDialog();

// �Ի�������


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cameraImage;
	void showCamera(cv::Mat mat);
	virtual BOOL OnInitDialog();
};
