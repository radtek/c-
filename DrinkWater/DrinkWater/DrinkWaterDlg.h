
// DrinkWaterDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"

#define WM_SYSTEMTRAY WM_USER+5
#define	TIMER_HIDE_WINDOW 0
#define	TIMER_DRINK_WATER 1
#define	TIMER_BLINK		  2

// CDrinkWaterDlg �Ի���
class CDrinkWaterDlg : public CDialogEx
{
// ����
public:
	CDrinkWaterDlg(CWnd* pParent = NULL);	// ��׼���캯��
	

// �Ի�������
	enum { IDD = IDD_DRINKWATER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	NOTIFYICONDATA m_NotifyIcon;//����ͼ��
	BOOL m_bIconIsExist;//����ͼ���Ѵ���
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
