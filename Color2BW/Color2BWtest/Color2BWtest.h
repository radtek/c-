
// Color2BWtest.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CColor2BWtestApp:
// �йش����ʵ�֣������ Color2BWtest.cpp
//

class CColor2BWtestApp : public CWinApp
{
public:
	CColor2BWtestApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CColor2BWtestApp theApp;