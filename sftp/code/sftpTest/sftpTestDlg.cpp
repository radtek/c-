
// sftpTestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "sftpTest.h"
#include "sftpTestDlg.h"
#include "afxdialogex.h"

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


// CsftpTestDlg �Ի���




CsftpTestDlg::CsftpTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CsftpTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsftpTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CsftpTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CsftpTestDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CsftpTestDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CsftpTestDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CsftpTestDlg ��Ϣ��������

BOOL CsftpTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

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

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CsftpTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CsftpTestDlg::OnPaint()
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
HCURSOR CsftpTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

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
FunFtprename		ftprename	= NULL;

void CsftpTestDlg::OnBnClickedButton1()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	bool bRet = false;
	do 
	{
		m_hinstacne = ::LoadLibrary("sftp.dll");
		if (m_hinstacne == NULL) 
			break;

		ftpopen = (FunFtpopen)::GetProcAddress(m_hinstacne, "ftpopen");
		if (ftpopen == NULL)
		{
			break;
		}

		ftpclose = (FunFtpclose)::GetProcAddress(m_hinstacne, "ftpclose");
		if (ftpclose == NULL)
		{
			break;
		}

		uploaddir = (FunUploaddir)::GetProcAddress(m_hinstacne, "uploaddir");
		if (uploaddir == NULL)
		{
			break;
		}

		uploadfile = (FunUploadfile)::GetProcAddress(m_hinstacne, "uploadfile");
		if (uploadfile == NULL)
		{
			break;
		}
		ftprename = (FunFtprename)::GetProcAddress(m_hinstacne, "ftprename");
		if (ftprename == NULL)
		{
			break;
		}


		bRet = true;
	} while (0);
	
	if (!bRet)
	{
		::FreeLibrary(m_hinstacne);
		m_hinstacne = NULL;
		MessageBox("fail");
		return;

	}
	MessageBox("ok");
	
}


void CsftpTestDlg::OnBnClickedButton2()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
}


void CsftpTestDlg::OnBnClickedButton3()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	/*
	const char* szRet =  uploadfile("");
	const char csOpen[] = "{\"ip\":\"192.168.1.4\",\"port\":\"23\",\"username\":\"test\",\"password\":\"1234\",\"mode\":\"1\"}";
	const char csCd[] = "{\"dir\":\"/test\"}";
	const char csDownloadFile[] = "{\"srcfilename\":\"/test/gwi/test1/test2/test.txt\",\"destfilename\":\"d:\\\\ftp_test\\\\ftp_file\\\\test1.txt\"}";
	const char csDownloadDir[] = "{\"srcdir\":\"/test\",\"destdir\":\"D:\\\\ftp_test\\\\ftpdir\"}";
	const char csUpdateFile[] = "{\"srcfilename\":\"e:\\\\ftp_sftp-master\\\\123.txt\",\"destfilename\":\"/gwi/txt/file/g.txt\"}";
	const char csUpdateDir[] = "{\"srcdir\":\"d:\\\\ftp_test\\\\update\\\\dir\",\"destdir\":\"/gwi/txt\"}";
	const char* pcRet = NULL;
	pcRet = ftpopen(csOpen);
	MessageBox(pcRet);
	*/
	//printf("%s\n",pcRet);
/*
	//getchar();
	pcRet = cd(csCd);
	printf("%s\n",pcRet);
	//getchar();
	*/
	//pcRet = uploadfile(csUpdateFile);
	//MessageBox(pcRet);
	//printf("%s\n",pcRet);
	//getchar();

	//pcRet = uploaddir(csUpdateDir);
	//printf("%s\n",pcRet);
	//getchar();
			
	//pcRet = downloadfile(csDownloadFile);
	//printf("%s\n",pcRet);
	//getchar();
			
			
	//pcRet = downloaddir(csDownloadDir);
	//printf("%s\n",pcRet);
	//getchar();
			

	//pcRet = ftpclose(NULL);
	//MessageBox(pcRet);
	//printf("%s\n",pcRet);
	//getchar();
	do 
	{
		if (!ftpopen("192.168.0.159",23,"test","1234"))
		{
			break;
		}
		/*
		if (!uploadfile("/gwi/txt","e:\\\\ftp_sftp-master\\\\123.txt","33.TXT"))
		{
			break;
		}
		*/
		if (!ftprename("/gwi/txt/33.txt","/gwi/txt/3333s.txt"))
		{
			break;
		}
		
	} while (0);
	ftpclose();
}