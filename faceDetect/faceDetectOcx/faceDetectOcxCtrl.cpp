// faceDetectOcxCtrl.cpp : CfaceDetectOcxCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxCtrl.h"
#include "faceDetectOcxPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CfaceDetectOcxCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CfaceDetectOcxCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MESSAGE_EVENT, OnMessageChanged) 
	ON_WM_TIMER()
	ON_WM_COPYDATA()
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CfaceDetectOcxCtrl, COleControl)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyInit", dispidhyInit, hyInit, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStartCamera", dispidhyStartCamera, hyStartCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStopCamera", dispidhyStopCamera, hyStopCamera, VT_I4, VTS_NONE)
	DISP_FUNCTION_ID(CfaceDetectOcxCtrl, "hyStartLiveDetect", dispidhyStartLiveDetect, hyStartLiveDetect, VT_I4, VTS_BSTR )
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CfaceDetectOcxCtrl, COleControl)
	EVENT_CUSTOM_ID("hyLivesInfoCallBack", eventidhyLivesInfoCallBack, hyLivesInfoCallBack, VTS_BSTR)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CfaceDetectOcxCtrl, 1)
	PROPPAGEID(CfaceDetectOcxPropPage::guid)
END_PROPPAGEIDS(CfaceDetectOcxCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CfaceDetectOcxCtrl, "FACEDETECTOCX.faceDetectOcxCtrl.1",
	0xf620116e, 0xe336, 0x4039, 0xb5, 0x5, 0x95, 0x3a, 0xcc, 0x37, 0x13, 0x63)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CfaceDetectOcxCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DfaceDetectOcx = { 0xB42C8159, 0x7C6C, 0x42CF, { 0x8B, 0x9E, 0xAE, 0x8E, 0xEE, 0x7E, 0x8E, 0x1D } };
const IID IID_DfaceDetectOcxEvents = { 0xBAAEA834, 0xE078, 0x4CC0, { 0xA2, 0xCF, 0x37, 0x86, 0xC6, 0x69, 0x9F, 0xAA } };


// �ؼ�������Ϣ

static const DWORD _dwfaceDetectOcxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CfaceDetectOcxCtrl, IDS_FACEDETECTOCX, _dwfaceDetectOcxOleMisc)



// CfaceDetectOcxCtrl::CfaceDetectOcxCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CfaceDetectOcxCtrl ��ϵͳע�����

BOOL CfaceDetectOcxCtrl::CfaceDetectOcxCtrlFactory::UpdateRegistry(BOOL bRegister)
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



// CfaceDetectOcxCtrl::CfaceDetectOcxCtrl - ���캯��

CfaceDetectOcxCtrl::CfaceDetectOcxCtrl()
{
	InitializeIIDs(&IID_DfaceDetectOcx, &IID_DfaceDetectOcxEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	
}



// CfaceDetectOcxCtrl::~CfaceDetectOcxCtrl - ��������

CfaceDetectOcxCtrl::~CfaceDetectOcxCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
	
}



// CfaceDetectOcxCtrl::OnDraw - ��ͼ����

void CfaceDetectOcxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CfaceDetectOcxCtrl::DoPropExchange - �־���֧��

void CfaceDetectOcxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CfaceDetectOcxCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CfaceDetectOcxCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CfaceDetectOcxCtrl ��Ϣ�������


LONG CfaceDetectOcxCtrl::hyInit(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	g_theApp.setHwnd(this->GetSafeHwnd());
		
	return g_theApp.Init();
}


LONG CfaceDetectOcxCtrl::hyStartCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������

	return g_theApp.StartCamera();
}


LONG CfaceDetectOcxCtrl::hyStopCamera(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������

	return g_theApp.StopCamera();
}


LONG CfaceDetectOcxCtrl::hyStartLiveDetect(LPCTSTR jpgImagePath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: �ڴ���ӵ��ȴ���������
	
	return g_theApp.StartLiveDetect(jpgImagePath);
}


int CfaceDetectOcxCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	m_DlgCamera.Create(IDD_DLG_CAMERA,this);
	return 0;
}


void CfaceDetectOcxCtrl::OnSize(UINT nType, int cx, int cy)
{
	COleControl::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	/*
	RECT activeXRect;
	GetClientRect(&activeXRect);
	m_DlgCamera.MoveWindow(&activeXRect);
	*/
	g_theApp.setCameraSize(cx,cy);
	
}


LRESULT CfaceDetectOcxCtrl::OnMessageChanged(WPARAM wParam, LPARAM lParam) 
{ 
	// TODO: �����û��Զ�����Ϣ 
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
			TrcWritef( EC_INFO ,"OnMessageChanged: ����data���");
			hyLivesInfoCallBack(strData);
			//MessageBox(strData);

			TrcWritef( EC_INFO ,"OnMessageChanged: �ص��¼����");
		}
		
	}
	else
	{
		TrcWritef( EC_INFO ,"OnMessageChanged: pCopyDataStruct ��");
	}
	
	return 0; 
}


BOOL CfaceDetectOcxCtrl::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
			TrcWritef( EC_INFO ,"OnCopyData: ����data���");
			string strData(szData);
			hyLivesInfoCallBack(strData.c_str());
			
			delete []szData;
			TrcWritef( EC_INFO ,"OnCopyData: �ص��¼����");
		}
		else
		{
			TrcWritef( EC_INFO ,"OnCopyData: pCopyDataStruct->dwData  != 1001");
		}

	}
	else
	{
		TrcWritef( EC_INFO ,"OnCopyData: pCopyDataStruct ��");

	}

	return COleControl::OnCopyData(pWnd, pCopyDataStruct);
}
