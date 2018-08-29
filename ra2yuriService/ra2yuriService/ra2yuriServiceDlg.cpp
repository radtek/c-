
// ra2yuriServiceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ra2yuriService.h"
#include "ra2yuriServiceDlg.h"
#include "afxdialogex.h"

#include "CMD.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cra2yuriServiceDlg �Ի���




Cra2yuriServiceDlg::Cra2yuriServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cra2yuriServiceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cra2yuriServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
}

BEGIN_MESSAGE_MAP(Cra2yuriServiceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEND, &Cra2yuriServiceDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &Cra2yuriServiceDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SELECT_FILE, &Cra2yuriServiceDlg::OnBnClickedBtnSelectFile)
END_MESSAGE_MAP()


// Cra2yuriServiceDlg ��Ϣ�������

BOOL Cra2yuriServiceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	m_strHead = "ra2yuri";


	CString strFileName = GetLocalPath() + "config.ini";
	int ipCount = GetPrivateProfileInt("SETTING","IPCOUNT",0,strFileName);

	for (int i = 0; i < ipCount; i++)
	{
		CString keyName;
		keyName.Format("IP%d",i);
		char ip[32] = {0};
		GetPrivateProfileString("SETTING",keyName,"",ip,32,strFileName);
		CString strIP(ip);
		m_listIP.AddTail(strIP);
	}

	if( 0 != m_listIP.GetCount() )
	{
		CString strIPEdit = "";
		for (POSITION p = m_listIP.GetHeadPosition(); p != NULL; )
		{
			strIPEdit += m_listIP.GetNext(p);
			strIPEdit += "\r\n";
		}
		SetDlgItemText(IDC_EDIT_IP,strIPEdit);
		UpdateWindow();
	}

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cra2yuriServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cra2yuriServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cra2yuriServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

int Cra2yuriServiceDlg::nSendChar(CString strSend, CString IP , int port /*=6000*/)
{
	ACE_SOCK_Connector  connector;
	ACE_SOCK_Stream     stream;
	ACE_INET_Addr addr ;//(31001,"10.10.10.10");
	int nRet = 0;
	do
	{
		if (IP.IsEmpty())
		{
			nRet = -1;
			break;
		}

		if (strSend.IsEmpty())
		{
			nRet = -2;
			break;
		}
		
		addr.set(port,IP);

		int nConn = connector.connect(stream, addr);
		if ( nConn == -1 )
		{
			nRet = -3;
			break;
		}

		if ( 0 > stream.send_n(strSend,strSend.GetLength()) )
		{
			nRet = -4;
			break;
		}

		char szRecvData[32] = {0};
		int nRecv = stream.recv_n( szRecvData, m_strHead.GetLength() + NUM_RETU_LEN);
		if ( -1 == nRecv  )
		{
			nRet = -5;
			break;
		}
		else if ( 0 == nRecv  )
		{
			nRet = -6;
			break;
		}
		//��������
		//ͷ��־+����ֵ(����2λ)
		//ra2yuri00
		CString strRecvData(szRecvData);

		CString strHead = strRecvData.Left(m_strHead.GetLength());
		if (strHead != m_strHead)
		{
			nRet = -7;
			break;
		}

		int nRecvRet = atoi(strRecvData.Right(NUM_RETU_LEN));
		if (nRecvRet != 0)
		{
			nRet = -8;
			break;
		}

		nRet = 0;

	} while (0);

	return nRet;
}

