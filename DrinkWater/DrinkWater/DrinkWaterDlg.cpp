
// DrinkWaterDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrinkWater.h"
#include "DrinkWaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//��ȡ��ǰ�ļ�λ��
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


// CDrinkWaterDlg �Ի���




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
	ON_MESSAGE(WM_SYSTEMTRAY, OnSystemtray)//�����Ϣӳ��
	ON_COMMAND(ID_32781, &CDrinkWaterDlg::On32781)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDrinkWaterDlg ��Ϣ�������

BOOL CDrinkWaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	if (!m_bIconIsExist)
	{
		m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
		//NotifyIcon.hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		m_NotifyIcon.hIcon = m_hIcon;  //�����Ǿ�Ҳ����
		m_NotifyIcon.hWnd = m_hWnd;
		lstrcpy(m_NotifyIcon.szTip,_T("��ˮ����"));
		m_NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
		m_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
		m_bIconIsExist = Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon);
		//MoveWindow(0,0,0,0);
		SetTimer(TIMER_HIDE_WINDOW,1,NULL);
		SetTimer(TIMER_DRINK_WATER,1000,NULL);
		
		
	}

	m_editFont.CreatePointFont(300, "����"); 
	GetDlgItem(IDC_ST_GREET)->SetFont(&m_editFont);
	GetDlgItem(IDC_ST_DRINK)->SetFont(&m_editFont);

	SetWindowPos(&wndTopMost,0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDrinkWaterDlg::OnPaint()
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
HCURSOR CDrinkWaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CDrinkWaterDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(Shell_NotifyIcon(NIM_ADD,&m_NotifyIcon))
		m_bIconIsExist = TRUE;
	this->ShowWindow(HIDE_WINDOW);
	//CDialogEx::OnClose();
}


void CDrinkWaterDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnClose();
}


afx_msg LRESULT CDrinkWaterDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{
	//wParam���յ���ͼ���ID����lParam���յ���������Ϊ
	//  if(wParam!=IDR_MAINFRAME)     
	//      return    1;     
	switch(lParam)     
	{     
	case  WM_RBUTTONDOWN://�Ҽ�����ʱ������ݲ˵�
		{       
			
			CMenu menuexit;
			//menu.LoadMenuW(IDR_MENU);//���ز˵���Դ
			menuexit.LoadMenu(IDR_MENU1);
			CMenu *pPopup=menuexit.GetSubMenu(0);
			CPoint mypoint;
			GetCursorPos(&mypoint);
			//ClientToScreen(&mypoint);//���ͻ�������ת��Ϊ��Ļ����
			SetForegroundWindow(); 
			PostMessage(WM_NULL,0,0);


			//��ʾ�Ҽ��˵��������ര��ӵ�С�
			pPopup->TrackPopupMenu(TPM_LEFTALIGN,mypoint.x,mypoint.y,this); 
			
		}     
		break;  
	case  WM_LBUTTONDBLCLK://��������Ĵ���     
		{     
			//ModifyStyleEx(0,WS_EX_TOPMOST);   //���Ըı䴰�ڵ���ʾ���
			//OnTimer(TIMER_DRINK_WATER);
			showDlg();
		}     
		break;     
	} 
	return 0;
}

void CDrinkWaterDlg::OnBnClickedMfcmenubutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


void CDrinkWaterDlg::OnDestroy()
{
	if(Shell_NotifyIcon(NIM_DELETE,&m_NotifyIcon))
		m_bIconIsExist = FALSE;
	//MessageBox("OnDestroy");
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
}



void CDrinkWaterDlg::On32781()
{
	// TODO: �ڴ���������������
	//�����Ҽ��˳�
	CDialogEx::OnCancel();
}



void CDrinkWaterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CTime time		= CTime::GetCurrentTime();
	CString strHour = time.Format("%H");
	CString strMin	= time.Format("%M");
	CString strSec	= time.Format("%S");

	int nHour = atoi(strHour);
	CString strGreet = "�ʺ�";

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
			strGreet = "�賿��";
		else if (nHour >= 5 && nHour < 7)
			strGreet = "�峿��";
		else if (nHour >= 7 && nHour < 9)
			strGreet = "���Ϻ�";
		else if (nHour >= 9 && nHour < 12)
			strGreet = "�����";
		else if (nHour >= 12 && nHour < 14)
			strGreet = "�����";
		else if (nHour >= 14 && nHour < 17)
			strGreet = "�����";
		else if (nHour >= 17 && nHour < 19)
			strGreet = "�����";
		else if (nHour >= 19 && nHour < 21)
			strGreet = "���Ϻ�";
		else if (nHour >= 21 && nHour < 24)
			strGreet = "��ҹ��";

		SetDlgItemText(IDC_ST_GREET,strGreet);

		GetDlgItem(IDC_ST_DRINK)->ShowWindow(SW_SHOW);
		SetTimer(TIMER_BLINK,700,NULL);

		if (m_bPlaySoundOnce)
		{
			m_bPlaySoundOnce = FALSE;
			//PlaySound(m_strCulPath+"\\sound\\drink.wav",NULL, SND_ASYNC|SND_NODEFAULT );
			CString strPlaySound = strGreet.Left(4) + "|��|��ȥ��ˮ��";
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
	GetClientRect(&cr);//��ȡ�Ի���ͻ������С   
	ClientToScreen(&cr);//ת��ΪӫĻ����   

	int x = GetSystemMetrics ( SM_CXSCREEN ); 
	int y= GetSystemMetrics ( SM_CYSCREEN ); 
	MoveWindow((x - cr.Width())/2 ,cr.top, cr.Width(),cr.Height());
	ShowWindow(SW_SHOWNORMAL);

	
}
//�ַ����ָ�
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