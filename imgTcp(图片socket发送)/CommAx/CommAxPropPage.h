#pragma once

// CommAxPropPage.h : CCommAxPropPage ����ҳ���������


// CCommAxPropPage : �й�ʵ�ֵ���Ϣ������� CommAxPropPage.cpp��

class CCommAxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCommAxPropPage)
	DECLARE_OLECREATE_EX(CCommAxPropPage)

// ���캯��
public:
	CCommAxPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_COMMAX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

