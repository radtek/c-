
// ra2yuriClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ra2yuriClient.h"
#include "ra2yuriClientDlg.h"
#include "afxdialogex.h"


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


// Cra2yuriClientDlg 对话框




Cra2yuriClientDlg::Cra2yuriClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cra2yuriClientDlg::IDD, pParent)
{
	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void Cra2yuriClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cra2yuriClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Cra2yuriClientDlg 消息处理程序

BOOL Cra2yuriClientDlg::OnInitDialog()
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

	m_strHead = "ra2yuri";
	run();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cra2yuriClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cra2yuriClientDlg::OnPaint()
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
HCURSOR Cra2yuriClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


bool Cra2yuriClientDlg::bStartUdp()
{
	m_bThreadUdp = true;
	m_handleThreadUdp = (HANDLE)::_beginthreadex(0, 0, ThreadFuncUdp, (void*)this, 0, & m_dwThreadUdpId );
	if (m_handleThreadUdp == INVALID_HANDLE_VALUE)
	{
		m_bThreadUdp = false;
		return false;
	}
	return true ;
}

UINT _stdcall Cra2yuriClientDlg::ThreadFuncUdp( LPVOID	lpPara ) 
{
	try
	{
		Cra2yuriClientDlg *pHandle = ( Cra2yuriClientDlg * ) lpPara ;

		while( pHandle->m_bThreadUdp )
		{
			pHandle->Udp();
			//Sleep(30);

		}
	}
	catch(...)
	{

	}
	return 0 ;
}

//只返回一个ip地址
bool GetLocalIP(char* ip)  
{  
	//1.初始化wsa  
	WSADATA wsaData;  
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);  
	if (ret!=0)  
	{  
		return false;  
	}  
	//2.获取主机名  
	char hostname[256];  
	ret=gethostname(hostname,sizeof(hostname));  
	if (ret==SOCKET_ERROR)  
	{  
		return false;  
	}  
	//3.获取主机ip  
	HOSTENT* host=gethostbyname(hostname);  
	if (host==NULL)  
	{  
		return false;  
	}  
	//4.转化为char*并拷贝返回  
	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));  
	return true;  
} 

void Cra2yuriClientDlg::Udp()
{
	ACE_INET_Addr localHost(6000), remote;
	ACE_SOCK_Dgram_Bcast udp(localHost);

	do 
	{
		char szRecvBuf[32] = {0};
		int recvLen = udp.recv(szRecvBuf, 32, remote);
		if (recvLen < 0)
		{
			break;
		}
		
		CString strRecvData(szRecvBuf);
		if (strRecvData != m_strHead)
		{
			break;
		}

		CString strSend = m_strHead + "00";
		int nSend = udp.send(strSend,strSend.GetLength(),5000);
		if (nSend < 0)
		{
			break;
		}

	} while (0);
	
	udp.close();
}


bool Cra2yuriClientDlg::bStartTcp()
{
	int port = 6000;       //绑定的端口
	m_port_to_listen.set_port_number(port);
	if (m_acceptor.open (m_port_to_listen,1) == -1)     //绑定端口
	{
		return false;
	}
	
	m_bThreadTcp = true;
	m_handleThreadTcp = (HANDLE)::_beginthreadex(0, 0, ThreadFuncTcp, (void*)this, 0, & m_dwThreadTcpId );
	if (m_handleThreadTcp == INVALID_HANDLE_VALUE)
	{
		m_bThreadTcp = false;
		return false;
	}

	return true ;
}

UINT _stdcall Cra2yuriClientDlg::ThreadFuncTcp( LPVOID	lpPara ) 
{
	try
	{
		Cra2yuriClientDlg *pHandle = ( Cra2yuriClientDlg * ) lpPara ;
		
		while( pHandle->m_bThreadTcp )
		{
			pHandle->Tcp();
			//Sleep(30);

		}
		
		
	}
	catch(...)
	{

	}
	return 0 ;
}

void Cra2yuriClientDlg::Tcp()
{
	
	ACE_SOCK_Stream peer;        //和客户端的数据通路

	CString strRet = "99";
	CString strSend = m_strHead;

	do
	{
		
		if (-1 == m_acceptor.accept (peer))
		{
			strRet = "01";
			break;
		}

		//头
		char szHead[16] = {0};
		int nRecvLen = peer.recv_n (szHead,m_strHead.GetLength() );
		if (0 >= nRecvLen)
		{
			strRet = "02";
			break;
		} 

		CString strHead(szHead);
		if (strHead != m_strHead)
		{
			strRet = "03";
			break;
		}

		//MessageBox("头:"+strHead);

		//数据总长
		char szDataLen[16] = {0};
		nRecvLen = peer.recv_n (szDataLen,NUM_DATA_LEN );
		if (0 >= nRecvLen)
		{
			strRet = "04";
			break;
		} 
		//MessageBox("总长:"+CString(szDataLen));
		int nDataLen = atoi( szDataLen );
		if (0 >= nDataLen)
		{
			strRet = "05";
			break;
		}

		//类型
		char szType[16] = {0};
		nRecvLen = peer.recv_n (szType,NUM_TYPE_LEN );
		if (0 >= nRecvLen)
		{
			strRet = "06";
			break;
		} 

		//MessageBox("类型:"+CString(szType));
		CString strType = CString(szType).MakeUpper();
		if (strType == "FILE")
		{
			
			strRet = RecvFile(peer);
			if (strRet != "00")
			{
				break;
			}
			
		}
		else if(strType == "ISME")
		{
			/*
			CString strChar;
			strRet = RecvChar(peer,strChar);
			if (strRet != "00")
			{
				break;
			}
			*/
		}
		else
		{
			strRet = "07";
			break;
		}

		
		
		/*
		char *szRecvData = new char[nDataLen+1];
		char *szRecvP = szRecvData; //szRecvP保留原指针等以后delete
		szRecvData[nDataLen] = '\0';
		bytes_received = peer.recv_n (szRecvData,nDataLen);
		if (0 >= bytes_received)
		{
			delete []szRecvData;
			strRet = "04";
			break;
		}

		char szPathLen[16] = {0};
		memcpy(szPathLen,szRecvData,NUM_PATH_LEN);
		szRecvData = szRecvData + NUM_PATH_LEN;

		//MessageBox("路径长:" + CString(szPathLen) );

		int nPathLen = atoi( szPathLen );
		char szFilePath[MAX_PATH] = {0};
		memcpy(szFilePath,szRecvData,nPathLen);
		szRecvData = szRecvData + nPathLen;

		CString strFilePath(szFilePath);

		//MessageBox("路径:" + strFilePath );

		char szFileLen[16] = {0};
		memcpy(szFileLen,szRecvData,NUM_FILE_LEN);
		szRecvData = szRecvData + NUM_FILE_LEN;

		//MessageBox("文件长:" + CString(szFileLen) );
		int nFileLen = atoi( szFileLen );
		*/
		
		
		strRet = "00";

		
		

	}while(0);

	strSend += strRet;
	peer.send_n(strSend, strSend.GetLength());    //对客户端发数据

	peer.close ();
}

