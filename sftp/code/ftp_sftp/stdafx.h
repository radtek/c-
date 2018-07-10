// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#define _CSTDIO_
#define _CSTRING_
#define _CWCHAR_

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��


#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include <afx.h>
#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ
#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <iostream>

#include <wininet.h>
#pragma comment(lib,"wininet.lib")

#include <string>

#include <strsafe.h>
// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
#include <stdexcept>

#define  GWI_PLUGIN_ERROR   "0100000004"
#define  GWI_PLUGIN_SUCCESS  "0000000000"
#define  GWI_PLUGIN_INVALID_FILE "0000000001"
#define  LEN_1024  1024

#define  RETURN_SUCCESS(ret,errno,data){\
	sprintf_s((ret),LEN_1024,"{\"success\": true,\"errno\":\"%s\",\"description\":\"operation success\",\"data\":\"%s\"}",(errno),(data));\
	return (ret);\
}

#define  RETURN_ERROR(ret,errno,descript,data){\
	sprintf_s((ret),LEN_1024,"{\"success\": false,\"errno\":\"%s\",\"description\":\"%s\",\"data\":\"%s\"}",(errno),(descript),(data));\
	return (ret);\
}

#define  ASSERT_PTR(jason,ret) \
	strcpy_s((ret),11,GWI_PLUGIN_ERROR);\
	if((jason) == nullptr)\
{\
	RETURN_ERROR((ret),GWI_PLUGIN_ERROR,"���Ϊ��ָ��","");\
}

//ʵ���ͷ��ڴ�
#define DELETE_ARRAY(ptr) \
	if(nullptr != ptr)\
	{\
	delete[] ptr;\
	ptr = nullptr;\
	}

//json���� root���ڵ� item��Ӧ�ڵ� var������ ret���ر���
#define GET_JASON_OBJECT(root,item,var,ret) \
	try{\
	(item) = cJSON_GetObjectItem((root), (var));\
	}catch(std::runtime_error e){\
	cJSON_Delete((root));\
	RETURN_ERROR((ret),GWI_PLUGIN_ERROR,"��ȡjson�ڵ�ֵʧ��","");\
	}
