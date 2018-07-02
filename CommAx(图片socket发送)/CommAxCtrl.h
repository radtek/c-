#pragma once

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>


using namespace std;

// CommAxCtrl.h : CCommAxCtrl ActiveX 控件类的声明。


// CCommAxCtrl : 有关实现的信息，请参阅 CommAxCtrl.cpp。

class CCommAxCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCommAxCtrl)

// 构造函数
public:
	CCommAxCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CCommAxCtrl();

	DECLARE_OLECREATE_EX(CCommAxCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CCommAxCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCommAxCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CCommAxCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
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

