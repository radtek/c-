// sftp.cpp : 实现文件
//

#include "stdAfx.h"
#include "sftp.h"
#include "Cashway_ATMC.h"


// sftp
sftp	* sftp::m_Instance = NULL;

HINSTANCE m_hinstacne = NULL;
FunFtpopenPublicKey		ftpopenPublicKey	= NULL;
FunFtpopenPassword		ftpopenPassword		= NULL;
FunFtpclose				ftpclose			= NULL;
FunUploaddir			uploaddir			= NULL;
FunUploadfile			uploadfile			= NULL;
FunFtprename			ftprename			= NULL;

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
	m_Instance->freeDll();
	delete m_Instance;
	m_Instance = NULL;
	
}

IMPLEMENT_DYNAMIC(sftp, CWnd)

sftp::sftp()
{
	m_bLoadDll = FALSE;
	m_bUsePublicKey = FALSE;
}

sftp::~sftp()
{
}


BEGIN_MESSAGE_MAP(sftp, CWnd)
END_MESSAGE_MAP()



// sftp 消息处理程序

bool sftp::loadDll()
{
	bool bRet = false;
	do 
	{

		m_hinstacne = ::LoadLibrary("sftp.dll");
		if (m_hinstacne == NULL) 
			break;

		ftpopenPassword = (FunFtpopenPassword)::GetProcAddress(m_hinstacne, "ftpopenPassword");
		if (ftpopenPassword == NULL)
		{
			break;
		}

		ftpopenPublicKey = (FunFtpopenPublicKey)::GetProcAddress(m_hinstacne, "ftpopenPublicKey");
		if (ftpopenPublicKey == NULL)
		{
			break;
		}

		ftpclose = (FunFtpclose)::GetProcAddress(m_hinstacne, "ftpclose");
		if (ftpclose == NULL)
		{
			break;
		}

		uploaddir = (FunUploaddir)::GetProcAddress(m_hinstacne, "uploaddir");
		if (uploaddir == NULL)
		{
			break;
		}

		uploadfile = (FunUploadfile)::GetProcAddress(m_hinstacne, "uploadfile");
		if (uploadfile == NULL)
		{
			break;
		}
		ftprename = (FunFtprename)::GetProcAddress(m_hinstacne, "ftprename");
		if (ftprename == NULL)
		{
			break;
		}

		bRet = true;

	} while (0);

	if ( !bRet)
	{
		freeDll();
	}

	return bRet;
}

void sftp::freeDll()
{
	::FreeLibrary(m_hinstacne);
	m_hinstacne = NULL;
}

void sftp::setPublicAndPrivateKey(CString strPublicKey, CString strPrivateKey)
{
	m_szPublicKey = strPublicKey;
	m_szPrivateKey = strPrivateKey;

	m_bUsePublicKey = FALSE;
	if ( strlen(m_szPublicKey) != 0 && strlen(m_szPrivateKey) != 0 )
	{
		m_bUsePublicKey = TRUE;
	}
}

BOOL sftp::SftpUploadFiles(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strSftpPath, CStringList &FilesList)
{

	BOOL bRet = FALSE;
	do 
	{
		if (!m_bLoadDll)
		{
			if (!loadDll())
			{
				break;
			}
			m_bLoadDll = TRUE;
		}

		if (m_bUsePublicKey)
		{
			if (!ftpopenPublicKey(strSftpIp,nSftpPort,strUser,strPassword,m_szPublicKey,m_szPrivateKey))
			{
				break;
			}
		}
		else
		{
			if (!ftpopenPassword(strSftpIp,nSftpPort,strUser,strPassword))
			{
				break;
			}
		}
		

		BOOL bFtpUpload = TRUE;
		for (POSITION pos = FilesList.GetHeadPosition(); pos != NULL; )
		{
			CString strFile = FilesList.GetNext(pos);
			if (!uploadfile(strSftpPath,strFile,NULL) )
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

	ftpclose();
	
	return bRet;
}

BOOL sftp::SftpRename(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strFtpPathName, CString strDesPathName)
{
	BOOL bRet = FALSE;
	do 
	{
		if (m_bUsePublicKey)
		{
			if (!ftpopenPublicKey(strSftpIp,nSftpPort,strUser,strPassword,m_szPublicKey,m_szPrivateKey))
			{
				break;
			}
		}
		else
		{
			if (!ftpopenPassword(strSftpIp,nSftpPort,strUser,strPassword))
			{
				break;
			}
		}

		if ( !ftprename(strFtpPathName,strDesPathName) )
		{
			break;
		}
		
		bRet = TRUE;

	} while (FALSE);

	ftpclose();
	
	return bRet;
}