int Cra2yuriServiceDlg::nSendFile(CString filePath, CString IP , int port /*=6000*/)
{

	//��־ͷ+�����ܳ�(�䳤10λ)+����(����4λ)+�ļ�·������(�䳤4λ)+�ļ�·������+�ļ���С(�䳤10λ)+�ļ�
	//ra2yuri0000000012FILE0008E:\1.ini0000000004XXXX
	int nRet = -999;
	
	ACE_SOCK_Connector  connector;
	ACE_SOCK_Stream     stream;
	ACE_INET_Addr addr ;//(31001,"10.10.10.10");

	CString strType = "FILE";

	CFile file;

	do 
	{
		if (IP.IsEmpty())
		{
			nRet = -1;
			break;
		}

		if (filePath.IsEmpty())
		{
			nRet = -2;
			break;
		}

		if (!file.Open(filePath,CFile::modeRead))
		{
			nRet = -3;
			break;
		}

		int nFileLen = file.GetLength();


		CString strFileLen;
		strFileLen.Format("%010d",nFileLen);

		CString strPathLen;
		strPathLen.Format("%04d",filePath.GetLength());

		CString sendData = strType + strPathLen + filePath + strFileLen;

		CString strDataLen;
		strDataLen.Format("%010d",sendData.GetLength() + nFileLen);

		sendData = m_strHead + strDataLen + sendData;

		addr.set(port,IP);

		int nConn = connector.connect(stream, addr);
		if ( nConn == -1 )
		{
			nRet = -4;
			break;
		}

		if ( 0 > stream.send_n(sendData,sendData.GetLength()) )
		{
			nRet = -5;
			break;
		}

		m_progress.SetRange(0,100);
		int nReadFileLenCount = 0;
		do 
		{
			long timeStart = GetTickCount();
			int nReadFileLen = (nFileLen - nReadFileLenCount) < MAX_FILE_SIZE ? (nFileLen - nReadFileLenCount) : MAX_FILE_SIZE;

			char *szFile = new char[nReadFileLen];
			if ( 0 >= file.Read(szFile,nReadFileLen) )
			{
				nRet = -6;
				break;
			}

			if( 0 > stream.send_n(szFile, nReadFileLen) )
			{
				nRet = -7;
				break;
			}
			delete [] szFile;
			nReadFileLenCount += nReadFileLen;
			
			//������

			int percent = (int)nReadFileLenCount * 100.0 / nFileLen;
			m_progress.SetPos(percent);
			
			long timeEnd = GetTickCount();
			double time = (timeEnd - timeStart)/2338340.0;
			double v = nReadFileLen/time/1024/1024/1024;

			CString strPercent;
			strPercent.Format("%d%% %.2fM/s",percent,v); //�ٷֱ�
			SetDlgItemText(IDC_STATIC_PERCENT,strPercent);
			UpdateWindow();
						

		}while(nFileLen - nReadFileLenCount > 0 );

		char szRecvData[32] = {0};
		int nRecv = stream.recv_n( szRecvData, m_strHead.GetLength() + NUM_RETU_LEN);
		if ( -1 == nRecv  )
		{
			nRet = -8;
			break;
		}
		else if ( 0 == nRecv  )
		{
			nRet = -9;
			break;
		}
		//��������
		//ͷ��־+����ֵ(����2λ)
		//ra2yuri00
		CString strRecvData(szRecvData);

		CString strHead = strRecvData.Left(m_strHead.GetLength());
		if (strHead != m_strHead)
		{
			nRet = -10;
			break;
		}

		int nRecvRet = atoi(strRecvData.Right(NUM_RETU_LEN));
		if (nRecvRet != 0)
		{
			nRet = -10 - nRecvRet;
			break;
		}

		nRet = 0;

	} while (0);

	m_progress.SetPos(0);
	SetDlgItemText(IDC_STATIC_PERCENT,"0% 0M/s");
	//UpdateWindow();

	file.Close();
	stream.close();

	return nRet;
}

void GetNameAndIp()
{

	struct   hostent   *host;
	struct   in_addr   *ptr;

	DWORD   dwScope = RESOURCE_CONTEXT;
	NETRESOURCE   *NetResource = NULL;
	HANDLE   hEnum;
	WNetOpenEnum(dwScope, NULL, NULL, NULL, &hEnum);

	WSADATA   wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	if (hEnum)
	{
		DWORD   Count = 0xFFFFFFFF;
		DWORD   BufferSize = 10240;
		LPVOID   Buffer = new char[10240];
		WNetEnumResource(hEnum, &Count, Buffer, &BufferSize);
		NetResource = (NETRESOURCE*)Buffer;

		char szHostName[200];

		for (unsigned int i = 0; i < BufferSize / sizeof(NETRESOURCE); i++, NetResource++)
		{
			if (NetResource->dwUsage == RESOURCEUSAGE_CONTAINER && NetResource->dwType == RESOURCETYPE_ANY)
			{
				if (NetResource->lpRemoteName)
				{
					CString   strFullName = NetResource->lpRemoteName;
					if (0 == strFullName.Left(2).Compare(_T("\\\\")))
						strFullName = strFullName.Right(strFullName.GetLength() - 2);

					gethostname(szHostName, strlen(szHostName));
					USES_CONVERSION;
					char *pchar = (LPSTR)(LPCSTR)strFullName;
					host = gethostbyname(pchar);

					if (host == NULL)   continue;
					ptr = (struct in_addr *)   host->h_addr_list[0];

					CString str = "";
					for (int n = 0; n<4; n++)
					{
						CString addr;
						if (n > 0)
						{
							str += ".";
						}
						int value = (unsigned int)((unsigned char*)host->h_addr_list[0])[n];
						char p[20];
						sprintf(p, "%d", value);
						str += p;
					}

					MessageBox(NULL,str,NULL,NULL);
					//std::cout <<"IP:"<< str <<" Name:"<<host->h_name<< std::endl;
				}
			}
		}
		delete   Buffer;
		WNetCloseEnum(hEnum);
	}
	WSACleanup();
}


