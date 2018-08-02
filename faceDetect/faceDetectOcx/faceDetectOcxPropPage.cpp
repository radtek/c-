// faceDetectOcxPropPage.cpp : CfaceDetectOcxPropPage 属性页类的实现。

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CfaceDetectOcxPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CfaceDetectOcxPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CfaceDetectOcxPropPage, "FACEDETECTOCX.faceDetectOcxPropPage.1",
	0x935576b7, 0x9957, 0x4aa8, 0xb2, 0xb7, 0x32, 0x6c, 0x42, 0xaa, 0x72, 0xce)



// CfaceDetectOcxPropPage::CfaceDetectOcxPropPageFactory::UpdateRegistry -
// 添加或移除 CfaceDetectOcxPropPage 的系统注册表项

BOOL CfaceDetectOcxPropPage::CfaceDetectOcxPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_FACEDETECTOCX_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CfaceDetectOcxPropPage::CfaceDetectOcxPropPage - 构造函数

CfaceDetectOcxPropPage::CfaceDetectOcxPropPage() :
	COlePropertyPage(IDD, IDS_FACEDETECTOCX_PPG_CAPTION)
{
}



// CfaceDetectOcxPropPage::DoDataExchange - 在页和属性间移动数据

void CfaceDetectOcxPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CfaceDetectOcxPropPage 消息处理程序
