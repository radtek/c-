#pragma once


// sftp
typedef bool (* FunFtpopenPublicKey)(const char* , int , const char* , const char* , const char* , const char*  );
typedef bool (* FunFtpopenPassword)(const char* , int , const char* , const char* );
typedef bool (* FunFtpclose)();
typedef bool (* FunUploaddir)(const char* ,const char*);
typedef bool (* FunUploadfile)(const char* ,const char*,const char*);
typedef bool (* FunFtprename)(const char* ,const char*);



class sftp : public CWnd
{
	DECLARE_DYNAMIC(sftp)


public:
	sftp();
	virtual ~sftp();
	static sftp * Instance();
	BOOL SftpUploadFiles(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strSftpPath, CStringList &FilesList);
	BOOL SftpRename(CString strSftpIp, INT nSftpPort, CString strUser, CString strPassword, CString strFtpPathName, CString strDesPathName);
	void setPublicAndPrivateKey(CString strPublicKey, CString strPrivateKey);

protected:
	DECLARE_MESSAGE_MAP()
	static	sftp	* m_Instance;
	static	void Destroy();
	bool loadDll();
	void freeDll();

	BOOL m_bLoadDll;
	BOOL m_bUsePublicKey;

	const char * m_szPublicKey;
	const char * m_szPrivateKey;


};


