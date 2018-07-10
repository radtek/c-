// ftp_sftp.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ftp_sftp.h"
#include "cJSON.h"
#include "SFTPClient.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "strsafe.h"
#include<vector>
// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

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

static wchar_t wsPathSplit[] = L"\\|/"; //�ļ�·���ָ�������
static char    csPathSplit[] = "\\|/"; //�ļ�·���ָ�������

/*

aim:�����Ժ���Ը��ݷָ����չѹ���ļ�����
author:wsh
time:20180403
input:
	string strTime:��Ҫ�ָ���ַ���
output:
	vector<WCHAR*>:�ָ�󱣴��vector����
*/
static std::vector<WCHAR*> split(const WCHAR* strTime)  
{  
	std::vector<WCHAR*> result;   
	unsigned int pos = 0;//�ַ���������ǣ����㽫���һ��������vector  
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
	//�ж����һ��
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

static const wchar_t* wcstrrchr(const wchar_t* str, const wchar_t* wc) //wc��ʽ"\\|/"����'\\"��'/'2�ַָ�
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
	unsigned int pos = 0;//�ַ���������ǣ����㽫���һ��������vector  
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
	//�ж����һ��
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

static const char* wcstrrchr(const char* str, const char* wc) //wc��ʽ"\\|/"����'\\"��'/'2�ַָ�
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

const char* ftpopen(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root)
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}

	memset(csIp,0,sizeof(csIp));
	memset(csPort,0,sizeof(csPort));
	memset(csUsername,0,sizeof(csUsername));
	memset(csPassword,0,sizeof(csPassword));

	memset(wsIp,0,sizeof(wsIp));
	memset(wsPort,0,sizeof(wsPort));
	memset(wsUsername,0,sizeof(wsUsername));
	memset(wsPassword,0,sizeof(wsPassword));
	memset(wsMode,0,sizeof(wsMode));

	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"ip",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsIp,sizeof(wsIp)/sizeof(wsIp[0]));
	sprintf_s(csIp,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"port",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsPort,sizeof(wsPort)/sizeof(wsPort[0]));
	sprintf_s(csPort,MAX_PATH,"%s",item->valuestring);
	intPort = (INTERNET_PORT)atoi(item->valuestring);
	GET_JASON_OBJECT(root,item,"username",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsUsername,sizeof(wsUsername)/sizeof(wsUsername[0]));
	sprintf_s(csUsername,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"password",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsPassword,sizeof(wsPassword)/sizeof(wsPassword[0]));
	sprintf_s(csPassword,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"mode",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsMode,sizeof(wsMode)/sizeof(wsMode[0]));
	cJSON_Delete(root);
	bSftp = wsMode[0] == L'1'?true:false;

	if (bSftp)
	{
		//�����ӿ�
		int ivalue = GWI_SFTPClient::sftpOpen(csIp,intPort,csUsername,csPassword);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		hInetSession=InternetOpen(AfxGetAppName(),INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
		if(!hInetSession) return ret;
		hFtpConn=InternetConnect(hInetSession,wsIp,intPort,
			wsUsername,wsPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
		if(!hFtpConn) 
		{
			InternetCloseHandle(hInetSession);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"��������ʧ��","");
		}
	}
	
	RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
}

const char* cd(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root) 
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}

	char csDir[MAX_PATH] = {};
	memset(csDir,0,sizeof(csDir));
	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"dir",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsDir,sizeof(wsDir)/sizeof(wsDir[0]));
	sprintf_s(csDir,MAX_PATH,"%s",item->valuestring);
	cJSON_Delete(root);

	if (bSftp)
	{
		int ivalue = GWI_SFTPClient::sftpCd(csDir);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if(wcslen(wsDir)!=0 && hFtpConn!=NULL && FtpSetCurrentDirectory(hFtpConn,wsDir))
		{
			RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
		}
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"���õ�ǰ·��ʧ��","");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}

