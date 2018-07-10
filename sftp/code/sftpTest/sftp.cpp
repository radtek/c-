// sftp.cpp : 实现文件
//

#include "stdafx.h"
#include "sftpTest.h"
#include "sftp.h"


// sftp
sftp	* sftp::m_Instance = NULL;

sftp * sftp::Instance()
{
	if(NULL == m_Instance)
	{
		m_Instance = new sftp();
		ASSERT(m_Instance);
		m_Instance->loadDll();
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
	m_bLoadSuccess = false;
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

		ftpopen = (FunFtpopen)::GetProcAddress(m_hinstacne, "ftpopen");
		if (ftpopen == NULL)
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
		m_bLoadSuccess = true;
	} while (0);

	if ( !bRet)
	{
		::FreeLibrary(m_hinstacne);
		m_hinstacne = NULL;
	}

	return bRet;
}

void sftp::freeDll()
{
	::FreeLibrary(m_hinstacne);
}

