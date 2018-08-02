/*
 * �ļ�����SFTPClient.cpp
 * ˵������SFTP �ͻ���ʵ��
 * ���������ṩ�ļ� �� �ļ��� �ϴ������ع���
 * ����  ��whish(spider�޸�)
 * ����  ��2018.07.11
 */ 
#include "stdafx.h"
#include "SFTPClient.h"
#include <string>
using namespace std;

static GWI_SFTPClient::GWI_SFTPClient_ErrorCode stuSftpError[] = {\
	{-1,"Can't open local file"},\
	{-2,"libssh2 initialization failed"},\
	{-3,"failed to connect!"},\
	{-4,"libssh2_session_init����sessionʧ��"},\
	{-5,"libssh2_session_handshake Failure establishing SSH session"},\
	{-6,"Authentication failed"},\
	{-7,"Unable to init SFTP session"},\
	{-8,"����SFTPĿ¼ʧ��"},\
	{-9,"Unable to open file with SFTP"}\
};
static int iGWI_SFTPClient_ErrorCode_len = sizeof(stuSftpError) / sizeof(stuSftpError[0]);



static const char *keyfile1="D:\\freeSSHd\\publicKey\\user1";
static const char *keyfile2="D:\\freeSSHd\\privateKey\\user1_rsa_1024";
static char gpassword[40]={'\0'};

static unsigned long hostaddr;
static int sock;
static struct sockaddr_in insockaddr;
static int iStatus = -1;
static const char *fingerprint;
static char *userauthlist = NULL;
static LIBSSH2_SESSION *session;


/**
* ��������SFTPDirExist
* ˵��  ���ж�SFTPĿ¼�Ƿ����
* ����  ��1.sftp_session
*           LIBSSH2_SFTP�Ựָ��
*       ��2.sftppath
*           SFTPĿ¼
* ����ֵ��1ָ��Ŀ¼���ڣ�0ָ��Ŀ¼������
*/
int SFTPDirExist(LIBSSH2_SFTP *sftp_session, const char *sftppath)
{
	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_opendir(sftp_session, sftppath);
 
    if(sftp_handle)
	{
		libssh2_sftp_closedir(sftp_handle);
	}
	return sftp_handle>0;
}
/**
* ��������makeSFTPDir
* ˵��  ������SFTPĿ¼
* ����  ��1.sftp_session
*           LIBSSH2_SFTP�Ựָ��
*       ��2.sftppath
*           SFTPĿ¼
* ����ֵ��0�ɹ���-1ʧ��
*/
int makeSFTPDir(LIBSSH2_SFTP *sftp_session, const char *sftppath)
{
	char tmppath[MAXPATH],tmp[MAXPATH];
	const char *seps = "/";
	char *token;
	memset(tmppath,0,MAXPATH);memset(tmp,0,MAXPATH);
	strncpy(tmp,sftppath,MAXPATH-1);
	token = strtok( tmp, seps );
	while( token != NULL )
	{
		sprintf_s(tmppath,MAXPATH,"%s/%s",tmppath,token);
		if(!SFTPDirExist(sftp_session, tmppath))
			if(libssh2_sftp_mkdir(sftp_session, tmppath,0))
				return -1;
		token = strtok( NULL, seps );
	}
	return 0;
}
static void kbd_sign_callback(const char *name, int name_len,
             const char *instruction, int instruction_len, int num_prompts,
             const LIBSSH2_USERAUTH_KBDINT_PROMPT *prompts,
             LIBSSH2_USERAUTH_KBDINT_RESPONSE *responses,
             void **abstract)
{
    int i;
    size_t n;
    char buf[1024];
    (void)abstract;

    fwrite(name, 1, name_len, stdout);
    fwrite(instruction, 1, instruction_len, stdout);

    for (i = 0; i < num_prompts; i++) {
    
        fwrite(prompts[i].text, 1, prompts[i].length, stdout);
        strcpy_s(buf,1024,gpassword);
		//fgets(buf, sizeof(buf), stdin);
        n = strlen(buf);
        while (n > 0 && strchr("\r\n", buf[n - 1]))
          n--;
        buf[n] = 0;
 
        responses[i].text = strdup(buf);
        responses[i].length = n;
        fwrite(responses[i].text, 1, responses[i].length, stdout);
    }
}

void GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(int errorcode,char* errormsg)
{
	if (errormsg == nullptr) return;
	for (int i=0 ;i<iGWI_SFTPClient_ErrorCode_len; i++)
	{
		if (stuSftpError[i].errorcode == errorcode)
		{
			strcpy_s(errormsg,MAX_PATH,stuSftpError[i].errormsg);
			return;
		}
	}
	errormsg = "";
}
/************************************************************************
	 * ��������sftpOpen 
	 * ˵��  ������SFTP����
	 * ����  ��1.ip
	 *           SFTP������IP
	 *       ��2.port
	 *           SFTP�������˿�
	 *       ��3.username
	 *           SFTP�û���
	 *       ��4.password
	 *           SFTP�û�����
	 *		 : 5.publicKey
	 *			 ��Կ
	 *		 : 6.privateKey
	 *			 ˽Կ
	 * ����   �� 0 �ɹ���1 ��������
	************************************************************************/
int GWI_SFTPClient::sftpOpen(const char *ip,int port,const char *username, const char *password,const char *publicKey, const char *privateKey)
{
	int ret = 0;
	int auth_pw = 0;
	int rc;
	iStatus = 0;
	strcpy_s(gpassword,40,password);

#ifdef WIN32
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,0), &wsadata);
#endif

	iStatus |= 1;
	hostaddr = inet_addr(ip);
    rc = libssh2_init (0);
    if (rc != 0) {
        return -2;
    }
	iStatus |= 2;
    /*
     * The application code is responsible for creating the socket
     * and establishing the connection
     */ 
    sock = socket(AF_INET, SOCK_STREAM, 0);
    iStatus |= 4;
    insockaddr.sin_family = AF_INET;
    insockaddr.sin_port = htons(port);
    insockaddr.sin_addr.s_addr = hostaddr;

    if (connect(sock, (struct sockaddr*)(&insockaddr),
                sizeof(struct sockaddr_in)) != 0) {
		libssh2_exit();
        return -3;
    }
    /* Create a session instance
     */ 
    session = libssh2_session_init();

    if(!session)
	{
        return -4;
	}
    /* Since we have set non-blocking, tell libssh2 we are blocking */ 
    libssh2_session_set_blocking(session, 1);

    /* ... start it up. This will trade welcome banners, exchange keys,
     * and setup crypto, compression, and MAC layers
     */ 
   while ((rc = libssh2_session_handshake(session, sock)) == LIBSSH2_ERROR_EAGAIN);
    if (rc) {
        return -5;
    }
	iStatus |= 8;
    /* At this point we havn't yet authenticated.  The first thing to do
     * is check the hostkey's fingerprint against our known hosts Your app
     * may have it hard coded, may go to a file, may present it to the
     * user, that's your call
     */ 
    fingerprint = libssh2_hostkey_hash(session, LIBSSH2_HOSTKEY_HASH_SHA1);
	//for(i = 0; i < 20; i++) {
    //   fprintf(stderr, "%02X ", (unsigned char)fingerprint[i]);
    //}
    //fprintf(stderr, "\n");
 
	/* check what authentication methods are available */ 
    userauthlist = libssh2_userauth_list(session, username, strlen(username));
	if (strstr(userauthlist, "password") != NULL) {
        auth_pw |= 1;
    }
    if (strstr(userauthlist, "keyboard-interactive") != NULL) {
        auth_pw |= 2;
    }
    if (strstr(userauthlist, "publickey") != NULL) {
        auth_pw |= 4;
    }

	if (publicKey != NULL && privateKey != NULL && auth_pw & 4 )
	{
		while ((rc = libssh2_userauth_publickey_fromfile(session, username, keyfile1, keyfile2, password)) == LIBSSH2_ERROR_EAGAIN);
	}
	else if (auth_pw & 1)
	{
		while ((rc = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
	}
	/*
	if(auth_pw & 1)
	{
		while ((rc = libssh2_userauth_password(session, username, password)) == LIBSSH2_ERROR_EAGAIN);
		
	}else if(auth_pw & 2)
	{ 
		while ((rc = libssh2_userauth_keyboard_interactive(session, username, &kbd_sign_callback)) == LIBSSH2_ERROR_EAGAIN);
	}else 
	
	if(auth_pw & 4)
	{ 
		while ((rc = libssh2_userauth_publickey_fromfile(session, username, keyfile1, keyfile2, password)) == LIBSSH2_ERROR_EAGAIN);
	}
	*/
	if (rc) {
        ret = -6;
    }
	return ret;
}

/************************************************************************
	 * ������ �� sftpClose
	 * ˵��   �� �ͷ���Դ
	 * ����   �� ��
	 * ����   �� 0 �ɹ���1 ��������
	/************************************************************************/

int GWI_SFTPClient::sftpClose()
{
	if (iStatus&8)
	{
		libssh2_session_disconnect(session, "Normal Shutdown");
		libssh2_session_free(session);
	}

	if (iStatus&4)
	{
#ifdef WIN32
		closesocket(sock);
#else
		close(sock);
#endif
	}
	
	if (iStatus&2)
	{
		libssh2_exit();
	}

	if (iStatus&1)
	{
#ifdef WIN32
		WSACleanup( );
#endif
	}
	iStatus = 0;
	return 0;
}
/**
* ��������uploadFile
* ˵��  ���ϴ��ļ�
* ���� 
*       ��1.sftppath
*           SFTP·��
*       ��2.slocalfile
*           Ҫ�ϴ��ı����ļ�ȫ·���������ļ���
*       ��3.sftpfilename
*           �ϴ���SFTP������������ļ�������Ϊ�գ����Ϊ�գ�
*           �������˱�����ļ�������slocalfile�е��ļ�������һ��
* ����ֵ��0�ɹ�������ʧ�ܣ���
*/
int GWI_SFTPClient::uploadFile(
			   const char *sftppath,
			   const char *slocalfile,
			   const char *sftpfilename/* = NULL*/)
{
	int ret = 0;
	int auth_pw = 0;
    int rc;
    LIBSSH2_SFTP *sftp_session;
    LIBSSH2_SFTP_HANDLE *sftp_handle;

	char sftp_filefullpath[MAX_PATH];
	FILE *hlocalfile;
    char mem[1024*10];
    size_t nread;
    char *ptr;

	hlocalfile = fopen(slocalfile, "rb");
    if (!hlocalfile) {
        return -1;
    }

	sftp_session = libssh2_sftp_init(session);
	if (!sftp_session) {
		return -7;
	}
		 
	/* �ϴ��ļ� */
	rc = 0;
	if(!SFTPDirExist(sftp_session, sftppath))
		rc = makeSFTPDir(sftp_session, sftppath);
	if(rc != 0){
		return -8;
	}
	/* FTP�ļ�ȫ·���������ļ��� */
	sprintf_s(sftp_filefullpath,MAX_PATH,
		sftppath[strlen(sftppath)-1]=='/'?"%s%s":"%s/%s",
		sftppath,
		sftpfilename==NULL?PathFindFileNameA(slocalfile):sftpfilename);

	sftp_handle = libssh2_sftp_open(sftp_session, sftp_filefullpath,
						LIBSSH2_FXF_WRITE|LIBSSH2_FXF_CREAT|LIBSSH2_FXF_TRUNC,
						LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
						LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
			 
	if (!sftp_handle) {
		return -9;
	}
	do {
		nread = fread(mem, 1, sizeof(mem), hlocalfile);
		if (nread <= 0) {
			/* end of file */ 
			break;
		}
		ptr = mem;
				 
		do {
			/* write data in a loop until we block */ 
			rc = libssh2_sftp_write(sftp_handle, ptr, nread);
			if(rc < 0)
				break;
			ptr += rc;
			nread -= rc;
		} while (nread);
				 
	} while (rc > 0);
	fclose(hlocalfile);
	libssh2_sftp_close(sftp_handle);
	libssh2_sftp_shutdown(sftp_session);
	return 0;
}

/**
* ��������downloadFile
* ˵��  �������ļ�
* ���� 
*       ��1.sftpfile
*           Ҫ���ص�SFTP�ļ�ȫ·���������ļ���
*       ��2.slocalfile
*           ���浽���ص��ļ�ȫ·�������ļ���
* ����ֵ��0�ɹ�������ʧ�ܣ���
*/
int GWI_SFTPClient::downloadFile(
			   const char *sftpfile,
			   const char *slocalfile)
{
	int ret = 0;
    int auth_pw = 0;
    int rc;
    LIBSSH2_SFTP *sftp_session;
    LIBSSH2_SFTP_HANDLE *sftp_handle;
	FILE *hlocalfile;
    char mem[1024*10];
 
	sftp_session = libssh2_sftp_init(session);
	if (!sftp_session) {
		return -7;
	}
	/* ��SFTP�ļ� */
	sftp_handle = libssh2_sftp_open(sftp_session, sftpfile, LIBSSH2_FXF_READ,
							LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
							LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
	if (!sftp_handle) {
		libssh2_sftp_shutdown(sftp_session);
		return -9;
	}
	/* ��API����ֱ���ʹ��ļ�����·�� */
	MakeSureDirectoryPathExists(slocalfile);
	hlocalfile = fopen(slocalfile, "wb+");
	if (!hlocalfile) {
		libssh2_sftp_shutdown(sftp_session);
		return -1;
	}
	do {
		rc = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));	 
		if (rc > 0) {
			fwrite(mem, 1, rc, hlocalfile);
		} else {
			break;
		}
	} while (1);
	if (hlocalfile)
		fclose(hlocalfile);
	libssh2_sftp_close(sftp_handle);
	libssh2_sftp_shutdown(sftp_session);
	return 0;
}
/**
* ��������getFileList
* ˵��  ����ȡ�ļ��б�
* ����  
*       ��1.sftpdir
*           Ҫ��ȡ�ļ��б�ĵ�SFTP·��
*       ��2.ext
*           �ļ�����չ������Ϊ"",��ʾ��ȡ�����ļ�
*       ��3.retlist
*           �Ը�ʽ��"['filename1','filename2',...]"�����ļ��б�
* ����ֵ��0�ɹ�������ʧ�ܣ���
*/
int GWI_SFTPClient::getFileList(
			   const char *sftpdir,
			   const char *ext,
			   /*char * retlist*/
			   CString &retlist)
{
	int ret = 0;
    int auth_pw = 0;
    int rc;
    LIBSSH2_SFTP *sftp_session;
    LIBSSH2_SFTP_HANDLE *sftp_handle;
	char tmpbuffer[512];
	char longentry[512];
	LIBSSH2_SFTP_ATTRIBUTES attrs;
	retlist.Empty();
		
	sftp_session = libssh2_sftp_init(session);

	if (!sftp_session) {
		return -7;
	}
	/* ��SFTPĿ¼ */
	sftp_handle = libssh2_sftp_opendir(sftp_session, sftpdir); 
	if (!sftp_handle) {
		return -8;
	}
				//strcat(retlist,"[");
	retlist.AppendChar('[');
	do {
		wchar_t tmp[256];
		memset(&attrs,0,sizeof(LIBSSH2_SFTP_ATTRIBUTES));
		memset(tmpbuffer,0,sizeof(tmpbuffer));
		memset(longentry,0,sizeof(longentry));
		rc = libssh2_sftp_readdir_ex(sftp_handle, tmpbuffer, sizeof(tmpbuffer),longentry, sizeof(longentry), &attrs);
		if(rc > 0) {
			// ���˵���ǰĿ¼����һ��Ŀ¼��
			if(strcmp(tmpbuffer,".")==0||strcmp(tmpbuffer,"..")==0)
				continue;
			if(ext&& strlen(ext)>0)
			{
				// �ļ����͹���
				char *pos = strstr(tmpbuffer,ext);
				if(!pos)
					continue;
				if(strcmp(pos,ext)!=0)
					continue;
			}
			MultiByteToWideChar(CP_ACP,0,tmpbuffer,strlen(tmpbuffer)+1,tmp,sizeof(tmp)/sizeof(tmp[0]));
			retlist.Append(tmp);
			retlist.AppendChar(',');
			//strcat(retlist,tmp);
			//strcat(retlist,",");
		} 
		else
			break;
	} while (1);
	if(retlist.ReverseFind(',')==retlist.GetLength()-1)
		retlist.SetAt(retlist.GetLength()-1,'\0');
	retlist.AppendChar(']');
	//if(retlist[strlen(retlist)-1]==',')
	//   retlist[strlen(retlist)-1]='\0';
	//strcat(retlist,"]");
	libssh2_sftp_closedir(sftp_handle);
	libssh2_sftp_shutdown(sftp_session);
		
	return ret;
}

static int preSftpDownloadFileDir(const char sftpDir[MAX_PATH],const char localDir[MAX_PATH],const char extendDir[MAX_PATH])
{
	int rc = -1;
	int iSt = 0;
	LIBSSH2_SFTP *sftp_session;
	LIBSSH2_SFTP_HANDLE *sftp_handle;
	char tmpbuffer[512];
	char longentry[512];
	LIBSSH2_SFTP_ATTRIBUTES attrs;

	char csCreateLocalDir[MAX_PATH] = {};
	sprintf_s(csCreateLocalDir,MAX_PATH,"%s%s",localDir,extendDir);
	CreateDirectoryA(csCreateLocalDir,NULL);
	sftp_session = libssh2_sftp_init(session);

	if (!sftp_session) {
		return -7;
	}
	/* ��SFTPĿ¼ */
	sftp_handle = libssh2_sftp_opendir(sftp_session, sftpDir); 
	if (!sftp_handle) {
		libssh2_sftp_shutdown(sftp_session);
		return -8;
	}

	do {
		memset(&attrs,0,sizeof(LIBSSH2_SFTP_ATTRIBUTES));
		memset(tmpbuffer,0,sizeof(tmpbuffer));
		memset(longentry,0,sizeof(longentry));
		rc = libssh2_sftp_readdir_ex(sftp_handle, tmpbuffer, sizeof(tmpbuffer),longentry, sizeof(longentry), &attrs);
		if(rc > 0) {
			// ���˵���ǰĿ¼����һ��Ŀ¼��
			if(strcmp(tmpbuffer,".")==0||strcmp(tmpbuffer,"..")==0)
				continue;
		    //add �ж����ļ��� �����ļ� ͨ��ĩβ��'.' �� �Ƿ����¼�Ŀ¼ 
			char* pcTmp = strrchr(tmpbuffer,'.');
			if (!pcTmp)//���Ǵ���׺���ļ�
			{
				char sftpDirAdd[MAX_PATH] = {};
				sprintf_s(sftpDirAdd,MAX_PATH,"%s/%s",sftpDir,tmpbuffer);
				LIBSSH2_SFTP_HANDLE* sftp_handle = libssh2_sftp_open(sftp_session, sftpDirAdd, LIBSSH2_FXF_READ,
					LIBSSH2_SFTP_S_IRUSR|LIBSSH2_SFTP_S_IWUSR|
					LIBSSH2_SFTP_S_IRGRP|LIBSSH2_SFTP_S_IROTH);
				if (sftp_handle) { //û�к�׺���ļ�
					libssh2_sftp_close(sftp_handle);
					char localDirAdd[MAX_PATH] = {};
					sprintf_s(localDirAdd,MAX_PATH,"%s%s\\\\%s",localDir,extendDir,tmpbuffer);
					iSt = GWI_SFTPClient::downloadFile(sftpDirAdd,localDirAdd);			
				}
				else{  //�ļ���
					char externDirAdd[MAX_PATH] = {};
					sprintf_s(externDirAdd,"%s\\\\%s",extendDir,tmpbuffer);
					iSt = preSftpDownloadFileDir(sftpDirAdd,localDir,externDirAdd);
				}
			}
			else //����׺���ļ�
			{
				char SftpFilePath[MAX_PATH] = {};
				sprintf_s(SftpFilePath,MAX_PATH,"%s/%s",sftpDir,tmpbuffer);
				char localDirAdd[MAX_PATH] = {};
				sprintf_s(localDirAdd,MAX_PATH,"%s%s\\\\%s",localDir,extendDir,tmpbuffer);
				iSt = GWI_SFTPClient::downloadFile(SftpFilePath,localDirAdd);
			}
			if (iSt != 0) //�ݹ���� ֱ�ӽ����ϲ����
			{
				break;
			}
		} 
		else
			break;
	
	} while (1);
	//�ͷ���Դ
	libssh2_sftp_closedir(sftp_handle);
	libssh2_sftp_shutdown(sftp_session);
	return iSt;
}
/************************************************************************
��������sftpdDwnloadFileDir
˵����ʵ��SFTP�ļ������ص����أ��������ļ�    
	���������� 
const char *sftpdir �� �ϴ���SFTP���ļ�����
const char *slocaldir�������ļ�����
	        ���
��
���أ�0 �ɹ�  ����ʧ��
************************************************************************/
int GWI_SFTPClient::sftpDwnloadFileDir(const char *sftpdir, const char *slocaldir)
{
	return preSftpDownloadFileDir(sftpdir,slocaldir,"");
}

static int preSftpUpdateFileDir(const char localDir[MAX_PATH],const char sftpDir[MAX_PATH],const char externDir[MAX_PATH])      
{  
	WIN32_FIND_DATAA FindFileData;  
	HANDLE hFind=INVALID_HANDLE_VALUE;  
	char DirSpec[MAX_PATH];                  //����Ҫ�������ļ��е�Ŀ¼  
	sprintf_s(DirSpec,MAX_PATH,"%s\\*",localDir);

	hFind=FindFirstFileA(DirSpec,&FindFileData);          //�ҵ��ļ����еĵ�һ���ļ�  
	bool ret = 0;
	if(hFind==INVALID_HANDLE_VALUE)                               //���hFind�������ʧ�ܣ����������Ϣ  
	{  
		return -1;    
	}  

	char csSftpFilePath[MAX_PATH] = {};
	sprintf_s(csSftpFilePath,MAX_PATH,"%s%s",sftpDir,externDir);
	LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
	if(!SFTPDirExist(sftp_session, csSftpFilePath))
		ret = makeSFTPDir(sftp_session, csSftpFilePath);
	libssh2_sftp_shutdown(sftp_session);
	if(ret != 0){
		return -8;
	}
	else   
	{   
		while(FindNextFileA(hFind,&FindFileData)!=0)                            //���ļ������ļ��д���ʱ  
		{  
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0&&strcmp(FindFileData.cFileName,".")==0||strcmp(FindFileData.cFileName,"..")==0)        //�ж����ļ���&&��ʾΪ"."||��ʾΪ"."  
			{  
				continue;  
			}  
			char DirAdd[MAX_PATH];  
			sprintf_s(DirAdd,MAX_PATH,"%s\\%s",localDir,FindFileData.cFileName);
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0)      //�ж�������ļ���  
			{  
				//ƴ�ӵõ����ļ��е�����·��  
				char ExternDirAdd[MAX_PATH];
				sprintf_s(ExternDirAdd,MAX_PATH,"%s/%s",externDir,FindFileData.cFileName);
				ret = preSftpUpdateFileDir(DirAdd,sftpDir,ExternDirAdd);                                  //ʵ�ֵݹ����  
				if(ret)
				{
					FindClose(hFind);  
					return ret;
				}
			}  
			if((FindFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)==0)    //��������ļ���  
			{  
				//wcout<<Dir<<"\\"<<FindFileData.cFileName<<endl;            //�������·��  
				char sftpDirAdd[MAX_PATH] = {};
				sprintf_s(sftpDirAdd,MAX_PATH,"%s%s",sftpDir,externDir);
				
				char csLocalDirAdd[MAX_PATH] = {};
				sprintf_s(csLocalDirAdd,MAX_PATH,"%s\\%s",localDir,FindFileData.cFileName);

				ret = GWI_SFTPClient::uploadFile(sftpDirAdd,csLocalDirAdd);
				if(ret)
				{
					FindClose(hFind);  
					return ret;
				}
			}  
		}  
		
		FindClose(hFind);  
	} 
	return ret;
} 