//����Ҫ�������ļ���·������������Ӧ�ļ��� �õ��ļ�����Ҷ�ӽڵ� 
static bool TraverseDirectory(wchar_t Dir[MAX_PATH],wchar_t FtpDir[MAX_PATH],wchar_t ExternDir[MAX_PATH])      
{  
	WIN32_FIND_DATA FindFileData;  
	HANDLE hFind=INVALID_HANDLE_VALUE;  
	wchar_t DirSpec[MAX_PATH];                  //����Ҫ�������ļ��е�Ŀ¼  
	StringCchCopy(DirSpec,MAX_PATH,Dir);
	StringCchCat(DirSpec,MAX_PATH,L"\\*");
 
	bool bIsEmptyFolder = true;
	hFind=FindFirstFile(DirSpec,&FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  
	bool ret;
	if(hFind==INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
	{  
		ret =  false;    
	}  
	else   
	{   
		while(FindNextFile(hFind,&FindFileData)!=0)                            //���ļ������ļ��д���ʱ  
		{  
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0&&wcscmp(FindFileData.cFileName,L".")==0||wcscmp(FindFileData.cFileName,L"..")==0)        //�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
			{  
				continue;  
			}  
			wchar_t DirAdd[MAX_PATH];  
			StringCchCopy(DirAdd,MAX_PATH,Dir);  
			StringCchCat(DirAdd,MAX_PATH,TEXT("\\"));  
			StringCchCat(DirAdd,MAX_PATH,FindFileData.cFileName); 
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0)      //�ж�������ļ���  
			{  
				//ƴ�ӵõ����ļ��е�����·��  
				bIsEmptyFolder = false;
				wchar_t ExternDirAdd[MAX_PATH];
				StringCchCopy(ExternDirAdd,MAX_PATH,ExternDir);  
				StringCchCat(ExternDirAdd,MAX_PATH,TEXT("/")); 
				StringCchCat(ExternDirAdd,MAX_PATH,FindFileData.cFileName); 
				ret = TraverseDirectory(DirAdd,FtpDir,ExternDirAdd);                                  //ʵ�ֵݹ����  
				if(!ret)
				{
					FindClose(hFind);  
					return ret;
				}
			}  
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)    //��������ļ���  
			{  
				//wcout<<Dir<<"\\"<<FindFileData.cFileName<<endl;            //�������·��  
				bIsEmptyFolder = false;
				wchar_t FtpDirAdd[MAX_PATH] = {};
				StringCchCopy(FtpDirAdd,MAX_PATH,FtpDir);  
				StringCchCat(FtpDirAdd,MAX_PATH,ExternDir); 
				createFtpMultiDir(FtpDirAdd);
				FtpSetCurrentDirectory(hFtpConn,FtpDirAdd);
				SetCurrentDirectory(Dir);
				ret = FtpPutFile(hFtpConn,FindFileData.cFileName,FindFileData.cFileName,FTP_TRANSFER_TYPE_BINARY,INTERNET_FLAG_HYPERLINK);
			    if(!ret)
				{
					FindClose(hFind);  
					return ret;
				}
			}  
		}  
		if (bIsEmptyFolder)   //���ļ���
		{
			wchar_t FtpDirAdd[MAX_PATH] = {};
			StringCchCopy(FtpDirAdd,MAX_PATH,FtpDir); 
			StringCchCat(FtpDirAdd,MAX_PATH,ExternDir); 
			ret = createFtpMultiDir(FtpDirAdd);
		}
		FindClose(hFind);  
	} 
	return ret;
} 

const char* uploaddir(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root)
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}

	wchar_t wsSrcDir[MAX_PATH] = {};
	wchar_t wsDestDir[MAX_PATH] = {};
	memset(wsSrcDir,0,sizeof(wsSrcDir));
	memset(wsDestDir,0,sizeof(wsDestDir));

	char csDestDir[MAX_PATH] = {};
	char csSrcDir[MAX_PATH] = {};
	memset(csDestDir,0,sizeof(csDestDir));
	memset(csSrcDir,0,sizeof(csSrcDir));

	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"srcdir",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsSrcDir,sizeof(wsSrcDir)/sizeof(wsSrcDir[0]));
	sprintf_s(csSrcDir,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"destdir",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsDestDir,sizeof(wsDestDir)/sizeof(wsDestDir[0]));
	sprintf_s(csDestDir,MAX_PATH,"%s",item->valuestring);
	cJSON_Delete(root);

	WIN32_FIND_DATA FindFileData;  
	HANDLE hFind=INVALID_HANDLE_VALUE;  
	hFind=FindFirstFile(wsSrcDir,&FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  
	if(hFind==INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
	{  
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"ԭ�ļ�������",""); 
	}  
	if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0) //�ļ�����δ���
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�����ļ��ж����ļ�,����uploadfile",""); 
	}

	if (bSftp)
	{
		int ivalue = GWI_SFTPClient::sftpUpdateFileDir(csDestDir,csSrcDir);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if (hFtpConn == NULL) 
		{
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"����ʧ��","");
		}
		createFtpMultiDir(wsDestDir);
		//�ݹ��ϴ�
		if(TraverseDirectory(wsSrcDir,wsDestDir,L""))
		{
			RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
		}
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�ϴ��ļ���ʧ��","");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}
const char* uploadfile(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root)
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}

	wchar_t wsSrcfilename[MAX_PATH] = {};
	wchar_t wsDestfilename[MAX_PATH] = {};
	memset(wsSrcfilename,0,sizeof(wsSrcfilename));
	memset(wsDestfilename,0,sizeof(wsDestfilename));

	char csSrcfilename[MAX_PATH] = {};
	char csDestfilename[MAX_PATH] = {};
	memset(csSrcfilename,0,sizeof(csSrcfilename));
	memset(csDestfilename,0,sizeof(csDestfilename));

	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"srcfilename",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsSrcfilename,sizeof(wsSrcfilename)/sizeof(wsSrcfilename[0]));
	sprintf_s(csSrcfilename,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"destfilename",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsDestfilename,sizeof(wsDestfilename)/sizeof(wsDestfilename[0]));
	sprintf_s(csDestfilename,MAX_PATH,"%s",item->valuestring);
	cJSON_Delete(root);

	WIN32_FIND_DATA FindFileData;  
	HANDLE hFind=INVALID_HANDLE_VALUE;  
	hFind=FindFirstFile(wsSrcfilename,&FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  
	if(hFind==INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
	{  
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"ԭ�ļ�������",""); 
	}  
	if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0) //�ļ��У���δ���
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�����ļ������ļ���,����uploaddir",""); 
	}

	if (bSftp)
	{
		//sftp����
		char csPath[MAX_PATH] = {};
		char csName[MAX_PATH] = {};
		getPath_Name(csDestfilename,csPathSplit,csPath,csName);
		char* pcName = strlen(csName)>0?csName:nullptr;
		int ivalue = GWI_SFTPClient::uploadFile(csPath,csSrcfilename,pcName);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if (hFtpConn == NULL) 
		{
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"����ʧ��","");
		}
	
		wchar_t wsSubPath[MAX_PATH] = {}; 
		wchar_t wsSubName[MAX_PATH] = {}; 
		getPath_Name(wsDestfilename,wsPathSplit,wsSubPath,wsSubName);
		if (wcslen(wsSubPath))
			createFtpMultiDir(wsSubPath);
		else
		{
			DWORD dwPathLen;
			FtpGetCurrentDirectory(hFtpConn,wsSubPath,&dwPathLen);
			StringCchCopy(wsSubName,MAX_PATH,wsDestfilename);
		}
		FtpSetCurrentDirectory(hFtpConn,wsSubPath);

		if(FtpPutFile(hFtpConn,wsSrcfilename,wsSubName,FTP_TRANSFER_TYPE_BINARY,INTERNET_FLAG_HYPERLINK))
		{
			RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
		}
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�ϴ��ļ�ʧ��","");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}

static bool bNotFtpFindFirstFile = true;
static bool preDownloaddir(wchar_t FtpDir[MAX_PATH],wchar_t LocalDir[MAX_PATH],wchar_t ExternDir[MAX_PATH]) 
{
	bool bret = false;bool bIsEmptyFolder = true;
	WIN32_FIND_DATA findData;
	HINTERNET hFind = NULL;


	wchar_t wsLocalDirAdd[MAX_PATH] = {};
	StringCchCopy(wsLocalDirAdd,MAX_PATH,LocalDir);  
	StringCchCat(wsLocalDirAdd,MAX_PATH,ExternDir);
	createMultiDir(wsLocalDirAdd);

	HINTERNET hFtpConn_T=InternetConnect(hInetSession,wsIp,intPort,wsUsername,wsPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
	FtpSetCurrentDirectory(hFtpConn_T,FtpDir);
	if(!(hFind=FtpFindFirstFile(hFtpConn_T,_T("*"),&findData,0,0)))
	{
		if (GetLastError()!= ERROR_NO_MORE_FILES)
			bret=FALSE;
		else
			bret=TRUE;
		InternetCloseHandle(hFtpConn_T);
		return bret;
	}
	
	do
	{
		if((findData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0&&wcscmp(findData.cFileName,L".")==0||wcscmp(findData.cFileName,L"..")==0)        //�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
		{  
			continue;  
		}  
		if(findData.dwFileAttributes==FILE_ATTRIBUTE_DIRECTORY)
		{
			bIsEmptyFolder = false;
			wchar_t wsFtpDirAdd[MAX_PATH] = {}; 
			wchar_t wsExternDirAdd[MAX_PATH] = {}; 
			StringCchCopy(wsFtpDirAdd,MAX_PATH,FtpDir);  
			StringCchCat(wsFtpDirAdd,MAX_PATH,TEXT("/")); 
			StringCchCat(wsFtpDirAdd,MAX_PATH,findData.cFileName);

			StringCchCopy(wsExternDirAdd,MAX_PATH,ExternDir);  
			StringCchCat(wsExternDirAdd,MAX_PATH,TEXT("\\")); 
			StringCchCat(wsExternDirAdd,MAX_PATH,findData.cFileName);
			bret = preDownloaddir(wsFtpDirAdd,LocalDir,wsExternDirAdd);
			if (!bret)
			{
				InternetCloseHandle(hFtpConn_T);
				return bret;
			}
		}
		else
		{
			SetCurrentDirectory(wsLocalDirAdd);
			FtpSetCurrentDirectory(hFtpConn_T,FtpDir);

			bret = FtpGetFile(hFtpConn_T,findData.cFileName,findData.cFileName,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY|
				INTERNET_FLAG_NO_CACHE_WRITE,0);
			if (!bret)
			{
				InternetCloseHandle(hFtpConn_T);
				break;
			}
		}
	}while(InternetFindNextFile(hFind,&findData));
	InternetCloseHandle(hFtpConn_T);
	return bret;
}
const char* downloaddir(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root)
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}
	wchar_t wsDestDir[MAX_PATH] = {};
	wchar_t wsSrcDir[MAX_PATH] = {};
	memset(wsDestDir,0,sizeof(wsDestDir));
	memset(wsSrcDir,0,sizeof(wsSrcDir));

	char csDestDir[MAX_PATH] = {};
	char csSrcDir[MAX_PATH] = {};
	memset(csDestDir,0,sizeof(csDestDir));
	memset(csSrcDir,0,sizeof(csSrcDir));

	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"srcdir",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsSrcDir,sizeof(wsSrcDir)/sizeof(wsSrcDir[0]));
	sprintf_s(csSrcDir,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"destdir",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsDestDir,sizeof(wsDestDir)/sizeof(wsDestDir[0]));
	sprintf_s(csDestDir,MAX_PATH,"%s",item->valuestring);
	cJSON_Delete(root);

	if (bSftp)
	{
		createMultiDir(csDestDir);
		int ivalue = GWI_SFTPClient::sftpDwnloadFileDir(csSrcDir,csDestDir);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if (hFtpConn == NULL) 
		{
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"����ʧ��","");
		}
		createMultiDir(wsDestDir);
		FtpSetCurrentDirectory(hFtpConn,wsSrcDir);

		if(preDownloaddir(wsSrcDir,wsDestDir,L""))
		{
			RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
		}
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�����ļ���ʧ��","");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}

