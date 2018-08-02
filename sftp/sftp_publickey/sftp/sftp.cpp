// sftp.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "sftp.h"

#include "cJSON.h"
#include "SFTPClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <string>
#include "strsafe.h"
#include<vector>

CWinApp theApp;

#include<vector>
using namespace std;

static bool bSftp = false;
static wchar_t wsIp[MAX_PATH] = {};
static wchar_t wsPort[MAX_PATH] = {};
static INTERNET_PORT intPort;
static wchar_t wsUsername[MAX_PATH] = {};
static wchar_t wsPassword[MAX_PATH] = {};
static wchar_t wsMode[MAX_PATH] = {};
static wchar_t wsDir[MAX_PATH] = {};

static char csIp[MAX_PATH] = {};
static char csPort[MAX_PATH] = {};
static char csUsername[MAX_PATH] = {};
static char csPassword[MAX_PATH] = {};
static char csDir[MAX_PATH] = {};

static HINTERNET hInetSession = NULL;
static HINTERNET hFtpConn = NULL;

static wchar_t wsPathSplit[] = L"\\|/"; //文件路径分隔符控制
static char    csPathSplit[] = "\\|/"; //文件路径分隔符控制

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

// CsftpApp

BEGIN_MESSAGE_MAP(CsftpApp, CWinApp)
END_MESSAGE_MAP()


// CsftpApp 构造

CsftpApp::CsftpApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CsftpApp 对象

//CsftpApp theApp;


// CsftpApp 初始化

BOOL CsftpApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/*

aim:便于以后可以根据分割符扩展压缩文件数量
author:wsh
time:20180403
input:
	string strTime:需要分割的字符串
output:
	vector<WCHAR*>:分割后保存的vector容器
*/
static std::vector<WCHAR*> split(const WCHAR* strTime)  
{  
	std::vector<WCHAR*> result;   
	unsigned int pos = 0;//字符串结束标记，方便将最后一个单词入vector  
	size_t i;
	for( i = 0; i < wcslen(strTime); i++)  
	{  
		if(strTime[i] == '|')  
		{  
			WCHAR* temp = new WCHAR[i-pos+1];
			memset(temp,0x00,sizeof(WCHAR));
			wcsncpy(temp,strTime+pos,i-pos);
			temp[i-pos] = 0x00;
			result.push_back(temp); 
			pos=i+1;
		}  
	}  
	//判断最后一个
	if (pos < i)
	{
		WCHAR* temp = new WCHAR[i-pos+1];
		memset(temp,0x00,sizeof(WCHAR));
		wcsncpy(temp,strTime+pos,i-pos);
		temp[i-pos] = 0x00;
		result.push_back(temp);
	}
	return result;  
} 

static const wchar_t* wcstrrchr(const wchar_t* str, const wchar_t wc)  
{  
	const wchar_t* pwc = NULL;  
	for (int i=wcslen(str)-1;i>=0;i--)  
	{  
		if (str[i] == wc)  
		{  
			pwc = str + i;  
			break;  
		}  
	}  
	return pwc;  
}  

static const wchar_t* wcstrrchr(const wchar_t* str, const wchar_t* wc) //wc格式"\\|/"包括'\\"和'/'2种分割
{
	std::vector<WCHAR*>  vec = split(wc);
	
	const wchar_t* pwc = NULL; 
	for (int i=wcslen(str)-1;i>=0;i--)  
	{  
		for (std::vector<WCHAR*>::const_iterator itr = vec.cbegin();itr!=vec.cend();itr++)
		{	
			if (wcsncmp(&str[i],*itr,1) == 0)  
			{  
				pwc = str + i;  
				return pwc;  
			}  
		}
		
	}  
	return pwc;  
}

static void getPath_Name(const wchar_t* str, const wchar_t* wc,wchar_t* wsPath, wchar_t* wsName)
{
	const wchar_t* pwc = wcstrrchr(str,wc);
	wsPath[0]=0;  
	wsName[0]=0;
	if (pwc)
	{
		for (int i=0; i<pwc-str; i++)  
			wsPath[i] = *(str+i); 
		StringCchCopy(wsName,MAX_PATH,pwc+1);
	}
}

static std::vector<char*> split(const char* strTime)  
{  
	std::vector<char*> result;   
	unsigned int pos = 0;//字符串结束标记，方便将最后一个单词入vector  
	size_t i;
	for( i = 0; i < strlen(strTime); i++)  
	{  
		if(strTime[i] == '|')  
		{  
			char* temp = new char[i-pos+1];
			memset(temp,0x00,sizeof(temp));
			strncpy(temp,strTime+pos,i-pos);
			temp[i-pos] = 0x00;
			result.push_back(temp); 
			pos=i+1;
		}  
	}  
	//判断最后一个
	if (pos < i)
	{
		char* temp = new char[i-pos+1];
		memset(temp,0x00,sizeof(temp));
		strncpy(temp,strTime+pos,i-pos);
		temp[i-pos] = 0x00;
		result.push_back(temp);
	}
	return result;  
} 

