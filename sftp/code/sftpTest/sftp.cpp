#include "StdAfx.h"
#include "sftp.h"

#ifdef HAVE_WINSOCK2_H
#include <winsock2.h>
#endif
#ifdef HAVE_SYS_SOCKET_H
#include <sys/socket.h>
#endif
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_ARPA_INET_H
#include <arpa/inet.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif

#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <ctype.h>


//冠字号FSN文件相关信息配置文件
//CONST string FSN_FTP_INI_PATH = "C:\\eastcom\\App\\conf\\TransParm.ini";

sftp::sftp(void)
{
	ZeroMemory(&m_ftpInfo, sizeof(m_ftpInfo));
}

sftp::~sftp(void)
{
}

sftp * sftp::m_Instance = NULL;

sftp * sftp::Instance()
{
	if(NULL == m_Instance)
	{
		m_Instance = new sftp();
		ASSERT(m_Instance);
		atexit(Destroy);
	}

	return m_Instance;
}

void sftp::Destroy()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str()); 

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, d);
	}

	return res;
}

//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  初始化sftp
//=========================================================================
SHORT sftp::SftpInit()
{
	unsigned long hostaddr;
	struct sockaddr_in sin;
	const char *fingerprint;
	SHORT sRet = 0;
	
	WSADATA wsadata;
	int iErr;

	iErr = WSAStartup(MAKEWORD(2,0), &wsadata);
	if (iErr != 0) {
		//CLogger::Instance()->TraceInfo("WSAStartup failed with error: %d\n", iErr);
		return 1;
	}
	
	hostaddr = inet_addr(m_ftpInfo.szIP);
	sRet = libssh2_init (0);

    if (sRet != 0) {
        //CLogger::Instance()->TraceInfo("libssh2 initialization failed (%d)\n", sRet);
        return 1;
    }
 
    /*
     * The application code is responsible for creating the socket
     * and establishing the connection
     */ 
    iSock = socket(AF_INET, SOCK_STREAM, 0);
 
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_ftpInfo.nPort);
    sin.sin_addr.s_addr = hostaddr;
    if (connect(iSock, (struct sockaddr*)(&sin),
                sizeof(struct sockaddr_in)) != 0) {
        //CLogger::Instance()->TraceInfo("failed to connect!\n");
        return 1;
    }
 
    /* Create a session instance
     */ 
	sftp::session = libssh2_session_init();

    if(!session)
        return 1;
 
    /* Since we have set non-blocking, tell libssh2 we are blocking */ 
    libssh2_session_set_blocking(session, 1);

 
    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */ 
    sRet = libssh2_session_handshake(session, iSock);

    if(sRet) {
        //CLogger::Instance()->TraceInfo("Failure establishing SSH session: %d\n", sRet);
        return 1;
    }
 
    /* At this point we havn't yet authenticated.  The first thing to do
     * is check the hostkey's fingerprint against our known hosts Your app
     * may have it hard coded, may go to a file, may present it to the
     * user, that's your call
     */ 
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
 
	/* We could authenticate via password */ 
	if (libssh2_userauth_password(session, m_ftpInfo.szAccount, m_ftpInfo.szPassword)) {

		//CLogger::Instance()->TraceInfo("Authentication by password failed.\n");
		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(iSock);
		libssh2_exit();

		return 1;
	}

    sftp_session = libssh2_sftp_init(session);
    if (!sftp_session) {
        //CLogger::Instance()->TraceInfo("Unable to init SFTP session\n");
		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(iSock);
		libssh2_exit();

		return 1;
    }

	return 0;
}

//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  退出释放相应资源
//=========================================================================
void sftp::SftpExit()
{
	libssh2_sftp_close(sftp_handle);
	libssh2_sftp_shutdown(sftp_session);
	libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
	libssh2_session_free(session);
	closesocket(iSock);
	libssh2_exit();

	return;
}


//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  远程创建路径
//=========================================================================
SHORT sftp::CreatePath(string strRemotePath)
{
	SHORT sRet = 0;
	vector<string> vecPath = split(strRemotePath,"/");
	//CParameterList param((PCHAR)strRemotePath.c_str(), '/');
	int iCount = 0;
	int i = 0;
	string strSftpPath = "/";
	
	iCount = vecPath.size();
	//iCount = param.GetCount();

	for (i = 0; i < iCount; i++)
	{
		strSftpPath = strSftpPath + vecPath[i] + "/";
		//CLogger::Instance()->TraceInfo("CreatePath: %d\n", sRet);
		//strSftpPath = strSftpPath + param.Param(i) + "/";
		sRet = libssh2_sftp_mkdir(sftp_session, (CONST char *)strSftpPath.c_str(),

			LIBSSH2_SFTP_S_IRWXU|
			LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IXGRP|
			LIBSSH2_SFTP_S_IROTH|LIBSSH2_SFTP_S_IXOTH);

		if(sRet != 0 && sRet != -31)
		{
			//CLogger::Instance()->TraceInfo("libssh2_sftp_mkdir failed: %d\n", sRet);
			return sRet;
		}
	}
	
	return 0;
}

