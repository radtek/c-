
// ra2yuriClient.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ra2yuriClient.h"
#include "ra2yuriClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cra2yuriClientApp

BEGIN_MESSAGE_MAP(Cra2yuriClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// Cra2yuriClientApp ����

Cra2yuriClientApp::Cra2yuriClientApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
	
}


// Ψһ��һ�� Cra2yuriClientApp ����

Cra2yuriClientApp theApp;


// Cra2yuriClientApp ��ʼ��

BOOL Cra2yuriClientApp::InitInstance()
{
	m_handle = ::CreateMutex(NULL,FALSE,"ra2yuriClientONE");  
	if (GetLastError() == ERROR_ALREADY_EXISTS)  
	{  
		MessageBox(NULL,"����������","��ʾ",NULL);
		return FALSE;
	} 
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	Cra2yuriClientDlg dlg;
	m_pMainWnd = &dlg;
	//INT_PTR nResponse = dlg.DoModal();
	INT_PTR nResponse = dlg.Create(Cra2yuriClientDlg::IDD);
	//Sleep(2000);
	dlg.ShowWindow(SW_HIDE);
	dlg.RunModalLoop();  
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int Cra2yuriClientApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CloseHandle(m_handle);
	return CWinApp::ExitInstance();
}
