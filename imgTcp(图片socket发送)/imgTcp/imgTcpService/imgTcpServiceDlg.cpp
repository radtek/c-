
// imgTcpServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "imgTcpService.h"
#include "imgTcpServiceDlg.h"
#include "afxdialogex.h"
#include <afx.h>

#include <string>

using namespace std;



#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"

#pragma comment(lib, "ACE.lib")
#pragma comment(lib, "Common.lib")

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


// CimgTcpServiceDlg 对话框




CimgTcpServiceDlg::CimgTcpServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CimgTcpServiceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CimgTcpServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CimgTcpServiceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CimgTcpServiceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CimgTcpServiceDlg 消息处理程序

BOOL CimgTcpServiceDlg::OnInitDialog()
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

void CimgTcpServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CimgTcpServiceDlg::OnPaint()
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
HCURSOR CimgTcpServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
/*
void CimgTcpServiceDlg::recv()
{
	ACE_INET_Addr port_to_listen(5000, ACE_LOCALHOST), client_addr;
	ACE_SOCK_Acceptor acceptor;
	ACE_SOCK_Stream client;
	ACE_TCHAR strClientAddr[64], strBuffer[1024];
	ssize_t bytes_received = 0;

	if(acceptor.open(port_to_listen, 1) == -1)
	{
		MessageBox("service 1");
		return;
	}

	while(1)
	{
		if(acceptor.accept(client, &client_addr) == -1)
		{
			acceptor.close();
			MessageBox("service 2");
			break;
		}

		client_addr.addr_to_string(strClientAddr, sizeof(strClientAddr));

		ACE_OS::memset(strBuffer, 0, sizeof(strBuffer));
		if(bytes_received = client.recv(strBuffer, sizeof(strBuffer)) == -1)
		{
			client.close();
			break;
		}

		string strSend = "000050000000000procbank{abc}";
		client.send_n(strSend.c_str(),strSend.length());

		client.close();
	}

	acceptor.close();
}

*/
void CimgTcpServiceDlg::recv()
{
	ACE_INET_Addr port_to_listen(5999);        //绑定的端口
	ACE_SOCK_Acceptor acceptor;
	if (acceptor.open (port_to_listen,1) == -1)     //绑定端口
	{
		MessageBox("bind port fail not return");
		//return;
	}

	while(true)
	{
		ACE_SOCK_Stream peer;        //和客户端的数据通路
		ACE_Time_Value timeout (10, 0);

		if (acceptor.accept (peer) != -1)    //建立和客户端的连接
		{
			//cout<<endl<<endl<<"client connect. "<<endl;
			char buffer[1024];
			ssize_t bytes_received;

			ACE_INET_Addr raddr;
			peer.get_local_addr(raddr);
			//cout<<endl<<"local port\t"<<raddr.get_host_name()<<"\t"<<raddr.get_port_number()<<endl;

			
			bytes_received = peer.recv (buffer,23);
			if (bytes_received != -1)
			{
				continue;
			}

			string strLen = buffer;
			int nJsonLen = atoi( strLen.substr(0,5).c_str() );

			if (nJsonLen <= 0)
			{
				continue;
			}

			char *szJson = new char[nJsonLen];
			bytes_received = peer.recv (szJson,nJsonLen);
			if (bytes_received != -1)
			{
				delete []szJson;
				continue;
			}

			CString strMessage = szJson;
			MessageBox(strMessage);
			delete []szJson;

			int nImgLen = atoi( strLen.substr(5,10).c_str() );

			char *szImg = new char[nImgLen];
			bytes_received = peer.recv (szImg,nImgLen);
			if (bytes_received != -1)
			{
				delete []szImg;
				continue;
			}

			CString writeImg = "e:/recv.bmp";
			CFile file2;
			file2.Open(writeImg,CFile::modeCreate|CFile::modeWrite);
			file2.Write(szImg,nImgLen);
			file2.Flush();
			file2.Close();

			delete []szImg;

			string strSend = "000050000000000procbank{abc}";
			peer.send(strSend.c_str(), strSend.length());    //对客户端发数据
		}
		peer.close ();
		
	}

}




void CimgTcpServiceDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	recv();
}