const char* downloadfile(const char* jason)
{
	char* ret = new char[LEN_1024];
	ASSERT_PTR(jason,ret);
	cJSON * root = cJSON_Parse(jason);
	if (!root)
	{
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"json����ʧ��","");
	}

	wchar_t wsSrcfilename[MAX_PATH] = {};
	wchar_t wsDestfilename[MAX_PATH] = {};
	memset(wsSrcfilename,0,sizeof(wsSrcfilename));
	memset(wsDestfilename,0,sizeof(wsDestfilename));

	char csSrcfilename[MAX_PATH] = {};
	char csDestfilename[MAX_PATH] = {};
	memset(csSrcfilename,0,sizeof(csSrcfilename));
	memset(csDestfilename,0,sizeof(csDestfilename));

	cJSON * item = NULL;
	GET_JASON_OBJECT(root,item,"srcfilename",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsSrcfilename,sizeof(wsSrcfilename)/sizeof(wsSrcfilename[0]));
	sprintf_s(csSrcfilename,MAX_PATH,"%s",item->valuestring);
	GET_JASON_OBJECT(root,item,"destfilename",ret);
	MultiByteToWideChar(CP_ACP,0,item->valuestring,strlen(item->valuestring)+1,wsDestfilename,sizeof(wsDestfilename)/sizeof(wsDestfilename[0]));
	sprintf_s(csDestfilename,MAX_PATH,"%s",item->valuestring);
	cJSON_Delete(root);

	if (bSftp)
	{
		//sftp����
		int ivalue = GWI_SFTPClient::downloadFile(csSrcfilename,csDestfilename);
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if (hFtpConn == NULL) 
		{
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"����ʧ��","");
		}
		HINTERNET hFtpConn_T=InternetConnect(hInetSession,wsIp,intPort,wsUsername,wsPassword,INTERNET_SERVICE_FTP,INTERNET_FLAG_PASSIVE,0);
		const wchar_t* pwcSrcfilename = wcstrrchr(wsSrcfilename,L'/');
		wchar_t wsSubPath[MAX_PATH] = {}; 
		wchar_t wsSubName[MAX_PATH] = {}; 
		getPath_Name(wsSrcfilename,wsPathSplit,wsSubPath,wsSubName);
		if (wcslen(wsSubPath) <= 0)
		{
			DWORD dwPathLen;
			FtpGetCurrentDirectory(hFtpConn,wsSubPath,&dwPathLen);
			StringCchCopy(wsSubName,MAX_PATH,wsSrcfilename);
		}
		FtpSetCurrentDirectory(hFtpConn_T,wsSubPath);
		wchar_t wsDestPath[MAX_PATH] = {}; 
		wchar_t wsDestName[MAX_PATH] = {}; 
		getPath_Name(wsDestfilename,wsPathSplit,wsDestPath,wsDestName);
		if (wcslen(wsDestPath) <= 0)
		{
			GetCurrentDirectory(MAX_PATH,wsDestPath);
			StringCchCopy(wsDestName,MAX_PATH,wsDestfilename);
		}
		createMultiDir(wsDestPath);
		SetCurrentDirectory(wsDestPath);

		bool  bFtpGetFileFlag = FtpGetFile(hFtpConn_T,wsSubName,wsDestName,FALSE,FILE_ATTRIBUTE_NORMAL,FTP_TRANSFER_TYPE_BINARY|
			INTERNET_FLAG_NO_CACHE_WRITE,0);
		InternetCloseHandle(hFtpConn_T);
		if(bFtpGetFileFlag)
		{
			RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
		}
		RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"�����ļ�ʧ��","");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}
const char* ftpclose(const char* jason)
{
	char* ret = new char[LEN_1024];
	if (bSftp)
	{
		//sftp����
		int ivalue = GWI_SFTPClient::sftpClose();
		if (ivalue)
		{
			char csErrorMsg[MAX_PATH] = {};
			GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(ivalue,csErrorMsg);
			RETURN_ERROR(ret,GWI_PLUGIN_ERROR,csErrorMsg,"");
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	else
	{
		if (hFtpConn != NULL)
		{
			InternetCloseHandle(hFtpConn);
		}
		if (hInetSession != NULL)
		{
			InternetCloseHandle(hInetSession);
		}
		RETURN_SUCCESS(ret,GWI_PLUGIN_SUCCESS,"");
	}
	RETURN_ERROR(ret,GWI_PLUGIN_ERROR,"","");
}


/*
int main(int argc, char *argv[])
{
	uploadFile("127.0.0.1",
			   22,
			   "user",
			   "password",
			   "/test/qee/eee/",
			   "c:\\����Ʊ��v1.0.0.5.20150720.exe",
			   "v1.0.0.5.20150720.exe");

	downloadFile("127.0.0.1",
			   22,
			   "user",
			   "password",
			   "/test/qee/eee/v1.0.0.5.20150720.exe",
			   "c:\\dsdfsdf\\adf\\5.ssss.exe");
    return 0;
}
*/