// imgTcpPropPage.cpp : CimgTcpPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "imgTcp.h"
#include "imgTcpPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CimgTcpPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CimgTcpPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CimgTcpPropPage, "IMGTCP.imgTcpPropPage.1",
	0xdf3b7d12, 0x887d, 0x43ec, 0xa9, 0x56, 0xf5, 0x83, 0x7a, 0xb8, 0x79, 0xae)



// CimgTcpPropPage::CimgTcpPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� CimgTcpPropPage ��ϵͳע�����

BOOL CimgTcpPropPage::CimgTcpPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_IMGTCP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CimgTcpPropPage::CimgTcpPropPage - ���캯��

CimgTcpPropPage::CimgTcpPropPage() :
	COlePropertyPage(IDD, IDS_IMGTCP_PPG_CAPTION)
{
}



// CimgTcpPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void CimgTcpPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CimgTcpPropPage ��Ϣ�������
