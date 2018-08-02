// sftp2.h : sftp2 DLL ����ͷ�ļ�
//
#pragma once
#include ".\libssh2\include\libssh2_config.h"
#include ".\libssh2\include\libssh2.h"
#include ".\libssh2\include\libssh2_sftp.h"
#include <string>
#include <vector>
using namespace std;

#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\libssh2.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\libeay32.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\zlib.lib" )

//#include "Logger.h"
//using namespace LOGGER;

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Csftp2App
// �йش���ʵ�ֵ���Ϣ������� sftp2.cpp
//

struct FTPSERV_INFO 
{
	char szAccount[30];
	char szPassword[30];
	char szIP[100];
	int nPort;
	CHAR szLogPath[512];
};

class Csftp2App : public CWinApp
{
public:
	Csftp2App();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()

	SHORT SftpInit();
	void SftpExit();
	SHORT CreatePath(string strRemotePath);
	void UpdateInfo(string strIP, int nPort, string strAccount, string strPassword);
	SHORT SftpUploadFile(string strLocalFilePath, string strRemFilePath);
	SHORT SftpRename(string strOldRemoteFile,string strNewRemoteFile);
	SHORT CreateDirUploadRename(string strDirPathName,string strLocalFile, string strRemoteFile,string strOldRemoteFile,string strNewRemoteFile);//����Ŀ¼���ϴ��ļ����޸�����
	BOOL SftpUploadFiles(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strSftpPath, CStringList &FilesList, CStringList &RenameFilesList);
private:
	int iSock;
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	LIBSSH2_SFTP *sftp_session;
	LIBSSH2_SESSION *session;
	FTPSERV_INFO m_ftpInfo;
};
