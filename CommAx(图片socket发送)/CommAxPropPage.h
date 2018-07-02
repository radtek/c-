#pragma once

// CommAxPropPage.h : CCommAxPropPage 属性页类的声明。


// CCommAxPropPage : 有关实现的信息，请参阅 CommAxPropPage.cpp。

class CCommAxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCommAxPropPage)
	DECLARE_OLECREATE_EX(CCommAxPropPage)

// 构造函数
public:
	CCommAxPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_COMMAX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