CString Cra2yuriClientDlg::RecvFile(ACE_SOCK_Stream &peer)
{
	CString strRet = "98";
	do 
	{
		//路经长
		char szFilePathLen[16] = {0};
		int nRecvLen = peer.recv_n (szFilePathLen,NUM_PATH_LEN );
		if (0 >= nRecvLen)
		{
			strRet = "08";
			break;
		} 
		//MessageBox("路径长:"+CString(szFilePathLen));
		int nFilePathLen = atoi( szFilePathLen );
		if (0 >= nFilePathLen)
		{
			strRet = "09";
			break;
		}

		//路经
		char szFilePath[MAX_PATH] = {0};
		nRecvLen = peer.recv_n (szFilePath,nFilePathLen);
		if (0 >= nRecvLen)
		{
			strRet = "10";
			break;
		} 

		CString strFilePath(szFilePath);

		//MessageBox("路径:"+strFilePath);
		///////测试
		//int n = strFilePath.Find("\\");
		//strFilePath = strFilePath.Left(n) + "\\1" + strFilePath.Mid(n);
		///////

		CString strFilePathTemp = strFilePath;

		int nPos = strFilePathTemp.Find("\\");
		CString path = strFilePathTemp.Left(nPos);
		strFilePathTemp = strFilePathTemp.Mid(nPos+1);
		while(true)
		{
			nPos = strFilePathTemp.Find("\\");
			if (nPos < 0)
			{
				break;
			}
			path += "\\" + strFilePathTemp.Left(nPos);
			if(!PathIsDirectory(path))
			{
				CreateDirectory(path,NULL);
			}
			strFilePathTemp = strFilePathTemp.Mid(nPos+1);

		}

		//文件长
		char szFileLen[16] = {0};
		nRecvLen = peer.recv_n (szFileLen,NUM_FILE_LEN );
		if (0 >= nRecvLen)
		{
			strRet = "12";
			break;
		} 
		//MessageBox("文件长:"+CString(szFileLen));
		int nFileLen = atoi( szFileLen );
		if (0 >= nFileLen)
		{
			strRet = "13";
			break;
		}

		CFile file2;
		if (!file2.Open(strFilePath,CFile::modeCreate|CFile::modeWrite))
		{
			strRet = "14";
			break;
		}

		int nReadFileLenCount = 0;
		do 
		{
			int nReadFileLen = (nFileLen - nReadFileLenCount) < MAX_FILE_SIZE ? (nFileLen - nReadFileLenCount) : MAX_FILE_SIZE;

			char *szFile = new char[nReadFileLen];
			nRecvLen = peer.recv_n (szFile,nReadFileLen );
			if (0 >= nRecvLen)
			{
				strRet = "15";
				break;
			} 

			file2.Write(szFile,nReadFileLen);
			delete [] szFile;
			nReadFileLenCount += nReadFileLen;
		}while(nFileLen - nReadFileLenCount > 0 );

		file2.Flush();
		file2.Close();

		strRet = "00";

	} while (0);
	return strRet;
}

CString Cra2yuriClientDlg::RecvChar(ACE_SOCK_Stream &peer, CString &strRecv)
{
	CString strRet = "97";
	do 
	{
		//字符串长
		char szCharLen[16] = {0};
		int nRecvLen = peer.recv_n (szCharLen,4 );
		if (0 >= nRecvLen)
		{
			strRet = "16";
			break;
		} 
		//MessageBox("字符串长:"+CString(szCharLen));
		int nCharLen = atoi( szCharLen );
		if (0 >= nCharLen)
		{
			strRet = "17";
			break;
		}

		//字符串
		char *szChar = new char [nCharLen+1];
		szChar[nCharLen] = '\0';
		nRecvLen = peer.recv_n (szChar,nCharLen);
		if (0 >= nRecvLen)
		{
			strRet = "18";
			break;
		} 

		strRecv = CString(szChar);

		delete []szChar;
	} while (0);
	return strRet;
}

void Cra2yuriClientDlg::run()
{
	bStartUdp();
	bStartTcp();
}

void Cra2yuriClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	
}