//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  获取登陆sftp的ip、端口、用户、密码
//=========================================================================
void sftp::UpdateInfo(string strIP, int nPort, string strAccount, string strPassword)
{
	// 获取ftp参数
	memcpy(m_ftpInfo.szIP,strIP.c_str(),strIP.length());
	m_ftpInfo.nPort = nPort;
	memcpy(m_ftpInfo.szAccount,strAccount.c_str(),strAccount.length());
	memcpy(m_ftpInfo.szPassword,strPassword.c_str(),strPassword.length());
	//CLogger::Instance()->TraceInfo("-- Get sFtp  IP:%s, szAccount:%s, Password:%s,Port:%d",m_ftpInfo.szIP, m_ftpInfo.szAccount,m_ftpInfo.szPassword,m_ftpInfo.nPort);
	/*
	char szPort[10] = {0};
	::GetPrivateProfileString("FTP", "IP", "", m_ftpInfo.szIP, sizeof(m_ftpInfo.szIP) - 1, FSN_FTP_INI_PATH.c_str());
	::GetPrivateProfileString("FTP", "Account", "", m_ftpInfo.szAccount, sizeof(m_ftpInfo.szAccount) - 1, FSN_FTP_INI_PATH.c_str());
	::GetPrivateProfileString("FTP", "Password", "", m_ftpInfo.szPassword, sizeof(m_ftpInfo.szPassword) - 1, FSN_FTP_INI_PATH.c_str());
	::GetPrivateProfileString("FTP", "Port", "", szPort, sizeof(szPort) - 1, FSN_FTP_INI_PATH.c_str());

	m_ftpInfo.nPort = atoi(szPort);
	
	*/
}

