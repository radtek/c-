// CommAxPropPage.cpp : CCommAxPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "CommAx.h"
#include "CommAxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCommAxPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CCommAxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CCommAxPropPage, "COMMAX.CommAxPropPage.1",
	0x81f80b2a, 0xbdd1, 0x4a38, 0x95, 0x5c, 0xab, 0x60, 0x74, 0x44, 0x3e, 0xb1)



// CCommAxPropPage::CCommAxPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CCommAxPropPage ��ϵͳע�����

BOOL CCommAxPropPage::CCommAxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_COMMAX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CCommAxPropPage::CCommAxPropPage - ���캯��

CCommAxPropPage::CCommAxPropPage() :
	COlePropertyPage(IDD, IDS_COMMAX_PPG_CAPTION)
{
}



// CCommAxPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CCommAxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CCommAxPropPage ��Ϣ�������
