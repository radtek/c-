// imgTcp.cpp : CimgTcpApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "imgTcp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CimgTcpApp theApp;

const GUID CDECL _tlid = { 0x65AC894F, 0x4D99, 0x4D16, { 0xBE, 0xEC, 0x5B, 0x44, 0x8, 0xE5, 0x62, 0x8D } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CimgTcpApp::InitInstance - DLL ��ʼ��

BOOL CimgTcpApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
	}

	return bInit;
}



// CimgTcpApp::ExitInstance - DLL ��ֹ

int CimgTcpApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
