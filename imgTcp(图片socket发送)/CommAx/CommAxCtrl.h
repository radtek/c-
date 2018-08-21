#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>


using namespace std;

// CommAxCtrl.h : CCommAxCtrl ActiveX �ؼ����������


// CCommAxCtrl : �й�ʵ�ֵ���Ϣ������� CommAxCtrl.cpp��

class CCommAxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCommAxCtrl)

// ���캯��
public:
	CCommAxCtrl();

// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CCommAxCtrl();

	DECLARE_OLECREATE_EX(CCommAxCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(CCommAxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCommAxCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CCommAxCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:
	enum {
		dispidNETSendAndReceiveImg = 2L,
		dispidNETSendAndReceive = 1L
	};
protected:
	BSTR NETSendAndReceive(LPCTSTR strJson);
	BSTR NETSendAndReceiveImg(LPCTSTR strJson, LPCTSTR strImgPath);
private:
	int			CalcNetDataLength( const string &strHead );
	string		cis_Bytes2HexString( const string &str ) ;
	string		GenerateHead(int nDataLen);
	string		cis_HexString2Bytes( const string &str );
	string		cis_Utf82Ansi(const string& strUtf8);
	string		cis_Utf162Ansi(const wstring& strUtf16);
private:
	int    m_HeadLength;
	string m_HeadCode;
	bool   m_bIncludeSelf;
};