//ֻ����һ��ip��ַ
bool GetLocalIP(char* ip)  
{  
	//1.��ʼ��wsa  
	WSADATA wsaData;  
	int ret=WSAStartup(MAKEWORD(2,2),&wsaData);  
	if (ret!=0)  
	{  
		return false;  
	}  
	//2.��ȡ������  
	char hostname[256];  
	ret=gethostname(hostname,sizeof(hostname));  
	if (ret==SOCKET_ERROR)  
	{  
		return false;  
	}  
	//3.��ȡ����ip  
	HOSTENT* host=gethostbyname(hostname);  
	if (host==NULL)  
	{  
		return false;  
	}  
	//4.ת��Ϊchar*����������  
	strcpy(ip,inet_ntoa(*(in_addr*)*host->h_addr_list));  
	return true;  
} 

TCHAR* StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}
//���cmd����������
CString strCmdSend(CString str)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		//MessageBox(NULL,"Error on CreatePipe()!",NULL,NULL);
		return NULL;
	}
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline=StringToChar(str);
	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		//MessageBox(NULL,"Error on CreateProcess()!",NULL,NULL);
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer,0,4096);
	CString output;
	DWORD byteRead;
	while(true)
	{
		if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	return output;
}
//�и��ַ���

CStringArray* Split(CString str, char ch)
{
	str.TrimLeft(ch);
	str.TrimRight(ch); 
	int nStart = 0;
	int nLastStart = 0;
	CStringArray *pStrArray = new CStringArray();
	while(-1 != (nStart = str.Find(ch, nStart)))
	{
		if(nLastStart != nStart)
		{// ��������ch,
			pStrArray->Add(str.Mid(nLastStart, nStart - nLastStart));
		}
		nStart ++;
		nLastStart = nStart;
	}
	pStrArray->Add(str.Mid(nLastStart));	// ���һ���Ӵ�
	return pStrArray;
}

void listIP(CString str,CStringList &list)
{
	CString strCheck = "0123456789.";
	CString IP = "";
	int nPos = 0;
	int ipSum = 0;
	for(int i = 0; i < str.GetLength(); i++)
	{
		if( strCheck.Find(str[i]) < 0 )
		{
			if (ipSum > 0)
			{
				IP = str.Mid(nPos,ipSum);
				//MessageBox(NULL,IP,NULL,NULL);
				CStringArray *arr = Split(IP,'.');
				if (arr->GetCount() == 4)
				{
					list.AddTail(IP);
				}
			}
			
			ipSum = 0;
			continue;
		}
		else
		{
			ipSum++;
		}

		if (ipSum == 1)
		{
			nPos = i;
		}

	}
}


CString Cra2yuriServiceDlg::GetLocalPath()
{
	TCHAR _szPath[MAX_PATH + 1]={0};
	GetModuleFileName(NULL, _szPath, MAX_PATH);
	(_tcsrchr(_szPath, _T('\\')))[1] = 0;//ɾ���ļ�����ֻ���·�� �ִ�

	CString strPath(_szPath);
	return strPath;
}

