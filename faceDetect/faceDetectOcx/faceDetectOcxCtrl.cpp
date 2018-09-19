// faceDetectOcxCtrl.cpp : CfaceDetectOcxCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxCtrl.h"
#include "faceDetectOcxPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CfaceDetectOcxCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CfaceDetectOcxCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageChanged) 
	ON_WM_TIMER()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CfaceDetectOcxCtrl, COleControl)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyInit", dispidhyInit, hyInit, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStartCamera", dispidhyStartCamera, hyStartCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStopCamera", dispidhyStopCamera, hyStopCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStartLiveDetect", dispidhyStartLiveDetect, hyStartLiveDetect, VT_I4, VTS_BSTR )
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CfaceDetectOcxCtrl, COleControl)
	EVENT_CUSTOM_ID("hyLivesInfoCallBack", eventidhyLivesInfoCallBack, hyLivesInfoCallBack, VTS_BSTR)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CfaceDetectOcxCtrl, 1)
	PROPPAGEID(CfaceDetectOcxPropPage::guid)
END_PROPPAGEIDS(CfaceDetectOcxCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CfaceDetectOcxCtrl, "FACEDETECTOCX.faceDetectOcxCtrl.1",
	0xf620116e, 0xe336, 0x4039, 0xb5, 0x5, 0x95, 0x3a, 0xcc, 0x37, 0x13, 0x63)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CfaceDetectOcxCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DfaceDetectOcx = { 0xB42C8159, 0x7C6C, 0x42CF, { 0x8B, 0x9E, 0xAE, 0x8E, 0xEE, 0x7E, 0x8E, 0x1D } };
const IID IID_DfaceDetectOcxEvents = { 0xBAAEA834, 0xE078, 0x4CC0, { 0xA2, 0xCF, 0x37, 0x86, 0xC6, 0x69, 0x9F, 0xAA } };


// 控件类型信息

static const DWORD _dwfaceDetectOcxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CfaceDetectOcxCtrl, IDS_FACEDETECTOCX, _dwfaceDetectOcxOleMisc)



// CfaceDetectOcxCtrl::CfaceDetectOcxCtrlFactory::UpdateRegistry -
// 添加或移除 CfaceDetectOcxCtrl 的系统注册表项

BOOL CfaceDetectOcxCtrl::CfaceDetectOcxCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_FACEDETECTOCX,
			IDB_FACEDETECTOCX,
			afxRegApartmentThreading,
			_dwfaceDetectOcxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CfaceDetectOcxCtrl::CfaceDetectOcxCtrl - 构造函数

CfaceDetectOcxCtrl::CfaceDetectOcxCtrl()
{
	InitializeIIDs(&IID_DfaceDetectOcx, &IID_DfaceDetectOcxEvents);
	// TODO: 在此初始化控件的实例数据。
	
}



// CfaceDetectOcxCtrl::~CfaceDetectOcxCtrl - 析构函数

CfaceDetectOcxCtrl::~CfaceDetectOcxCtrl()
{
	// TODO: 在此清理控件的实例数据。
	
}



// CfaceDetectOcxCtrl::OnDraw - 绘图函数

void CfaceDetectOcxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CfaceDetectOcxCtrl::DoPropExchange - 持久性支持

void CfaceDetectOcxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CfaceDetectOcxCtrl::OnResetState - 将控件重置为默认状态

void CfaceDetectOcxCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CfaceDetectOcxCtrl 消息处理程序


LONG CfaceDetectOcxCtrl::hyInit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	g_theApp.setHwnd(this->GetSafeHwnd());
		
	return g_theApp.Init();
}


LONG CfaceDetectOcxCtrl::hyStartCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码

	return g_theApp.StartCamera();
}


LONG CfaceDetectOcxCtrl::hyStopCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码

	return g_theApp.StopCamera();
}


LONG CfaceDetectOcxCtrl::hyStartLiveDetect(LPCTSTR jpgImagePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	
	return g_theApp.StartLiveDetect(jpgImagePath);
}


int CfaceDetectOcxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_DlgCamera.Create(IDD_DLG_CAMERA,this);
	return 0;
}


void CfaceDetectOcxCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	/*
	RECT activeXRect;
	GetClientRect(&activeXRect);
	m_DlgCamera.MoveWindow(&activeXRect);
	*/
	g_theApp.setCameraSize(cx,cy);
	
}


LRESULT CfaceDetectOcxCtrl::OnMessageChanged(WPARAM wParam, LPARAM lParam) 
{ 
	// TODO: 处理用户自定义消息 
	COPYDATASTRUCT* pCopyDataStruct = (COPYDATASTRUCT*)lParam;
	
	if (pCopyDataStruct)
	{
		if ( pCopyDataStruct->dwData  == 1001 )
		{
			LPBYTE	lpData = (LPBYTE) pCopyDataStruct->lpData;
			int	len = pCopyDataStruct->cbData;
			CString	strData ;
			::CopyMemory(strData.GetBufferSetLength( len ),
				lpData ,
				len ) ;
			TrcWritef( EC_INFO ,"OnMessageChanged: 接收data完毕");
			hyLivesInfoCallBack(strData);
			//MessageBox(strData);

			TrcWritef( EC_INFO ,"OnMessageChanged: 回调事件完毕");
		}
		
	}
	else
	{
		TrcWritef( EC_INFO ,"OnMessageChanged: pCopyDataStruct 空");
	}
	
	return 0; 
}


BOOL CfaceDetectOcxCtrl::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pCopyDataStruct)
	{
		if ( pCopyDataStruct->dwData  == 1001 )
		{
			LPBYTE	lpData = (LPBYTE) pCopyDataStruct->lpData;
			int	len = pCopyDataStruct->cbData;
			//CString	strData ;
			//::CopyMemory(strData.GetBufferSetLength( len ), lpData , len ) ;
			char *szData = new char[len+1];
			szData[len] = '\0';
			TrcWritef( EC_INFO ,"OnCopyData: len = %d",len);
			//memcpy_s(szData,len,lpData,len);
			memcpy(szData,lpData ,len);
			TrcWritef( EC_INFO ,"OnCopyData: 接收data完毕");
			string strData(szData);
			hyLivesInfoCallBack(strData.c_str());
			
			delete []szData;
			TrcWritef( EC_INFO ,"OnCopyData: 回调事件完毕");
		}
		else
		{
			TrcWritef( EC_INFO ,"OnCopyData: pCopyDataStruct->dwData  != 1001");
		}

	}
	else
	{
		TrcWritef( EC_INFO ,"OnCopyData: pCopyDataStruct 空");

	}

	return COleControl::OnCopyData(pWnd, pCopyDataStruct);
}
