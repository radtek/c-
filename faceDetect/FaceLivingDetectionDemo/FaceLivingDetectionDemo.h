
// FaceLivingDetectionDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFaceLivingDetectionDemoApp:
// �йش����ʵ�֣������ FaceLivingDetectionDemo.cpp
//

class CFaceLivingDetectionDemoApp : public CWinApp
{
public:
	CFaceLivingDetectionDemoApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CFaceLivingDetectionDemoApp theApp;