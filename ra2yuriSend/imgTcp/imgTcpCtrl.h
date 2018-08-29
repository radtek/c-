#pragma once

#include <string>

using namespace std;

// imgTcpCtrl.h : CimgTcpCtrl ActiveX �ؼ����������


// CimgTcpCtrl : �й�ʵ�ֵ���Ϣ������� imgTcpCtrl.cpp��

class CimgTcpCtrl : public COleControl
{
	DECLARE_DYNCREATE(CimgTcpCtrl)

// ���캯��
public:
	CimgTcpCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CimgTcpCtrl();

	DECLARE_OLECREATE_EX(CimgTcpCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CimgTcpCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CimgTcpCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CimgTcpCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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

