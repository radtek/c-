#pragma once

// imgTcpPropPage.h : CimgTcpPropPage ����ҳ���������


// CimgTcpPropPage : �й�ʵ�ֵ���Ϣ������� imgTcpPropPage.cpp��

class CimgTcpPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CimgTcpPropPage)
	DECLARE_OLECREATE_EX(CimgTcpPropPage)

// ���캯��
public:
	CimgTcpPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_IMGTCP };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

