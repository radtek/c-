#pragma once

// faceDetectOcxPropPage.h : CfaceDetectOcxPropPage ����ҳ���������


// CfaceDetectOcxPropPage : �й�ʵ�ֵ���Ϣ������� faceDetectOcxPropPage.cpp��

class CfaceDetectOcxPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CfaceDetectOcxPropPage)
	DECLARE_OLECREATE_EX(CfaceDetectOcxPropPage)

// ���캯��
public:
	CfaceDetectOcxPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_FACEDETECTOCX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

