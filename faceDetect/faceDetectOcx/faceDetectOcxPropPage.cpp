// faceDetectOcxPropPage.cpp : CfaceDetectOcxPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CfaceDetectOcxPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CfaceDetectOcxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CfaceDetectOcxPropPage, "FACEDETECTOCX.faceDetectOcxPropPage.1",
	0x935576b7, 0x9957, 0x4aa8, 0xb2, 0xb7, 0x32, 0x6c, 0x42, 0xaa, 0x72, 0xce)



// CfaceDetectOcxPropPage::CfaceDetectOcxPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CfaceDetectOcxPropPage ��ϵͳע�����

BOOL CfaceDetectOcxPropPage::CfaceDetectOcxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FACEDETECTOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CfaceDetectOcxPropPage::CfaceDetectOcxPropPage - ���캯��

CfaceDetectOcxPropPage::CfaceDetectOcxPropPage() :
	COlePropertyPage(IDD, IDS_FACEDETECTOCX_PPG_CAPTION)
{
}



// CfaceDetectOcxPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CfaceDetectOcxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CfaceDetectOcxPropPage ��Ϣ�������
