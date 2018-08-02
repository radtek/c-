
// sftpTestDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "sftpTest.h"
#include "sftpTestDlg.h"
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


// CsftpTestDlg 对话框




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


// CsftpTestDlg 消息处理程序

BOOL CsftpTestDlg::OnInitDialog()
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CsftpTestDlg::OnPaint()
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
HCURSOR CsftpTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef bool (* FunFtpopenPassword)(const char* , int , const char* , const char* );
typedef bool (* FunFtpopenPublicKey)(const char* , int , const char* , const char* ,const char* , const char* );
typedef bool (* FunFtpclose)();
typedef bool (* FunUploaddir)(const char* ,const char*);
typedef bool (* FunUploadfile)(const char* ,const char*,const char*);
typedef bool (* FunFtprename)(const char* ,const char*);
HINSTANCE m_hinstacne = NULL;
FunFtpopenPassword		ftpopenPassword		= NULL;
FunFtpopenPublicKey		ftpopenPublicKey	= NULL;
FunFtpclose		ftpclose	= NULL;
FunUploaddir	uploaddir	= NULL;
FunUploadfile	uploadfile	= NULL;
FunFtprename		ftprename	= NULL;

void CsftpTestDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bRet = false;
	do 
	{
		m_hinstacne = ::LoadLibrary("sftp.dll");
		if (m_hinstacne == NULL) 
			break;

		ftpopenPassword = (FunFtpopenPassword)::GetProcAddress(m_hinstacne, "ftpopenPassword");
		if (ftpopenPassword == NULL)
		{
			break;
		}

		ftpopenPublicKey = (FunFtpopenPublicKey)::GetProcAddress(m_hinstacne, "ftpopenPublicKey");
		if (ftpopenPublicKey == NULL)
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
	// TODO: 在此添加控件通知处理程序代码
	do 
	{
		
		if (!ftpopenPassword("192.168.18.104",23,"user2","1234"))
		{
			MessageBox("ftpopen失败");
			break;
		}
		
		if (!uploadfile("/","e:\\\\ttt.txt",NULL))
		{
			MessageBox("uploadfile失败");
			break;
		}
		
		
		if (!ftprename("/ttt.txt","/user_password_upload.txt"))
		{
			break;
		}
		
		MessageBox("成功 上传user_password_upload.txt");
	} while (0);
	ftpclose();
}


void CsftpTestDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	
	do 
	{
		
		if (!ftpopenPublicKey("192.168.18.104",23,"user1","","",""))
		{
			MessageBox("ftpopen失败");
			break;
		}
		
		
		
		if (!uploadfile("/","e:\\\\ttt.txt",NULL))
		{
			MessageBox("uploadfile失败");
			break;
		}
		
		
		if (!ftprename("/ttt.txt","/publicKey_upload.txt"))
		{
			break;
		}
		
		MessageBox("成功 上传publicKey_upload.txt");
	} while (0);
	ftpclose();
}
