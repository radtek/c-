#pragma once

// CameraPropPage.h : CCameraPropPage ����ҳ���������


// CCameraPropPage : �й�ʵ�ֵ���Ϣ������� CameraPropPage.cpp��

class CCameraPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCameraPropPage)
	DECLARE_OLECREATE_EX(CCameraPropPage)

// ���캯��
public:
	CCameraPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_CAMERA };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

