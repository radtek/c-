
// ra2yuriClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cra2yuriClientApp:
// �йش����ʵ�֣������ ra2yuriClient.cpp
//

class Cra2yuriClientApp : public CWinApp
{
public:
	Cra2yuriClientApp();

	HANDLE m_handle;
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern Cra2yuriClientApp theApp;