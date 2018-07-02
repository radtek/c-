// CommAx.cpp : CCommAxApp �� DLL ע���ʵ�֡�

#include "stdafx.h"
#include "CommAx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCommAxApp theApp;

const GUID CDECL _tlid = { 0x174AB775, 0x4743, 0x4A1E, { 0xBF, 0x7B, 0xD5, 0x70, 0xBA, 0xC9, 0xB0, 0xF3 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CCommAxApp::InitInstance - DLL ��ʼ��

BOOL CCommAxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣
		TrcErrInit("CommAx");
	}

	return bInit;
}



// CCommAxApp::ExitInstance - DLL ��ֹ

int CCommAxApp::ExitInstance()
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
