
// ra2yuriService.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cra2yuriServiceApp:
// �йش����ʵ�֣������ ra2yuriService.cpp
//

class Cra2yuriServiceApp : public CWinApp
{
public:
	Cra2yuriServiceApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cra2yuriServiceApp theApp;