#pragma once

// imgTcpPropPage.h : CimgTcpPropPage 属性页类的声明。


// CimgTcpPropPage : 有关实现的信息，请参阅 imgTcpPropPage.cpp。

class CimgTcpPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CimgTcpPropPage)
	DECLARE_OLECREATE_EX(CimgTcpPropPage)

// 构造函数
public:
	CimgTcpPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_IMGTCP };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

