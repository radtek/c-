#pragma once


// sftp
typedef bool (* FunFtpopen)(const char* , int , const char* , const char* );
typedef bool (* FunFtpclose)();
typedef bool (* FunUploaddir)(const char* ,const char*);
typedef bool (* FunUploadfile)(const char* ,const char*,const char*);
typedef bool (* FunFtprename)(const char* ,const char*);

HINSTANCE m_hinstacne = NULL;
FunFtpopen		ftpopen		= NULL;
FunFtpclose		ftpclose	= NULL;
FunUploaddir	uploaddir	= NULL;
FunUploadfile	uploadfile	= NULL;
FunFtprename	ftprename	= NULL;

class sftp : public CWnd
{
	DECLARE_DYNAMIC(sftp)


public:
	sftp();
	virtual ~sftp();
	static sftp * Instance();
	bool loadDll();
	void freeDll();
	bool m_bLoadSuccess;

protected:
	DECLARE_MESSAGE_MAP()
	static	sftp	* m_Instance;
	static	void Destroy();
};


