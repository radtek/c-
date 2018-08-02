// CameraCtrl.cpp : CCameraCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "Camera.h"
#include "CameraCtrl.h"
#include "CameraPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCameraCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CCameraCtrl, COleControl)
	ON_MESSAGE(OCM_COMMAND, &CCameraCtrl::OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CCameraCtrl, COleControl)
	DISP_FUNCTION_ID(CCameraCtrl, "hyStartCamera", dispidhyStartCamera, hyStartCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hyStopCamera", dispidhyStopCamera, hyStopCamera, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hyGetPicture", dispidhyGetPicture, hyGetPicture, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hySavePicturePath", dispidhySavePicturePath, hySavePicturePath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CCameraCtrl, "hySetPicMaxWidthHeight", dispidhySetPicMaxWidthHeight, hySetPicMaxWidthHeight, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CCameraCtrl, "hySetWindowPosition", dispidhySetWindowPosition, hySetWindowPosition, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CCameraCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CCameraCtrl, 1)
	PROPPAGEID(CCameraPropPage::guid)
END_PROPPAGEIDS(CCameraCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CCameraCtrl, "CAMERA.CameraCtrl.1",
	0x942c47f3, 0x62f8, 0x454f, 0xbc, 0x2c, 0xca, 0x6c, 0x5a, 0xe1, 0xcc, 0xd1)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CCameraCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DCamera = { 0xB4F6D8B7, 0x84AB, 0x4759, { 0xA6, 0x9C, 0x87, 0x2E, 0x18, 0x57, 0xA8, 0x59 } };
const IID IID_DCameraEvents = { 0xBB502913, 0x9A6D, 0x4702, { 0x8A, 0x1D, 0x8E, 0x2E, 0xED, 0xD2, 0x58, 0xDF } };


// 控件类型信息

static const DWORD _dwCameraOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCameraCtrl, IDS_CAMERA, _dwCameraOleMisc)



// CCameraCtrl::CCameraCtrlFactory::UpdateRegistry -
// 添加或移除 CCameraCtrl 的系统注册表项

BOOL CCameraCtrl::CCameraCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_CAMERA,
			IDB_CAMERA,
			afxRegApartmentThreading,
			_dwCameraOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CCameraCtrl::CCameraCtrl - 构造函数

CCameraCtrl::CCameraCtrl()
{
	InitializeIIDs(&IID_DCamera, &IID_DCameraEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CCameraCtrl::~CCameraCtrl - 析构函数

CCameraCtrl::~CCameraCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CCameraCtrl::OnDraw - 绘图函数

void CCameraCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	DoSuperclassPaint(pdc, rcBounds);
}



// CCameraCtrl::DoPropExchange - 持久性支持

void CCameraCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CCameraCtrl::OnResetState - 将控件重置为默认状态

void CCameraCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CCameraCtrl::PreCreateWindow - 修改 CreateWindowEx 的参数

BOOL CCameraCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}



// CCameraCtrl::IsSubclassedControl - 这是一个子类控件

BOOL CCameraCtrl::IsSubclassedControl()
{
	return TRUE;
}



// CCameraCtrl::OnOcmCommand - 处理命令消息

LRESULT CCameraCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	// TODO: 在此接通 wNotifyCode。

	return 0;
}



// CCameraCtrl 消息处理程序


LONG CCameraCtrl::hyStartCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码

	return theApp.StartCamera();
}


void CCameraCtrl::hyStopCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return theApp.StopCamera();
}


BSTR CCameraCtrl::hyGetPicture(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	strResult = theApp.GetPicture();

	return strResult.AllocSysString();
}


void CCameraCtrl::hySavePicturePath(LPCTSTR strPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return theApp.SavePicturePath(strPath);
}


void CCameraCtrl::hySetPicMaxWidthHeight(LONG width, LONG height)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return theApp.SetPicMaxWidthHeight(width, height);
}


void CCameraCtrl::hySetWindowPosition(LONG windowX, LONG windowY, LONG windowWidth, LONG windowHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	return theApp.SetWindowPosition(windowX,windowY,windowWidth,windowHeight);
}

int CCameraCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_dlgShow.Create(IDD_DLG_SHOW,this);

	return 0;
}


void CCameraCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	RECT activeXRect;
	GetClientRect(&activeXRect);
	m_dlgShow.MoveWindow(&activeXRect);
	m_dlgShow.ShowWindow(SW_SHOW);
}

