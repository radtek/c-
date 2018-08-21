// CommAx.cpp : CCommAxApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "CommAx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCommAxApp theApp;

const GUID CDECL _tlid = { 0x174AB775, 0x4743, 0x4A1E, { 0xBF, 0x7B, 0xD5, 0x70, 0xBA, 0xC9, 0xB0, 0xF3 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CCommAxApp::InitInstance - DLL 初始化

BOOL CCommAxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
		TrcErrInit("CommAx");
	}

	return bInit;
}



// CCommAxApp::ExitInstance - DLL 终止

int CCommAxApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
