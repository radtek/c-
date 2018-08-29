
// ra2yuriEXEDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ra2yuriEXE.h"
#include "ra2yuriEXEDlg.h"
#include "afxdialogex.h"
#include <string>

#include <afx.h>

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

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cra2yuriEXEDlg 对话框




Cra2yuriEXEDlg::Cra2yuriEXEDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cra2yuriEXEDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cra2yuriEXEDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cra2yuriEXEDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &Cra2yuriEXEDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SELECT_FILE, &Cra2yuriEXEDlg::OnBnClickedBtnSelectFile)
	ON_BN_CLICKED(IDC_BTN_SEND, &Cra2yuriEXEDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()


// Cra2yuriEXEDlg 消息处理程序

BOOL Cra2yuriEXEDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cra2yuriEXEDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cra2yuriEXEDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cra2yuriEXEDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void Cra2yuriEXEDlg::OnBnClickedBtnSearch()
{
	//搜索接收电脑

	ACE_SOCK_Stream     stream;
	ACE_SOCK_Connector  connector;
	
	ACE_INET_Addr local_host(7000);
	ACE_INET_Addr remote_host(6000,"127.0.0.1");
	int nConn = connector.connect(stream, remote_host);
	CString str;
	str.Format("%d",nConn);
	MessageBox(str);
	ACE_SOCK_Dgram_Bcast local_broadcast_dgram(local_host);
	int remote_port = 7000; 

	char* message = "Hello World!\n";

	size_t sent_data_length = local_broadcast_dgram.send(message, strlen(message) + 1, remote_port);

	if (sent_data_length == -1) 
	{
		MessageBox("Error in data transmission\n");
	}


	local_broadcast_dgram.close();

	ACE_INET_Addr localHost(6000), remote;
	ACE_SOCK_Dgram_Bcast udp(localHost);



	int nRet = udp.send("123456",6,7000);
	str.Format("%d",nRet);
	MessageBox(str);
	char recvBuf[32] = {0};
	//int recvLen = udp.recv(recvBuf, 32, remote);
	//MessageBox(remote.get_host_name());
	while(1)
	{
		int recvLen = udp.recv(recvBuf, 32, remote);
		
		str.Format("%d",recvLen);
		MessageBox(str);
		MessageBox(remote.get_host_name());
	}
}


void Cra2yuriEXEDlg::OnBnClickedBtnSelectFile()
{
	//选择文件
	int port = 6000;
	ACE_INET_Addr port_to_listen(port);        //绑定的端口
	ACE_SOCK_Acceptor acceptor;
	if (acceptor.open (port_to_listen,1) == -1)     //绑定端口
	{
		MessageBox("!open");
		return;
	}

	ACE_SOCK_Stream peer;        //和客户端的数据通路
	int nRet = acceptor.accept (peer);

	if (nRet != -1)    //建立和客户端的连接
	{
		MessageBox("!accept");
	}
}


void Cra2yuriEXEDlg::OnBnClickedBtnSend()
{
	//发送文件
	//10位总数+4位数字+文件路径名称+10位数字+文件
	//00000000100008E:\1.ini0000000002XX
	ACE_INET_Addr		addr;
	ACE_SOCK_Stream     stream;
	ACE_SOCK_Connector  connector;

	addr.set(6000,"192.168.0.159");
	int nConn = connector.connect(stream, addr);

	if ( nConn == -1 )
	{
		MessageBox("连接服务器不通");
		return;
	}

	if ( !stream.send_n( "123456",6) )
	{
		MessageBox("发送失败");
	}
}
