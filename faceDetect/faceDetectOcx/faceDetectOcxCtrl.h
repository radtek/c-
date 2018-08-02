#pragma once

#include "DlgCamera.h"

// faceDetectOcxCtrl.h : CfaceDetectOcxCtrl ActiveX 控件类的声明。


// CfaceDetectOcxCtrl : 有关实现的信息，请参阅 faceDetectOcxCtrl.cpp。



class CfaceDetectOcxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CfaceDetectOcxCtrl)

// 构造函数
public:
	CfaceDetectOcxCtrl();
	CDlgCamera m_DlgCamera;

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CfaceDetectOcxCtrl();

	DECLARE_OLECREATE_EX(CfaceDetectOcxCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CfaceDetectOcxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CfaceDetectOcxCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CfaceDetectOcxCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		eventidhyLivesInfoCallBack = 5L,

		dispidhyStartLiveDetect = 4L,
		dispidhyStopCamera = 3L,
		dispidhyStartCamera = 2L,
		dispidhyInit = 1L
	};
protected:
	//初始化
	LONG hyInit(void);
	//开启摄像头
	LONG hyStartCamera(void);
	//停止摄像头
	LONG hyStopCamera(void);
	//活体检测(图片路径)
	LONG hyStartLiveDetect(LPCTSTR jpgImagePath);



public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//接收子线程发来的消息
	afx_msg LRESULT OnMessageChanged(WPARAM wParam, LPARAM lParam);

public:
	//回调函数图片base64
	void hyLivesInfoCallBack(LPCTSTR base64Image)
	{
		FireEvent(eventidhyLivesInfoCallBack, EVENT_PARAM(VTS_BSTR), base64Image);
	}
};
