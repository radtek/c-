
// imgTcpService.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CimgTcpServiceApp:
// �йش����ʵ�֣������ imgTcpService.cpp
//

class CimgTcpServiceApp : public CWinApp
{
public:
	CimgTcpServiceApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CimgTcpServiceApp theApp;