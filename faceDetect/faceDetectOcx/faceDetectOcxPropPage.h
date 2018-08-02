#pragma once

// faceDetectOcxPropPage.h : CfaceDetectOcxPropPage 属性页类的声明。


// CfaceDetectOcxPropPage : 有关实现的信息，请参阅 faceDetectOcxPropPage.cpp。

class CfaceDetectOcxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CfaceDetectOcxPropPage)
	DECLARE_OLECREATE_EX(CfaceDetectOcxPropPage)

// 构造函数
public:
	CfaceDetectOcxPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_FACEDETECTOCX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