void Cra2yuriServiceDlg::OnBnClickedBtnSearch()
{
	// ��������
	m_listIP.RemoveAll();
	SetDlgItemText(IDC_EDIT_IP,"������...");
	UpdateWindow();

	
	ACE_INET_Addr localHost(5000), remote;
	ACE_SOCK_Dgram_Bcast udp(localHost);

	ACE_Time_Value  timeout(1);
	
	int nSend = udp.send(m_strHead,m_strHead.GetLength(),6000);
	if (nSend < 0)
	{
		MessageBox("ʧ��");
		return;
	}

	CString strIPEdit = "";
	while(true)
	{
		char recvBuf[32] = {0};
		int recvLen = udp.recv(recvBuf, 32, remote,0,&timeout);
		if (recvLen == -1)
		{
			break;
		}

		CString strRecv(recvBuf);
		if (strRecv != m_strHead + "00")
		{
			continue;
		}

		CString strIP = remote.get_host_addr();
		m_listIP.AddTail(strIP);
		
	}
	udp.close();

	do 
	{
		if( 0 != m_listIP.GetCount() )
		{
			//MessageBox("type udp");
			break;
		}
		

		CString strCmd = strCmdSend("arp -a");
		CStringList listip;
		listIP(strCmd,listip);

		//ra2yuri0000000004ISME
		CString strType = "ISME";
		CString strDataLen;
		strDataLen.Format("%010d",strType.GetLength());
		CString strSend = m_strHead + strDataLen + strType;
		for (POSITION p = listip.GetHeadPosition(); p != NULL; )
		{
			CString strIP = listip.GetNext(p);
			if( 0 == nSendChar(strSend,strIP) )
			{
				m_listIP.AddTail(strIP);
			}
		}
		if( 0 != m_listIP.GetCount() )
		{
			
			//MessageBox("type tcp");
			break;
		}
		MessageBox("δ�ҵ��͑���");
	} while (0);
	
	CString strFileName = GetLocalPath() + "config.ini";
	CString str;
	str.Format("%d",m_listIP.GetCount());
	WritePrivateProfileString("SETTING","IPCOUNT",str,strFileName);
	int i = 0;
	for (POSITION p = m_listIP.GetHeadPosition(); p != NULL;i++ )
	{
		str.Format("IP%d",i);
		WritePrivateProfileString("SETTING",str,m_listIP.GetNext(p),strFileName);

		strIPEdit += m_listIP.GetNext(p);
		strIPEdit += "\r\n";
	}
	
	SetDlgItemText(IDC_EDIT_IP,strIPEdit);
	UpdateWindow();
}


void Cra2yuriServiceDlg::OnBnClickedBtnSelectFile()
{
	// ѡ���ļ�

	m_listFilePath.RemoveAll();
	SetDlgItemText(IDC_EDIT_FILE_PATH,"");

	CFileDialog fDlg(TRUE,NULL, NULL, OFN_ALLOWMULTISELECT);  
	
	if( fDlg.DoModal() == IDOK )
	{
		CString filePathEdit = "";
		for (POSITION pos = fDlg.GetStartPosition(); pos != NULL;)
		{
			CString filePath = fDlg.GetNextPathName(pos);
			m_listFilePath.AddTail(filePath);
			filePathEdit += filePath;
			filePathEdit += "\r\n";
		}
		SetDlgItemText(IDC_EDIT_FILE_PATH,filePathEdit);
	}
}

void Cra2yuriServiceDlg::OnBnClickedBtnSend()
{
	// �����ļ�
	
	if (m_listIP.GetCount() == 0)
	{
		MessageBox("δ����������","��ʾ");
		return;
	}

	if (m_listFilePath.GetCount() == 0)
	{
		MessageBox("δѡ���ļ�","��ʾ");
		return;
	}

	bool bSuccess = true; 
	for (POSITION posI = m_listIP.GetHeadPosition(); posI != NULL; )
	{
		CString IP = m_listIP.GetNext(posI);
		for (POSITION posJ = m_listFilePath.GetHeadPosition(); posJ != NULL; )
		{
			CString filePath = m_listFilePath.GetNext(posJ);
			int nRet = nSendFile(filePath,IP);
			if( 0 != nRet )
			{
				CString strShow;
				strShow.Format("IP: %s FILE: %s ErrCode: %d",IP,filePath,nRet);
				MessageBox(strShow,"����ʧ��");
				bSuccess = false;
				break;
			}
		}
	}

	if (bSuccess)
	{
		MessageBox("���ͳɹ�","��ʾ");
	}
	

}

