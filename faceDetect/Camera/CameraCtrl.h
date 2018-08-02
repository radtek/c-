#pragma once
#include "DlgShow.h"

// CameraCtrl.h : CCameraCtrl ActiveX �ؼ����������


// CCameraCtrl : �й�ʵ�ֵ���Ϣ������� CameraCtrl.cpp��

class CCameraCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCameraCtrl)

// ���캯��
public:
	CCameraCtrl();
	CDlgShow m_dlgShow;
// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CCameraCtrl();

	DECLARE_OLECREATE_EX(CCameraCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CCameraCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCameraCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CCameraCtrl)		// �������ƺ�����״̬

	// ����ؼ�֧��
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		dispidhySetWindowPosition = 6L,
		dispidhySetPicMaxWidthHeight = 5L,
		dispidhySavePicturePath = 4L,
		dispidhyGetPicture = 3L,
		dispidhyStopCamera = 2L,
		dispidhyStartCamera = 1L
	};
protected:
	LONG hyStartCamera(void);
	void hyStopCamera(void);
	BSTR hyGetPicture(void);
	void hySavePicturePath(LPCTSTR strPath);
	void hySetPicMaxWidthHeight(LONG width, LONG height);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	void hySetWindowPosition(LONG windowX, LONG windowY, LONG windowWidth, LONG windowHeight);
};

