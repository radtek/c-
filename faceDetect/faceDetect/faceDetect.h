
// faceDetect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CfaceDetectApp:
// �йش����ʵ�֣������ faceDetect.cpp
//

class CfaceDetectApp : public CWinApp
{
public:
	CfaceDetectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CfaceDetectApp theApp;