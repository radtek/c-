// imgTcp.cpp : CimgTcpApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "imgTcp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CimgTcpApp theApp;

const GUID CDECL _tlid = { 0x65AC894F, 0x4D99, 0x4D16, { 0xBE, 0xEC, 0x5B, 0x44, 0x8, 0xE5, 0x62, 0x8D } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CimgTcpApp::InitInstance - DLL 初始化

BOOL CimgTcpApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CimgTcpApp::ExitInstance - DLL 终止

int CimgTcpApp::ExitInstance()
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
