// CameraCtrl.cpp : CCameraCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "Camera.h"
#include "CameraCtrl.h"
#include "CameraPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCameraCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CCameraCtrl, COleControl)
	ON_MESSAGE(OCM_COMMAND, &CCameraCtrl::OnOcmCommand)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CCameraCtrl, COleControl)
	DISP_FUNCTION_ID(CCameraCtrl, "hyStartCamera", dispidhyStartCamera, hyStartCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hyStopCamera", dispidhyStopCamera, hyStopCamera, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hyGetPicture", dispidhyGetPicture, hyGetPicture, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CCameraCtrl, "hySavePicturePath", dispidhySavePicturePath, hySavePicturePath, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CCameraCtrl, "hySetPicMaxWidthHeight", dispidhySetPicMaxWidthHeight, hySetPicMaxWidthHeight, VT_EMPTY, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CCameraCtrl, "hySetWindowPosition", dispidhySetWindowPosition, hySetWindowPosition, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CCameraCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CCameraCtrl, 1)
	PROPPAGEID(CCameraPropPage::guid)
END_PROPPAGEIDS(CCameraCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CCameraCtrl, "CAMERA.CameraCtrl.1",
	0x942c47f3, 0x62f8, 0x454f, 0xbc, 0x2c, 0xca, 0x6c, 0x5a, 0xe1, 0xcc, 0xd1)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CCameraCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DCamera = { 0xB4F6D8B7, 0x84AB, 0x4759, { 0xA6, 0x9C, 0x87, 0x2E, 0x18, 0x57, 0xA8, 0x59 } };
const IID IID_DCameraEvents = { 0xBB502913, 0x9A6D, 0x4702, { 0x8A, 0x1D, 0x8E, 0x2E, 0xED, 0xD2, 0x58, 0xDF } };


// �ؼ�������Ϣ

static const DWORD _dwCameraOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCameraCtrl, IDS_CAMERA, _dwCameraOleMisc)



// CCameraCtrl::CCameraCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CCameraCtrl ��ϵͳע�����

BOOL CCameraCtrl::CCameraCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

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



// CCameraCtrl::CCameraCtrl - ���캯��

CCameraCtrl::CCameraCtrl()
{
	InitializeIIDs(&IID_DCamera, &IID_DCameraEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
}



// CCameraCtrl::~CCameraCtrl - ��������

CCameraCtrl::~CCameraCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CCameraCtrl::OnDraw - ��ͼ����

void CCameraCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	DoSuperclassPaint(pdc, rcBounds);
}



// CCameraCtrl::DoPropExchange - �־���֧��

void CCameraCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CCameraCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CCameraCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CCameraCtrl::PreCreateWindow - �޸� CreateWindowEx �Ĳ���

BOOL CCameraCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.lpszClass = _T("STATIC");
	return COleControl::PreCreateWindow(cs);
}



// CCameraCtrl::IsSubclassedControl - ����һ������ؼ�

BOOL CCameraCtrl::IsSubclassedControl()
{
	return TRUE;
}



// CCameraCtrl::OnOcmCommand - ����������Ϣ

LRESULT CCameraCtrl::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
	WORD wNotifyCode = HIWORD(wParam);

	// TODO: �ڴ˽�ͨ wNotifyCode��

	return 0;
}



// CCameraCtrl ��Ϣ�������


LONG CCameraCtrl::hyStartCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������

	return theApp.StartCamera();
}


void CCameraCtrl::hyStopCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return theApp.StopCamera();
}


BSTR CCameraCtrl::hyGetPicture(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	strResult = theApp.GetPicture();

	return strResult.AllocSysString();
}


void CCameraCtrl::hySavePicturePath(LPCTSTR strPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return theApp.SavePicturePath(strPath);
}


void CCameraCtrl::hySetPicMaxWidthHeight(LONG width, LONG height)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return theApp.SetPicMaxWidthHeight(width, height);
}


void CCameraCtrl::hySetWindowPosition(LONG windowX, LONG windowY, LONG windowWidth, LONG windowHeight)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	return theApp.SetWindowPosition(windowX,windowY,windowWidth,windowHeight);
}

int CCameraCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_dlgShow.Create(IDD_DLG_SHOW,this);

	return 0;
}


void CCameraCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	RECT activeXRect;
	GetClientRect(&activeXRect);
	m_dlgShow.MoveWindow(&activeXRect);
	m_dlgShow.ShowWindow(SW_SHOW);
}

