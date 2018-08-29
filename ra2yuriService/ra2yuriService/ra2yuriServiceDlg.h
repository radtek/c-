
// ra2yuriServiceDlg.h : ͷ�ļ�
//

#pragma once

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/SOCK_Dgram_Bcast.h"
#include <ace/SOCK_Dgram_Mcast.h>

#include "afxcmn.h"

#pragma comment(lib, "ACE.lib")


#define NUM_DATA_LEN 10
#define NUM_PATH_LEN 4
#define NUM_FILE_LEN 10
#define NUM_RETU_LEN 2

#define MAX_FILE_SIZE 1024*1024


// Cra2yuriServiceDlg �Ի���
class Cra2yuriServiceDlg : public CDialogEx
{
// ����
public:
	Cra2yuriServiceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RA2YURISERVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CStringList m_listIP;
	CStringList m_listFilePath;
	CString m_strHead;

	CString GetLocalPath();

	int nSendChar(CString strSend, CString IP , int port =6000);
	int nSendFile(CString filePath, CString IP , int port = 6000);

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnSelectFile();
	CProgressCtrl m_progress;
};
