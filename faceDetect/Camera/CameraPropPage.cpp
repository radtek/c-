// CameraPropPage.cpp : CCameraPropPage 属性页类的实现。

#include "stdafx.h"
#include "Camera.h"
#include "CameraPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCameraPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CCameraPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CCameraPropPage, "CAMERA.CameraPropPage.1",
	0x9e3cda45, 0xfc60, 0x4a80, 0xb1, 0xd1, 0xdf, 0x10, 0x4d, 0x37, 0xd3, 0x9f)



// CCameraPropPage::CCameraPropPageFactory::UpdateRegistry -
// 添加或移除 CCameraPropPage 的系统注册表项

BOOL CCameraPropPage::CCameraPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CAMERA_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CCameraPropPage::CCameraPropPage - 构造函数

CCameraPropPage::CCameraPropPage() :
	COlePropertyPage(IDD, IDS_CAMERA_PPG_CAPTION)
{
}



// CCameraPropPage::DoDataExchange - 在页和属性间移动数据

void CCameraPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CCameraPropPage 消息处理程序
