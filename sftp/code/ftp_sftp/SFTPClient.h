#ifndef _SFTPCLINET_GWI_
#define _SFTPCLINET_GWI_

#include ".\libssh2\include\libssh2_config.h"
#include ".\libssh2\include\libssh2.h"
#include ".\libssh2\include\libssh2_sftp.h"

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
#include <Shlwapi.h>

#include <imagehlp.h>
#pragma comment( lib, "imagehlp.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\libssh2.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\libeay32.lib" )
#pragma comment( lib, ".\\libssh2\\lib\\zlib.lib" )
#ifndef MAXPATH 
#define MAXPATH 256
#endif



	/**
	* ��������SFTPDirExist
	* ˵��  ���ж�SFTPĿ¼�Ƿ����
	* ����  ��1.sftp_session
	*           LIBSSH2_SFTP�Ựָ��
	*       ��2.sftppath
	*           SFTPĿ¼
	* ����ֵ��1ָ��Ŀ¼���ڣ�0ָ��Ŀ¼������
	*/
	int SFTPDirExist(LIBSSH2_SFTP *sftp_session, const char *sftppath);
	/**
	* ��������makeSFTPDir
	* ˵��  ������SFTPĿ¼
	* ����  ��1.sftp_session
	*           LIBSSH2_SFTP�Ựָ��
	*       ��2.sftppath
	*           SFTPĿ¼
	* ����ֵ��0�ɹ���-1ʧ��
	*/
	int makeSFTPDir(LIBSSH2_SFTP *sftp_session, const char *sftppath);
namespace GWI_SFTPClient{

	/************************************************************************/
	/* ʵ�ָ��ݴ�����õ�SFTP�Ĵ�����Ϣ                                                                     */
	/************************************************************************/
	typedef struct _GWI_SFTPClient_ErrorCode{
		int errorcode;
		char* errormsg;
	}GWI_SFTPClient_ErrorCode,*pGWI_SFTPClient_ErrorCode;

	void getGWI_SFTPClient_ErrorMsg_By_Errorcode(int errorcode,char* errormsg);
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
	 * ����   �� 0 �ɹ���1 ��������
	/************************************************************************/
	int sftpOpen(const char *ip,int port,const char *username, const char *password);

	/************************************************************************
	 * ������ �� sftpClose
	 * ˵��   �� �ͷ���Դ
	 * ����   �� ��
	 * ����   �� 0 �ɹ���1 ��������
	/************************************************************************/
	int sftpClose();

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
	int uploadFile(const char *sftppath,
				   const char *slocalfile,
				   const char *sftpfilename = NULL);
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
	int downloadFile(
				   const char *sftpfile,
				   const char *slocalfile);

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
	int getFileList(
				   const char *sftpdir,
				   const char *ext,
				   /*char * retlist*/
				   CString &retlist);

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
	int sftpUpdateFileDir(
		const char *sftpdir,
		const char *slocaldir);

	/************************************************************************/
	/* ��������downloadFileDir
	/* ˵����ʵ��SFTP�ļ������ص����أ��������ļ�    
	/* ���������� 
	const char *sftpdir �� �ϴ���SFTP���ļ�����
	const char *slocaldir�������ļ�����
	         ���
	��
	/* ���أ�0 �ɹ�  ����ʧ��
	/************************************************************************/
	int sftpDwnloadFileDir(
		const char *sftpdir,
		const char *slocaldir);
	/************************************************************************/
	/* ������ sftpCd
	/* ˵���� ����sftp�ļ���
	/* ������ ����
	const char* path���ļ���·��
	/************************************************************************/
	int sftpCd(const char* path);
}
#endif //#ifndef _SFTPCLINET_GWI_
