
// DrinkWaterDlg.h : 头文件
//

#pragma once
#include "afxwin.h"

#define WM_SYSTEMTRAY WM_USER+5
#define	TIMER_HIDE_WINDOW 0
#define	TIMER_DRINK_WATER 1
#define	TIMER_BLINK		  2

// CDrinkWaterDlg 对话框
class CDrinkWaterDlg : public CDialogEx
{
// 构造
public:
	CDrinkWaterDlg(CWnd* pParent = NULL);	// 标准构造函数
	

// 对话框数据
	enum { IDD = IDD_DRINKWATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	
	DECLARE_MESSAGE_MAP()

	
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);
	void showDlg(bool bAuto = false);
	void DrinkPlaySound(CString strSound);
	void Autostart();
	bool bGetStartUp();

	NOTIFYICONDATA m_NotifyIcon;//托盘图标
	BOOL m_bIconIsExist;//托盘图标已存在
	BOOL m_bPlaySoundOnce;
	CString m_strCulPath;
	CFont m_editFont;

	bool m_bStartUp;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMfcmenubutton1();
	afx_msg void OnDestroy();
	afx_msg void OnExit();
	afx_msg void On32777();
	afx_msg void On32781();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_pic;
};
