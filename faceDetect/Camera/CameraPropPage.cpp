// CameraPropPage.cpp : CCameraPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "Camera.h"
#include "CameraPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCameraPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CCameraPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CCameraPropPage, "CAMERA.CameraPropPage.1",
	0x9e3cda45, 0xfc60, 0x4a80, 0xb1, 0xd1, 0xdf, 0x10, 0x4d, 0x37, 0xd3, 0x9f)



// CCameraPropPage::CCameraPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CCameraPropPage ��ϵͳע�����

BOOL CCameraPropPage::CCameraPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CAMERA_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CCameraPropPage::CCameraPropPage - ���캯��

CCameraPropPage::CCameraPropPage() :
	COlePropertyPage(IDD, IDS_CAMERA_PPG_CAPTION)
{
}



// CCameraPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CCameraPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CCameraPropPage ��Ϣ�������
