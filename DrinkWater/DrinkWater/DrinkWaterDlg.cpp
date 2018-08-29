
// DrinkWaterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DrinkWater.h"
#include "DrinkWaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//获取当前文件位置
CString GetCurrentPath()   
{   
	HMODULE module = GetModuleHandle(0);   
	char pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   

	CString csFullPath(pFileName);   
	int nPos = csFullPath.ReverseFind( _T('\\') );   
	if( nPos < 0 )   
		return CString("");   
	else   
		return csFullPath.Left( nPos );   
} 
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


// CDrinkWaterDlg 对话框




CDrinkWaterDlg::CDrinkWaterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDrinkWaterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bIconIsExist = FALSE;
	m_bPlaySoundOnce = FALSE;
}

void CDrinkWaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDrinkWaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CDrinkWaterDlg::OnBnClickedOk)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SYSTEMTRAY, OnSystemtray)//添加消息映射
	ON_COMMAND(ID_32781, &CDrinkWaterDlg::On32781)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDrinkWaterDlg 消息处理程序

BOOL CDrinkWaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_strCulPath = GetCurrentPath();

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

	if (!m_bIconIsExist)
	{
		m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
		//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		m_NotifyIcon.hIcon = m_hIcon;  //上面那句也可以
		m_NotifyIcon.hWnd = m_hWnd;
		lstrcpy(m_NotifyIcon.szTip,_T("喝水提醒"));
		m_NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
		m_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_bIconIsExist = Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon);
		//MoveWindow(0,0,0,0);
		SetTimer(TIMER_HIDE_WINDOW,1,NULL);
		SetTimer(TIMER_DRINK_WATER,1000,NULL);
		
		
	}

	m_editFont.CreatePointFont(300, "宋体"); 
	GetDlgItem(IDC_ST_GREET)->SetFont(&m_editFont);
	GetDlgItem(IDC_ST_DRINK)->SetFont(&m_editFont);

	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDrinkWaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDrinkWaterDlg::OnPaint()
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
HCURSOR CDrinkWaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CDrinkWaterDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	// TODO: 在此添加控件通知处理程序代码
	if(Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon))
		m_bIconIsExist = TRUE;
	this->ShowWindow(HIDE_WINDOW);
	//CDialogEx::OnClose();
}


void CDrinkWaterDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnClose();
}


afx_msg LRESULT CDrinkWaterDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	//wParam接收的是图标的ID，而lParam接收的是鼠标的行为
	//  if(wParam!=IDR_MAINFRAME)     
	//      return    1;     
	switch(lParam)     
	{     
	case  WM_RBUTTONDOWN://右键起来时弹出快捷菜单
		{       
			
			CMenu menuexit;
			//menu.LoadMenuW(IDR_MENU);//加载菜单资源
			menuexit.LoadMenu(IDR_MENU1);
			CMenu *pPopup=menuexit.GetSubMenu(0);
			CPoint mypoint;
			GetCursorPos(&mypoint);
			//ClientToScreen(&mypoint);//将客户区坐标转换为屏幕坐标
			SetForegroundWindow(); 
			PostMessage(WM_NULL,0,0);


			//显示右键菜单，由视类窗口拥有。
			pPopup->TrackPopupMenu(TPM_LEFTALIGN,mypoint.x,mypoint.y,this); 
			
		}     
		break;  
	case  WM_LBUTTONDBLCLK://左键单击的处理     
		{     
			//ModifyStyleEx(0,WS_EX_TOPMOST);   //可以改变窗口的显示风格
			//OnTimer(TIMER_DRINK_WATER);
			showDlg();
		}     
		break;     
	} 
	return 0;
}

void CDrinkWaterDlg::OnBnClickedMfcmenubutton1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CDrinkWaterDlg::OnDestroy()
{
	if(Shell_NotifyIcon(NIM_DELETE,&m_NotifyIcon))
		m_bIconIsExist = FALSE;
	//MessageBox("OnDestroy");
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}



void CDrinkWaterDlg::On32781()
{
	// TODO: 在此添加命令处理程序代码
	//托盘右键退出
	CDialogEx::OnCancel();
}



void CDrinkWaterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CTime time		= CTime::GetCurrentTime();
	CString strHour = time.Format("%H");
	CString strMin	= time.Format("%M");
	CString strSec	= time.Format("%S");

	int nHour = atoi(strHour);
	CString strGreet = "问候";

	switch(nIDEvent)
	{
	case TIMER_HIDE_WINDOW:
		ShowWindow(SW_HIDE);
		KillTimer(TIMER_HIDE_WINDOW);
		break;
	case TIMER_DRINK_WATER:
		if ( (strMin == "30" || strMin == "00") && strSec == "00")
			showDlg(true);

		if (m_bIconIsExist)
			//break;

		SetDlgItemText(IDC_ST_HOUR,strHour);
		SetDlgItemText(IDC_ST_MINUTE,strMin);
		SetDlgItemText(IDC_ST_SECOND,strSec);

		if (nHour >= 0 && nHour < 5)
			strGreet = "凌晨好";
		else if (nHour >= 5 && nHour < 7)
			strGreet = "清晨好";
		else if (nHour >= 7 && nHour < 9)
			strGreet = "早上好";
		else if (nHour >= 9 && nHour < 12)
			strGreet = "上午好";
		else if (nHour >= 12 && nHour < 14)
			strGreet = "中午好";
		else if (nHour >= 14 && nHour < 17)
			strGreet = "下午好";
		else if (nHour >= 17 && nHour < 19)
			strGreet = "傍晚好";
		else if (nHour >= 19 && nHour < 21)
			strGreet = "晚上好";
		else if (nHour >= 21 && nHour < 24)
			strGreet = "深夜好";

		SetDlgItemText(IDC_ST_GREET,strGreet);

		GetDlgItem(IDC_ST_DRINK)->ShowWindow(SW_SHOW);
		SetTimer(TIMER_BLINK,700,NULL);

		if (m_bPlaySoundOnce)
		{
			m_bPlaySoundOnce = FALSE;
			//PlaySound(m_strCulPath+"\\sound\\drink.wav",NULL, SND_ASYNC|SND_NODEFAULT );
			CString strPlaySound = strGreet.Left(4) + "|好|快去喝水了";
			DrinkPlaySound(strPlaySound);
			
		}

		break;
	case TIMER_BLINK:
		GetDlgItem(IDC_ST_DRINK)->ShowWindow(SW_HIDE);
		KillTimer(TIMER_BLINK);
		
		break;
	default:
		break;
	}
	
	CDialogEx::OnTimer(nIDEvent);
}


void CDrinkWaterDlg::showDlg(bool bAuto)
{
	m_bIconIsExist = FALSE;

	if (bAuto)
		m_bPlaySoundOnce = TRUE;

	if(Shell_NotifyIcon(NIM_DELETE,&m_NotifyIcon))
		m_bIconIsExist = FALSE;

	CRect cr;  
	GetClientRect(&cr);//获取对话框客户区域大小   
	ClientToScreen(&cr);//转换为荧幕坐标   

	int x = GetSystemMetrics ( SM_CXSCREEN ); 
	int y= GetSystemMetrics ( SM_CYSCREEN ); 
	MoveWindow((x - cr.Width())/2 ,cr.top, cr.Width(),cr.Height());
	ShowWindow(SW_SHOWNORMAL);

	
}
//字符串分割
int SplitString(const CString str, char split, CStringArray &strArray)  
{  
	strArray.RemoveAll();  
	CString strTemp = str;  
	int iIndex = 0;  
	while (1)  
	{  
		iIndex = strTemp.Find(split);  
		if(iIndex >= 0)  
		{  
			strArray.Add(strTemp.Left(iIndex));  
			strTemp = strTemp.Right(strTemp.GetLength()-iIndex-1);  
		}  
		else  
		{  
			break;  
		}  
	}  
	strArray.Add(strTemp);  

	return strArray.GetSize();  
} 

void CDrinkWaterDlg::DrinkPlaySound(CString strSound)
{
	CStringArray strArray;
	int nArraylength = SplitString(strSound,'|',strArray);
	if(nArraylength == 0)
		return;
	
	for (int i = 0; i < nArraylength; i++)
	{
		
		CString strPlaySound = m_strCulPath + "\\sound\\" + strArray[i] + ".wav";
		//MessageBox(strPlaySound);
		if (i == nArraylength - 1)
			PlaySound(strPlaySound,NULL, SND_ASYNC );
		else
			PlaySound(strPlaySound,NULL, SND_SYNC );
		
	}
	
	
	
}