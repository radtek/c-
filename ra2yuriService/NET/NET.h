// NET.h : NET DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CNETApp
// �йش���ʵ�ֵ���Ϣ������� NET.cpp
//

class CNETApp : public CWinApp
{
public:
	CNETApp();



// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