/************************************************************************/
/* ��������sftpupdateFileDir
/* ˵����ʵ�ֱ����ļ����ϴ���FTP���������ļ�    
/* ���������� 
const char *sftpdir �� �ϴ���SFTP���ļ�����
const char *slocaldir�������ļ�����
	        ���
��
/* ���أ�0 �ɹ�  ����ʧ��
/************************************************************************/
int GWI_SFTPClient::sftpUpdateFileDir(const char *sftpdir,const char *slocaldir)
{
	return preSftpUpdateFileDir(slocaldir,sftpdir,"");
}

/************************************************************************/
/* ������ sftpCd
/* ˵���� ����sftp�ļ���
/* ������ ����
const char* path���ļ���·��
/************************************************************************/
int GWI_SFTPClient::sftpCd(const char* path)
{
	int ret = 0;
	LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
	if(!SFTPDirExist(sftp_session, path))
		ret = makeSFTPDir(sftp_session, path);
	libssh2_sftp_shutdown(sftp_session);
	if (ret)
	{
		return -8;
	}
	return ret;
}

/************************************************************************/
	/* ������ sftpRename
	/* ˵���� �������ļ�
	/* ������ ����
	const char* sftpfilename��ftp�ļ�·��
	const char* newfilename:������������
	/************************************************************************/
int GWI_SFTPClient::sftpRename( const char* sftpfilename, const char* newfilename)
{
	int ret = 0;
	LIBSSH2_SFTP* sftp_session = libssh2_sftp_init(session);
	ret = libssh2_sftp_rename(sftp_session,sftpfilename,newfilename);
	libssh2_sftp_shutdown(sftp_session);
	if (ret)
	{
		return -8;
	}
	return ret;
	
}