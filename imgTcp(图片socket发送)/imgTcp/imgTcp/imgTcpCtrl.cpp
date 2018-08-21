// imgTcpCtrl.cpp : CimgTcpCtrl ActiveX 控件类的实现。

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



// 消息映射

BEGIN_MESSAGE_MAP(CimgTcpCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CimgTcpCtrl, COleControl)
	DISP_FUNCTION_ID(CimgTcpCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strImgTcp", dispidstrImgTcp, strImgTcp, VT_BSTR, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strSetIpAndPort", dispidstrSetIpAndPort, strSetIpAndPort, VT_BSTR, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strGetLastError", dispidstrGetLastError, strGetLastError, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CimgTcpCtrl, "strSetTimeout", dispidstrSetTimeout, strSetTimeout, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CimgTcpCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CimgTcpCtrl, 1)
	PROPPAGEID(CimgTcpPropPage::guid)
END_PROPPAGEIDS(CimgTcpCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CimgTcpCtrl, "IMGTCP.imgTcpCtrl.1",
	0x285d5807, 0x9271, 0x4783, 0xb7, 0x37, 0x75, 0xf1, 0xc5, 0x51, 0xad, 0xf2)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CimgTcpCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DimgTcp = { 0x61F94D57, 0x91C1, 0x4332, { 0x8F, 0x4F, 0xE1, 0x3C, 0x90, 0xE2, 0xD, 0x5D } };
const IID IID_DimgTcpEvents = { 0xBFD2A219, 0x2DB9, 0x4C4D, { 0x9D, 0x37, 0x5D, 0xC0, 0x2D, 0xE0, 0x2C, 0x7B } };


// 控件类型信息

static const DWORD _dwimgTcpOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CimgTcpCtrl, IDS_IMGTCP, _dwimgTcpOleMisc)



// CimgTcpCtrl::CimgTcpCtrlFactory::UpdateRegistry -
// 添加或移除 CimgTcpCtrl 的系统注册表项

BOOL CimgTcpCtrl::CimgTcpCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

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



// CimgTcpCtrl::CimgTcpCtrl - 构造函数

CimgTcpCtrl::CimgTcpCtrl()
{
	InitializeIIDs(&IID_DimgTcp, &IID_DimgTcpEvents);
	// TODO: 在此初始化控件的实例数据。
	m_strIp = "";
	m_strPort = "";
	m_nHeadLength = NUM_JSON + NUM_IMG + NUM_MARK;  //json长度5,img长度10,标志位长度8
	m_bIpAndPort = false;
	m_strLastError = "";
	m_lTimeoutMillisecond = 90 * 1000; //毫秒
	TrcErrInit("ImgTcpOcx");
}



// CimgTcpCtrl::~CimgTcpCtrl - 析构函数

CimgTcpCtrl::~CimgTcpCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CimgTcpCtrl::OnDraw - 绘图函数

void CimgTcpCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CimgTcpCtrl::DoPropExchange - 持久性支持

void CimgTcpCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CimgTcpCtrl::OnResetState - 将控件重置为默认状态

void CimgTcpCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CimgTcpCtrl::AboutBox - 向用户显示“关于”框

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
		// 高4bit
		char index = (str[i] >> 4) & 0x0F ;
		res.push_back( stand[index] ) ;

		// 低4bit
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



// CimgTcpCtrl 消息处理程序


BSTR CimgTcpCtrl::strImgTcp(LPCTSTR strJsonLength, LPCTSTR strJson, LPCTSTR strImgPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	strResult = "-1";

	// TODO: 在此添加调度处理程序代码

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
			TrcWritef(EC_INFO,"ImgTcpOcx 未设置IP和端口号");
			m_strLastError = "未设置IP和端口号";
			break;
		}

		CString json = strJson;
		if (json.IsEmpty())
		{
			TrcWritef(EC_INFO,"ImgTcpOcx json传入为空");
			m_strLastError = "json传入为空";
			break;
		}
		
		if ( !jsonReader.parse(strJson, jsonValue) )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx Json解析失败");
			m_strLastError = "json解析失败";
			break;
		}
		
		int nImgLen = 0;

		CString imgPath = strImgPath;
		if (!imgPath.IsEmpty())
		{
			if (!file.Open(imgPath,CFile::modeRead))
			{
				TrcWritef(EC_INFO,"ImgTcpOcx 图片打开失败");
				m_strLastError = "图片打开失败";
				break;
			}

			nImgLen = file.GetLength();

			szImg = new char[nImgLen+1];
			szImg[nImgLen] = '0';
			if ( 0 >= file.Read(szImg,nImgLen) )
			{
				TrcWritef(EC_INFO,"ImgTcpOcx 图片读取失败");
				m_strLastError = "图片读取失败";
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
			TrcWritef(EC_INFO,"ImgTcpOcx 连接服务器不通 ip = %s   port = %s", m_strIp.c_str(), m_strPort.c_str());
			m_strLastError = "连接服务器不通";
			break;
		}

		if ( !stream.send_n( sendData,sendData.GetLength()) )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx 发送数据不全");
			m_strLastError = "发送数据不全";
			break;
		}
		TrcWritef(EC_INFO,"ImgTcpOcx send data = %s",sendData);

		if ( nImgLen != 0 )
		{
			if( !stream.send_n(szImg,nImgLen,&tv) )
			{
				TrcWritef(EC_INFO,"ImgTcpOcx 发送图片数据失败");
				m_strLastError = "发送图片数据失败";
				break;
			}
			TrcWritef(EC_INFO,"ImgTcpOcx send img path = %s",strImgPath);
		}
		

		char head[32] = {0};

		int nRecv = stream.recv_n( head,m_nHeadLength,&tv );
		//TrcWritef(EC_INFO,"ImgTcpOcx stream recv_n = %d", nRecv);
		if ( -1 == nRecv  )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx 接收超时");
			m_strLastError = "接收超时";
			break;
		}
		else if ( 0 == nRecv  )
		{
			TrcWritef(EC_INFO,"ImgTcpOcx 接收报文头位失败");
			m_strLastError = "接收报文头位失败";
			break;
		}

		

		CString strHead(head,m_nHeadLength);
		
		int nRecvJsonLen = atoi(strHead.Left(NUM_JSON));

		if (nRecvJsonLen <= 0)
		{
			TrcWritef(EC_INFO,"ImgTcpOcx 数据头不正确");
			m_strLastError = "数据头不正确";
			break;
		}

		CString strRecvMark = strHead.Mid(NUM_JSON+NUM_IMG,NUM_MARK);
		if (0 != strMark.Compare(strRecvMark))
		{
			TrcWritef(EC_INFO,"ImgTcpOcx 接收标志位不正确 标志位 = %s", strRecvMark);
			m_strLastError = "接收标志位不正确";
			break;
		}

		char * pRecvJson = new char [nRecvJsonLen+1];
		pRecvJson[nRecvJsonLen] = '0';

		if (!stream.recv_n( pRecvJson,nRecvJsonLen,&tv))
		{
			delete pRecvJson;
			TrcWritef(EC_INFO,"ImgTcpOcx 接送报文体失败");
			m_strLastError = "接送报文体失败";
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
			//接送报文体失败
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

	// TODO: 在此添加调度处理程序代码
	do 
	{
		string ip = (LPCSTR)strIp;
		if (ip == "")
		{
			m_strLastError = "ip设置为空";
			break;
		}

		string port = (LPCSTR)strPort;
		if (port == "")
		{
			m_strLastError = "端口号设置为空";
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

	// TODO: 在此添加调度处理程序代码
	strResult = m_strLastError.c_str();

	return strResult.AllocSysString();
}


BSTR CimgTcpCtrl::strSetTimeout(LPCTSTR strMillisecond)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult = "0";

	// TODO: 在此添加调度处理程序代码
	m_lTimeoutMillisecond = atoi(strMillisecond);

	return strResult.AllocSysString();
}
