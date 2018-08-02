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

struct FTPSERV_INFO 
{
	char szAccount[30];
	char szPassword[30];
	char szIP[100];
	int nPort;
	CHAR szLogPath[512];
};

class sftp
{
public:
	sftp(void);
	~sftp(void);

	static sftp * Instance();

	SHORT SftpInit();
	void SftpExit();
	SHORT CreatePath(string strRemotePath);
	void UpdateInfo(string strIP, int nPort, string strAccount, string strPassword);
	SHORT SftpUploadFile(string strLocalFilePath, string strRemFilePath);
	SHORT SftpRename(string strOldRemoteFile,string strNewRemoteFile);
	SHORT CreateDirUploadRename(string strDirPathName,string strLocalFile, string strRemoteFile,string strOldRemoteFile,string strNewRemoteFile);//创建目录、上传文件、修改名字
	BOOL SftpUploadFiles(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strSftpPath, CStringList &FilesList, CStringList &RenameFilesList);
private:
	int iSock;
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	LIBSSH2_SFTP *sftp_session;
	LIBSSH2_SESSION *session;
	FTPSERV_INFO m_ftpInfo;

	static	sftp	* m_Instance;
	static	void Destroy();
};
