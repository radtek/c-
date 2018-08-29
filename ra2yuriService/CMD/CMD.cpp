// CMD.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "CMD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CCMDApp

BEGIN_MESSAGE_MAP(CCMDApp, CWinApp)
END_MESSAGE_MAP()


// CCMDApp 构造

CCMDApp::CCMDApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CCMDApp 对象

CCMDApp theApp;


// CCMDApp 初始化

BOOL CCMDApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

TCHAR* StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
//输出cmd并返回数据
CString CCMDApp::strCmdSend(CString str)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		//MessageBox(NULL,"Error on CreatePipe()!",NULL,NULL);
		return NULL;
	}
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline=StringToChar(str);
	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		//MessageBox(NULL,"Error on CreateProcess()!",NULL,NULL);
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer,0,4096);
	CString output;
	DWORD byteRead;
	while(true)
	{
		if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	return output;
}