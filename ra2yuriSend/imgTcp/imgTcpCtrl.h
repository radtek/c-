#pragma once

#include <string>

using namespace std;

// imgTcpCtrl.h : CimgTcpCtrl ActiveX 控件类的声明。


// CimgTcpCtrl : 有关实现的信息，请参阅 imgTcpCtrl.cpp。

class CimgTcpCtrl : public COleControl
{
	DECLARE_DYNCREATE(CimgTcpCtrl)

// 构造函数
public:
	CimgTcpCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CimgTcpCtrl();

	DECLARE_OLECREATE_EX(CimgTcpCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CimgTcpCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CimgTcpCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CimgTcpCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidstrSetTimeout = 4L,
		dispidstrGetLastError = 3L,
		dispidstrSetIpAndPort = 2L,
		dispidstrImgTcp = 1L
	};

	string m_strIp;
	string m_strPort;

	int m_nHeadLength;

	bool m_bIpAndPort;

	string m_strLastError;

	long m_lTimeoutMillisecond;


protected:
	BSTR strImgTcp(LPCTSTR strJsonLength, LPCTSTR strJson, LPCTSTR strImgPath);
	BSTR strSetIpAndPort(LPCTSTR strIp, LPCTSTR strPort);
	BSTR strGetLastError(void);
	BSTR strSetTimeout(LPCTSTR strMillisecond);
};