//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  上传文件
//=========================================================================
SHORT sftp::SftpUploadFile(string strLocalFilePath, string strRemFilePath)
{
	FILE *local;
	size_t nread;
	int iRc;
	char *pTr;

	//打开文件
	local = fopen((CONST char*)strLocalFilePath.c_str(), "rb");
	if (!local) {
		//CLogger::Instance()->TraceInfo("Can't open local file %s", strLocalFilePath);
		return 1;
	}

	//获取文件长度
	fseek(local,0, SEEK_END);
	int length = ftell(local);
	fseek(local,0, SEEK_SET);

	//分配内存空间
	char *mem = (char *)malloc(length +1);
	sftp_handle =
		libssh2_sftp_open(sftp_session, (CONST char *)strRemFilePath.c_str(),

		LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
		LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
		LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);

	if (!sftp_handle) {
		//CLogger::Instance()->TraceInfo("Unable to open file with SFTP %s", strRemFilePath.c_str());
		libssh2_session_disconnect(session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(session);
		closesocket(iSock);
		libssh2_exit();
		return 1;
	}
	//CLogger::Instance()->TraceInfo("libssh2_sftp_open() is done, now send data!");

	do {
		nread = fread(mem, 1, sizeof(mem), local);
		if (nread <= 0) {
			break;
		}
		pTr = mem;

		do {
			/* write data in a loop until we block */
			iRc = libssh2_sftp_write(sftp_handle, pTr, nread);
			if(iRc < 0)
				break;
			pTr += iRc;
			nread -= iRc;
		} while (nread);

	} while (iRc > 0);

	//关闭文件
	if (local)
		fclose(local);

	return 0;
}

//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  文件改名
//=========================================================================
SHORT sftp::SftpRename(string strOldRemoteFile,string strNewRemoteFile)
{
	SHORT sRet = 0;
	/*
	//退出释放资源
	SftpExit();

	
	//初始化sftp
	sRet = SftpInit();
	if(sRet != 0)
	{
		//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename SftpInit return %d", sRet);
		return sRet;
	}
	*/
	//修改文件名称
	sRet = libssh2_sftp_rename(sftp_session, (CONST char*)strOldRemoteFile.c_str(), (CONST char *)strNewRemoteFile.c_str());
	//CLogger::Instance()->TraceInfo("- Rename file is %s,%s,%d", strOldRemoteFile.c_str(), strNewRemoteFile.c_str(), sRet);
	if (0 == sRet ||  LIBSSH2_ERROR_EAGAIN == sRet)
	{
		//CLogger::Instance()->TraceInfo("- sftp::SftpRename ::libssh2_sftp_rename return TRUE");
	}
	else
	{
		//CLogger::Instance()->TraceInfo("- sftp::SftpRename ::libssh2_sftp_rename  return %d", sRet);
		return sRet;
	}

	return 0;
}
//=========================================================================
//类名:  sftp
//调用:  
//
//参数:
//		无
//		
//返回:
//	0 -- "OK"
// 
//描述:
//  创建路径、上传文件、更改文件名
//=========================================================================
SHORT sftp::CreateDirUploadRename(string strDirPathName,string strLocalFile, string strRemoteFile,string strOldRemoteFile,string strNewRemoteFile)
{

	//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename strDirPathName = %s", strDirPathName);
	//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename strLocalFile = %s", strLocalFile.c_str());
	//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename strRemoteFile = %s", strRemoteFile);
	//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename strOldRemoteFile = %s", strOldRemoteFile);
	//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename strNewRemoteFile = %s", strNewRemoteFile);

	SHORT sRet = 0;

	//初始化sftp
	sRet = SftpInit();
	if(sRet != 0)
	{
		//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename SftpInit return %d", sRet);
		return sRet;
	}

	//创建路径
	sRet = CreatePath(strDirPathName);
	if (sRet != 0)
	{
		//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename CreatePath return %d", sRet);
		return sRet;
	}


	//上传文件
	sRet = SftpUploadFile(strLocalFile, strRemoteFile);
	if (sRet != 0)
	{
		//CLogger::Instance()->TraceInfo("--sftp::CreateDirUploadRename SftpUploadFile return %d", sRet);
		return sRet;
	}
	else
	{
		//CLogger::Instance()->TraceInfo("- sftp::CreateDirUploadRename SftpUploadFile return TRUE");
	}
	
	//修改文件名称
	sRet = SftpRename(strOldRemoteFile, strNewRemoteFile);
	//CLogger::Instance()->TraceInfo("- Rename file is %s,%s,%d", strOldRemoteFile.c_str(), strNewRemoteFile.c_str(), sRet);
	if (0 != sRet)
	{
		//CLogger::Instance()->TraceInfo("- sftp::CreateDirUploadRename ::SftpRename  return %d", sRet);
		return sRet;
	}
	else
	{
		//CLogger::Instance()->TraceInfo("- sftp::CreateDirUploadRename ::SftpRename return TRUE");
		
	}
	
	//退出释放资源
	SftpExit();

	return 0;
}

BOOL sftp::SftpUploadFiles(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strSftpPath, CStringList &FilesList, CStringList &RenameFilesList)
{
	//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles start============");
	BOOL bRet = FALSE;
	do 
	{
		UpdateInfo(strSftpIp.GetBuffer(0),nSftpPort,strUser.GetBuffer(0),strPassword.GetBuffer(0) );
		//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles strSftpPath = %s",strSftpPath);
		if (strSftpPath.Right(1) != "/")
		{
			strSftpPath += "/";
		}
		//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles strSftpPath = %s",strSftpPath);
		BOOL bFtpUpload = TRUE;
		for (POSITION pos = FilesList.GetHeadPosition(), posRename = RenameFilesList.GetHeadPosition(); pos != NULL ; )
		{
			CString strFile = FilesList.GetNext(pos);
			if ( !strFile.IsEmpty())
			{
				//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles strFile = %s",strFile);
			}
			int nFindPos = strFile.ReverseFind('\\');
			nFindPos = strFile.GetLength() - nFindPos - 1;
			//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles nFindPos = %d",nFindPos);
			CString strRemoteFile = (strSftpPath + strFile.Right(nFindPos));

			//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles strRemoteFile = %s",strRemoteFile);

			CString strRenameFile = RenameFilesList.GetNext(posRename);
			if ( !strRenameFile.IsEmpty())
			{
				//CLogger::Instance()->TraceInfo("- sftp::SftpUploadFiles strRenameFile = %s",strRenameFile);
			}
			if ( 0 != CreateDirUploadRename(strSftpPath.GetBuffer(0),strFile.GetBuffer(0),strRemoteFile.GetBuffer(0), strRemoteFile.GetBuffer(0),strRenameFile.GetBuffer(0)) )
			{
				bFtpUpload = FALSE;
				break;
			}
		}

		if (bFtpUpload)
		{
			bRet = TRUE;
		}


	} while (FALSE);


	return bRet;
}
