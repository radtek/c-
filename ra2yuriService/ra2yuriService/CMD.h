// CMD.h : CMD DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CCMDApp
// �йش���ʵ�ֵ���Ϣ������� CMD.cpp
//

class CCMDApp : public CWinApp
{
public:
	CCMDApp();
	CString strCmdSend(CString str);
// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
