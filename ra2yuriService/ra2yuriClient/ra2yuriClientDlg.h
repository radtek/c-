
// ra2yuriClientDlg.h : 头文件
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

#pragma comment(lib, "ACE.lib")

#define NUM_DATA_LEN 10
#define NUM_TYPE_LEN 4
#define NUM_PATH_LEN 4
#define NUM_FILE_LEN 10
#define NUM_RETU_LEN 2

#define MAX_FILE_SIZE 1024*1024

// Cra2yuriClientDlg 对话框
class Cra2yuriClientDlg : public CDialogEx
{
// 构造
public:
	Cra2yuriClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RA2YURICLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CString m_strHead;

	ACE_INET_Addr m_port_to_listen; 
	ACE_SOCK_Acceptor m_acceptor;

	bool m_bThreadUdp;
	HANDLE m_handleThreadUdp;
	unsigned int m_dwThreadUdpId ;

	bool bStartUdp();
	static UINT _stdcall ThreadFuncUdp( LPVOID lpPara );
	void Udp();

	bool m_bThreadTcp;
	HANDLE m_handleThreadTcp;
	unsigned int m_dwThreadTcpId ;

	bool bStartTcp();
	static UINT _stdcall ThreadFuncTcp( LPVOID lpPara );
	void Tcp();

	void run();
	CString RecvFile(ACE_SOCK_Stream &peer);
	CString RecvChar(ACE_SOCK_Stream &peer, CString &strRecv);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