static const char* wcstrrchr(const char* str, const char* wc) //wc格式"\\|/"包括'\\"和'/'2种分割
{
	std::vector<char*>  vec = split(wc);

	const char* pwc = NULL; 
	for (int i=strlen(str)-1;i>=0;i--)  
	{  
		for (std::vector<char*>::const_iterator itr = vec.cbegin();itr!=vec.cend();itr++)
		{	
			if (strncmp(&str[i],*itr,1) == 0)  
			{  
				pwc = str + i;  
				return pwc;  
			}  
		}

	}  
	return pwc;  
}

static void getPath_Name(const char* str, const char* wc,char* wsPath, char* wsName)
{
	const char* pwc = wcstrrchr(str,wc);
	wsPath[0] = 0;
	wsName[0] = 0;
	if (pwc)
	{
		for (int i=0; i<pwc-str; i++)  
			wsPath[i] = *(str+i); 
		sprintf_s(wsName,MAX_PATH,"%s",pwc+1);
	}
}

static bool createFtpMultiDir(const wchar_t* path)  
{  

	if (path == NULL || path == L"" || wcslen(path)<=0) return true;
	const wchar_t* pwcPath = wcstrrchr(path,L"/");
	wchar_t wsPath[MAX_PATH] = {};
	if (pwcPath)
	{
		for (int i=0; i<pwcPath-path; i++)  
			wsPath[i] = *(path+i); 
	}
	createFtpMultiDir(wsPath);
	if(FtpCreateDirectory(hFtpConn,path)) return true;  
	return false;  
}  

static bool createMultiDir(const wchar_t* path)  
{  
	if (path == NULL) return false;  
	if (PathIsDirectory(path)) return true;  

	wchar_t wsSubPath[MAX_PATH] = {}; 
	wchar_t wsSubName[MAX_PATH] = {};
	getPath_Name(path,wsPathSplit,wsSubPath,wsSubName); 
	if (wcslen(wsSubPath) )
		createMultiDir(wsSubPath);
	if(CreateDirectory(path,NULL)) return true;  
	return false;  
} 

static bool createMultiDir(const char* path)  
{  
	if (path == NULL) return false;  
	if (PathIsDirectoryA(path)) return true;  

	char csSubPath[MAX_PATH] = {}; 
	char csSubName[MAX_PATH] = {};
	getPath_Name(path,csPathSplit,csSubPath,csSubName); 
	if (strlen(csSubPath) )
		createMultiDir(csSubPath);
	if(CreateDirectoryA(path,NULL)) return true;  
	return false;  
} 


bool cd(const char* szDir)
{
	
	int ivalue = GWI_SFTPClient::sftpCd(szDir);
	if (ivalue)
	{
		return false;
	}
	return true;
}

bool ftpopenPublicKey(const char* szIp, int nPort,const char* szUsername, const char* szPassword, const char* szPublicKey , const char* szPrivateKey )
{
	int ivalue = GWI_SFTPClient::sftpOpen(szIp, nPort,szUsername,szPassword,szPublicKey,szPrivateKey);
	if (ivalue)
	{
		return false;
	}
	return true;
}

bool ftpopenPassword(const char* szIp, int nPort,const char* szUsername, const char* szPassword)
{
	int ivalue = GWI_SFTPClient::sftpOpen(szIp, nPort,szUsername,szPassword);
	if (ivalue)
	{
		return false;
	}
	return true;
}

bool uploaddir(const char* szFtpDir, const char* szLocalDir)
{
	
	int ivalue = GWI_SFTPClient::sftpUpdateFileDir(szFtpDir,szLocalDir);
	if (ivalue)
	{
		return false;
	}
	return true;

}
bool uploadfile(const char* szFtpPath, const char* szLocalFile, const char* szFtpFile = NULL)
{
	
	int ivalue = GWI_SFTPClient::uploadFile(szFtpPath,szLocalFile,szFtpFile);
	if (ivalue)
	{
		return false;
	}
	return true;
	
}

bool ftpclose()
{
	
	int ivalue = GWI_SFTPClient::sftpClose();
	if (ivalue)
	{
		return false;
	}
	return true;
	
}


bool downloaddir(const char* szFtpDir, const char* szLocalDir)
{
	createMultiDir(szFtpDir);
	int ivalue = GWI_SFTPClient::sftpDwnloadFileDir(szFtpDir,szLocalDir);
	if (ivalue)
	{
		return false;
	}
	return true;
	
}

bool downloadfile(const char* szFtpFile, const char* szLocalFile)
{
	
	int ivalue = GWI_SFTPClient::downloadFile(szFtpFile,szLocalFile);
	if (ivalue)
	{
		return false;
	}
	return true;
	
}

bool ftprename(const char* szFtpFileName, const char* szNewFileName)
{
	int ivalue = GWI_SFTPClient::sftpRename(szFtpFileName,szNewFileName);
	if (ivalue)
	{
		return false;
	}
	return true;
}