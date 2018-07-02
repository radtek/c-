// CommAxPropPage.cpp : CCommAxPropPage 属性页类的实现。

#include "stdafx.h"
#include "CommAx.h"
#include "CommAxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCommAxPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CCommAxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CCommAxPropPage, "COMMAX.CommAxPropPage.1",
	0x81f80b2a, 0xbdd1, 0x4a38, 0x95, 0x5c, 0xab, 0x60, 0x74, 0x44, 0x3e, 0xb1)



// CCommAxPropPage::CCommAxPropPageFactory::UpdateRegistry -
// 添加或移除 CCommAxPropPage 的系统注册表项

BOOL CCommAxPropPage::CCommAxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_COMMAX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CCommAxPropPage::CCommAxPropPage - 构造函数

CCommAxPropPage::CCommAxPropPage() :
	COlePropertyPage(IDD, IDS_COMMAX_PPG_CAPTION)
{
}



// CCommAxPropPage::DoDataExchange - 在页和属性间移动数据

void CCommAxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CCommAxPropPage 消息处理程序
