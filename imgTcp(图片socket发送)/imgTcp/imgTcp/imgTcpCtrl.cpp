// imgTcpCtrl.cpp : CimgTcpCtrl ActiveX �ؼ����ʵ�֡�

#include "stdafx.h"
#include "imgTcp.h"
#include "imgTcpCtrl.h"
#include "imgTcpPropPage.h"
#include "afxdialogex.h"
#include  <afx.h>
#include <iostream>
#include <WinDef.h>


#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"

#include "ITrace.h"
#include "json/json.h"

#pragma comment(lib, "ACE.lib")
#pragma comment(lib, "Common.lib")
#pragma comment(lib, "lib_json.lib")



#define MAX_CONN_TIMEOUT_SECOND      3 /*TCP connection timeout in second.*/
#define MAX_CONN_TIMEOUT_MILLISECOND 0 /*TCP connection timeout in ms.*/
#define MAX_RECV_TIMEOUT_SECOND      1 /*TCP connection timeout in second.*/
#define MAX_RECV_TIMEOUT_MILLISECOND 0 /*TCP connection timeout in ms.*/

#define NUM_JSON	5
#define NUM_IMG		10
#define NUM_MARK	8

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CimgTcpCtrl, COleControl)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(CimgTcpCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// ����ӳ��

BEGIN_DISPATCH_MAP(CimgTcpCtrl, COleControl)
	DISP_FUNCTION_ID(CimgTcpCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strImgTcp", dispidstrImgTcp, strImgTcp, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strSetIpAndPort", dispidstrSetIpAndPort, strSetIpAndPort, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strGetLastError", dispidstrGetLastError, strGetLastError, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strSetTimeout", dispidstrSetTimeout, strSetTimeout, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()



// �¼�ӳ��

BEGIN_EVENT_MAP(CimgTcpCtrl, COleControl)
END_EVENT_MAP()



// ����ҳ

// TODO: ����Ҫ��Ӹ�������ҳ�����ס���Ӽ���!
BEGIN_PROPPAGEIDS(CimgTcpCtrl, 1)
	PROPPAGEID(CimgTcpPropPage::guid)
END_PROPPAGEIDS(CimgTcpCtrl)



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(CimgTcpCtrl, "IMGTCP.imgTcpCtrl.1",
	0x285d5807, 0x9271, 0x4783, 0xb7, 0x37, 0x75, 0xf1, 0xc5, 0x51, 0xad, 0xf2)



// ����� ID �Ͱ汾

IMPLEMENT_OLETYPELIB(CimgTcpCtrl, _tlid, _wVerMajor, _wVerMinor)



// �ӿ� ID

const IID IID_DimgTcp = { 0x61F94D57, 0x91C1, 0x4332, { 0x8F, 0x4F, 0xE1, 0x3C, 0x90, 0xE2, 0xD, 0x5D } };
const IID IID_DimgTcpEvents = { 0xBFD2A219, 0x2DB9, 0x4C4D, { 0x9D, 0x37, 0x5D, 0xC0, 0x2D, 0xE0, 0x2C, 0x7B } };


// �ؼ�������Ϣ

static const DWORD _dwimgTcpOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CimgTcpCtrl, IDS_IMGTCP, _dwimgTcpOleMisc)



// CimgTcpCtrl::CimgTcpCtrlFactory::UpdateRegistry -
// ��ӻ��Ƴ� CimgTcpCtrl ��ϵͳע�����

BOOL CimgTcpCtrl::CimgTcpCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: ��֤���Ŀؼ��Ƿ���ϵ�Ԫģ���̴߳������
	// �йظ�����Ϣ����ο� MFC ����˵�� 64��
	// ������Ŀؼ������ϵ�Ԫģ�͹�����
	// �����޸����´��룬��������������
	// afxRegApartmentThreading ��Ϊ 0��

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_IMGTCP,
			IDB_IMGTCP,
			afxRegApartmentThreading,
			_dwimgTcpOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CimgTcpCtrl::CimgTcpCtrl - ���캯��

CimgTcpCtrl::CimgTcpCtrl()
{
	InitializeIIDs(&IID_DimgTcp, &IID_DimgTcpEvents);
	// TODO: �ڴ˳�ʼ���ؼ���ʵ�����ݡ�
	m_strIp = "";
	m_strPort = "";
	m_nHeadLength = NUM_JSON + NUM_IMG + NUM_MARK;  //json����5,img����10,��־λ����8
	m_bIpAndPort = false;
	m_strLastError = "";
	m_lTimeoutMillisecond = 90 * 1000; //����
	TrcErrInit("ImgTcpOcx");
}



// CimgTcpCtrl::~CimgTcpCtrl - ��������

CimgTcpCtrl::~CimgTcpCtrl()
{
	// TODO: �ڴ�����ؼ���ʵ�����ݡ�
}



// CimgTcpCtrl::OnDraw - ��ͼ����

void CimgTcpCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: �����Լ��Ļ�ͼ�����滻����Ĵ��롣
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CimgTcpCtrl::DoPropExchange - �־���֧��

void CimgTcpCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Ϊÿ���־õ��Զ������Ե��� PX_ ������
}



