
// ra2yuriEXEDlg.h : ͷ�ļ�
//

#pragma once


// Cra2yuriEXEDlg �Ի���
class Cra2yuriEXEDlg : public CDialogEx
{
// ����
public:
	Cra2yuriEXEDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RA2YURIEXE_DIALOG };

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
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnSelectFile();
	afx_msg void OnBnClickedBtnSend();
};
