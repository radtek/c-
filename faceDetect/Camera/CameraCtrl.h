#pragma once
#include "DlgShow.h"

// CameraCtrl.h : CCameraCtrl ActiveX 控件类的声明。


// CCameraCtrl : 有关实现的信息，请参阅 CameraCtrl.cpp。

class CCameraCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCameraCtrl)

// 构造函数
public:
	CCameraCtrl();
	CDlgShow m_dlgShow;
// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CCameraCtrl();

	DECLARE_OLECREATE_EX(CCameraCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CCameraCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCameraCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CCameraCtrl)		// 类型名称和杂项状态

	// 子类控件支持
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
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

