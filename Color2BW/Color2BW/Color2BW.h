// Color2BW.h : Color2BW DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CColor2BWApp
// �йش���ʵ�ֵ���Ϣ������� Color2BW.cpp
//

class CColor2BWApp : public CWinApp
{
public:
	CColor2BWApp();
	int Color2BW(const char * szSrcPath,const char * szDstPath);

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
