#pragma once

#include "DlgCamera.h"

// faceDetectOcxCtrl.h : CfaceDetectOcxCtrl ActiveX �ؼ����������


// CfaceDetectOcxCtrl : �й�ʵ�ֵ���Ϣ������� faceDetectOcxCtrl.cpp��



class CfaceDetectOcxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CfaceDetectOcxCtrl)

// ���캯��
public:
	CfaceDetectOcxCtrl();
	CDlgCamera m_DlgCamera;

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CfaceDetectOcxCtrl();

	DECLARE_OLECREATE_EX(CfaceDetectOcxCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CfaceDetectOcxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CfaceDetectOcxCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CfaceDetectOcxCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		eventidhyLivesInfoCallBack = 5L,

		dispidhyStartLiveDetect = 4L,
		dispidhyStopCamera = 3L,
		dispidhyStartCamera = 2L,
		dispidhyInit = 1L
	};
protected:
	//��ʼ��
	LONG hyInit(void);
	//��������ͷ
	LONG hyStartCamera(void);
	//ֹͣ����ͷ
	LONG hyStopCamera(void);
	//������(ͼƬ·��)
	LONG hyStartLiveDetect(LPCTSTR jpgImagePath);



public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//�������̷߳�������Ϣ
	afx_msg LRESULT OnMessageChanged(WPARAM wParam, LPARAM lParam);

public:
	//�ص�����ͼƬbase64
	void hyLivesInfoCallBack(LPCTSTR base64Image)
	{
		FireEvent(eventidhyLivesInfoCallBack, EVENT_PARAM(VTS_BSTR), base64Image);
	}
};