// CimgTcpCtrl::OnResetState - ���ؼ�����ΪĬ��״̬

void CimgTcpCtrl::OnResetState()
{
	COleControl::OnResetState();  // ���� DoPropExchange ���ҵ���Ĭ��ֵ

	// TODO: �ڴ��������������ؼ�״̬��
}



// CimgTcpCtrl::AboutBox - ���û���ʾ�����ڡ���

void CimgTcpCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_IMGTCP);
	dlgAbout.DoModal();
}



string	cis_Bytes2HexString( const string &str ) 
{
	string res ;
	char	stand[] = "0123456789ABCDEF" ;
	for( size_t i=0; i<str.size(); i++ )
	{
		// ��4bit
		char index = (str[i] >> 4) & 0x0F ;
		res.push_back( stand[index] ) ;

		// ��4bit
		index = str[i]  & 0x0F ;
		res.push_back( stand[index] ) ;
	}
	return res ;
}


string	cis_HexString2Bytes( const string &str ) 
{
	string	res ;
	size_t	nBegin = 0 ;
	int		nTemp ;
	char	szBuf[16] = {0} ;
	if( str.size() % 2 != 0 )
	{
		szBuf[0] = '0' ;
		szBuf[1] = str[0] ;
		sscanf(szBuf,"%x" ,&nTemp) ;
		nBegin = 1 ;
		res.push_back( (char)nTemp ) ;
	}
	for(; nBegin<str.size(); nBegin+=2 )
	{
		szBuf[0] = str[nBegin] ;
		szBuf[1] = str[nBegin+1] ;
		sscanf( szBuf,"%x" ,&nTemp ) ;
		res.push_back( (char)nTemp ) ;
	}
	return res ;
}



// CimgTcpCtrl ��Ϣ�������


BSTR CimgTcpCtrl::strImgTcp(LPCTSTR strJsonLength, LPCTSTR strJson, LPCTSTR strImgPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult = "-1";

	// TODO: �ڴ���ӵ��ȴ���������

	CFile file;
	char *szImg = NULL;
	
	ACE_INET_Addr addr ;//(31001,"10.10.10.10");
	ACE_SOCK_Stream     stream;
	ACE_SOCK_Connector  connector;

	Json::Reader jsonReader;
	Json::Value jsonValue;
	
	do 
	{
		if (!m_bIpAndPort)
		{
			TrcWritef(EC_INFO,"ImgTcpOcx δ����IP�Ͷ˿ں�");
			m_strLastError = "δ����IP�Ͷ˿ں�";
			break;
		}

		CString json = strJson;
		if (json.IsEmpty())
		{
			TrcWritef(EC_INFO,"ImgTcpOcx json����Ϊ��");
			m_strLastError = "json����Ϊ��";
			break;
		}
		
		if ( !jsonReader.parse(strJson, jsonValue) )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx Json����ʧ��");
			m_strLastError = "json����ʧ��";
			break;
		}
		
		int nImgLen = 0;

		CString imgPath = strImgPath;
		if (!imgPath.IsEmpty())
		{
			if (!file.Open(imgPath,CFile::modeRead))
			{
				TrcWritef(EC_INFO,"ImgTcpOcx ͼƬ��ʧ��");
				m_strLastError = "ͼƬ��ʧ��";
				break;
			}

			nImgLen = file.GetLength();

			szImg = new char[nImgLen+1];
			szImg[nImgLen] = '0';
			if ( 0 >= file.Read(szImg,nImgLen) )
			{
				TrcWritef(EC_INFO,"ImgTcpOcx ͼƬ��ȡʧ��");
				m_strLastError = "ͼƬ��ȡʧ��";
				break;
			}
		}
		CString strImgLen;

		char szImgLen[32] = {0};
		sprintf(szImgLen,"%d",nImgLen);
		jsonValue["SUN_FILE_SIZE"] = string(szImgLen);
		json = jsonValue.toStyledString().c_str();

		strImgLen = CString(szImgLen);
		/*
		CString strFind = "SUN_FILE_SIZE\":\"";
		int nFindPos = json.Find(strFind) + strFind.GetLength();
		
		strImgLen.Format("%d",nImgLen);
		json.Insert(nFindPos,strImgLen);
		*/
		TrcWritef(EC_INFO,"ImgTcpOcx strImgLen = %s",strImgLen);
		
		//TrcWritef(EC_INFO,"ImgTcpOcx json = %s",json);
		
		int nJsonLen = atoi(strJsonLength);
		if (nJsonLen == 0)
		{
			nJsonLen = CString(strJson).GetLength();
		}
		nJsonLen += strImgLen.GetLength();


		CString jsonLength;
		jsonLength.Format(_T("%05d"),nJsonLen);

		CString imgLength;
		imgLength.Format(_T("%010d"),nImgLen);

		CString strMark;
		strMark = "procbank";

		CString sendData;
		sendData = jsonLength + imgLength + strMark + json;
		
		
		addr.set(atoi(m_strPort.c_str()),m_strIp.c_str());
		ACE_Time_Value  tv(m_lTimeoutMillisecond/1000, MAX_CONN_TIMEOUT_MILLISECOND);

		int nConn = connector.connect(stream, addr, &tv);

		if ( nConn == -1 )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx ���ӷ�������ͨ ip = %s   port = %s", m_strIp.c_str(), m_strPort.c_str());
			m_strLastError = "���ӷ�������ͨ";
			break;
		}

		if ( !stream.send_n( sendData,sendData.GetLength()) )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx �������ݲ�ȫ");
			m_strLastError = "�������ݲ�ȫ";
			break;
		}
		TrcWritef(EC_INFO,"ImgTcpOcx send data = %s",sendData);

		if ( nImgLen != 0 )
		{
			if( !stream.send_n(szImg,nImgLen,&tv) )
			{
				TrcWritef(EC_INFO,"ImgTcpOcx ����ͼƬ����ʧ��");
				m_strLastError = "����ͼƬ����ʧ��";
				break;
			}
			TrcWritef(EC_INFO,"ImgTcpOcx send img path = %s",strImgPath);
		}
		

		char head[32] = {0};

		int nRecv = stream.recv_n( head,m_nHeadLength,&tv );
		//TrcWritef(EC_INFO,"ImgTcpOcx stream recv_n = %d", nRecv);
		if ( -1 == nRecv  )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx ���ճ�ʱ");
			m_strLastError = "���ճ�ʱ";
			break;
		}
		else if ( 0 == nRecv  )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx ���ձ���ͷλʧ��");
			m_strLastError = "���ձ���ͷλʧ��";
			break;
		}

		

		CString strHead(head,m_nHeadLength);
		
		int nRecvJsonLen = atoi(strHead.Left(NUM_JSON));

		if (nRecvJsonLen <= 0)
		{
			TrcWritef(EC_INFO,"ImgTcpOcx ����ͷ����ȷ");
			m_strLastError = "����ͷ����ȷ";
			break;
		}

		CString strRecvMark = strHead.Mid(NUM_JSON+NUM_IMG,NUM_MARK);
		if (0 != strMark.Compare(strRecvMark))
		{
			TrcWritef(EC_INFO,"ImgTcpOcx ���ձ�־λ����ȷ ��־λ = %s", strRecvMark);
			m_strLastError = "���ձ�־λ����ȷ";
			break;
		}

		char * pRecvJson = new char [nRecvJsonLen+1];
		pRecvJson[nRecvJsonLen] = '0';

		if (!stream.recv_n( pRecvJson,nRecvJsonLen,&tv))
		{
			delete pRecvJson;
			TrcWritef(EC_INFO,"ImgTcpOcx ���ͱ�����ʧ��");
			m_strLastError = "���ͱ�����ʧ��";
			break;
		}
		
		CString strRecvJson( pRecvJson ,nRecvJsonLen );
		
		strResult = strHead + strRecvJson;
		TrcWritef(EC_INFO,"ImgTcpOcx recv data = %s",strResult);
		delete []pRecvJson;

		/*
		int nRecvImgLen = atoi(strHead.Mid(NUM_JSON,NUM_IMG));
		if (nRecvImgLen <= 0)
		{
			break;
		}

		char * pRecvImg = new char[nRecvImgLen];
		pRecvImg[nRecvImgLen] = '0';

		if (!stream.recv_n( pRecvImg,nRecvImgLen,&tv))
		{
			delete pRecvImg;
			//���ͱ�����ʧ��
			break;
		}

		CString writeImg = "e:/write.bmp";
		CFile file2;
		file2.Open(writeImg,CFile::modeCreate|CFile::modeWrite);
		file2.Write(pRecvImg,nRecvImgLen);
		file2.Flush();
		file2.Close();

		delete pRecvImg;
		*/
		
	} while (0);
	if (NULL != szImg)
	{
		delete []szImg;
	}
	
	file.Close();
	stream.close();

	return strResult.AllocSysString();
}


BSTR CimgTcpCtrl::strSetIpAndPort(LPCTSTR strIp, LPCTSTR strPort)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	m_bIpAndPort = false;
	strResult = "-1";

	// TODO: �ڴ���ӵ��ȴ���������
	do 
	{
		string ip = (LPCSTR)strIp;
		if (ip == "")
		{
			m_strLastError = "ip����Ϊ��";
			break;
		}

		string port = (LPCSTR)strPort;
		if (port == "")
		{
			m_strLastError = "�˿ں�����Ϊ��";
			break;
		}

		m_strIp = ip;
		m_strPort = port;

		m_bIpAndPort = true;

		strResult = "0";

	} while (0);
	

	return strResult.AllocSysString();
}


BSTR CimgTcpCtrl::strGetLastError(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: �ڴ���ӵ��ȴ���������
	strResult = m_strLastError.c_str();

	return strResult.AllocSysString();
}


BSTR CimgTcpCtrl::strSetTimeout(LPCTSTR strMillisecond)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = "0";

	// TODO: �ڴ���ӵ��ȴ���������
	m_lTimeoutMillisecond = atoi(strMillisecond);

	return strResult.AllocSysString();
}
