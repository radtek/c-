#pragma once

// CameraPropPage.h : CCameraPropPage 属性页类的声明。


// CCameraPropPage : 有关实现的信息，请参阅 CameraPropPage.cpp。

class CCameraPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCameraPropPage)
	DECLARE_OLECREATE_EX(CCameraPropPage)

// 构造函数
public:
	CCameraPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_CAMERA };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

