
// ra2yuriEXE.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cra2yuriEXEApp:
// �йش����ʵ�֣������ ra2yuriEXE.cpp
//

class Cra2yuriEXEApp : public CWinApp
{
public:
	Cra2yuriEXEApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cra2yuriEXEApp theApp;