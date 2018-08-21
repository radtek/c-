// CommAxCtrl.cpp : CCommAxCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "CommAx.h"
#include "CommAxCtrl.h"
#include "CommAxPropPage.h"
#include "afxdialogex.h"

#pragma warning(disable: 4996)

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"



#define MAX_CONN_TIMEOUT_SECOND      3 /*TCP connection timeout in second.*/
#define MAX_CONN_TIMEOUT_MILLISECOND 0 /*TCP connection timeout in ms.*/
#define MAX_RECV_TIMEOUT_SECOND      1 /*TCP connection timeout in second.*/
#define MAX_RECV_TIMEOUT_MILLISECOND 0 /*TCP connection timeout in ms.*/

#ifdef _DEBUG
#pragma comment(lib, "../Lib/ACE.lib")
#pragma comment(lib, "Common.lib")
#else
#pragma comment(lib, "ACE.lib")
#pragma comment(lib, "Common.lib")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CCommAxCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CCommAxCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CCommAxCtrl, COleControl)
	DISP_FUNCTION_ID(CCommAxCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CCommAxCtrl, "NETSendAndReceive", dispidNETSendAndReceive, NETSendAndReceive, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CCommAxCtrl, "NETSendAndReceiveImg", dispidNETSendAndReceiveImg, NETSendAndReceiveImg, VT_BSTR, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CCommAxCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CCommAxCtrl, 1)
	PROPPAGEID(CCommAxPropPage::guid)
END_PROPPAGEIDS(CCommAxCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CCommAxCtrl, "COMMAX.CommAxCtrl.1",
	0x23bcc422, 0x10ab, 0x479a, 0xb4, 0xc7, 0xc7, 0x25, 0x87, 0x4b, 0xb1, 0x7)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CCommAxCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DCommAx = { 0x4F628536, 0xE5DD, 0x4DA0, { 0x83, 0x41, 0xE8, 0xE4, 0xB7, 0xA2, 0x2D, 0x95 } };
const IID IID_DCommAxEvents = { 0x27131751, 0x8E45, 0x499F, { 0x90, 0x3E, 0x83, 0x61, 0xF2, 0x66, 0x7F, 0x1A } };


// 控件类型信息

static const DWORD _dwCommAxOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCommAxCtrl, IDS_COMMAX, _dwCommAxOleMisc)



// CCommAxCtrl::CCommAxCtrlFactory::UpdateRegistry -
// 添加或移除 CCommAxCtrl 的系统注册表项

BOOL CCommAxCtrl::CCommAxCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_COMMAX,
			IDB_COMMAX,
			afxRegApartmentThreading,
			_dwCommAxOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CCommAxCtrl::CCommAxCtrl - 构造函数

CCommAxCtrl::CCommAxCtrl()
{
	InitializeIIDs(&IID_DCommAx, &IID_DCommAxEvents);
	m_bIncludeSelf = true;
	// TODO: 在此初始化控件的实例数据。
}



// CCommAxCtrl::~CCommAxCtrl - 析构函数

CCommAxCtrl::~CCommAxCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CCommAxCtrl::OnDraw - 绘图函数

void CCommAxCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CCommAxCtrl::DoPropExchange - 持久性支持

void CCommAxCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CCommAxCtrl::OnResetState - 将控件重置为默认状态

void CCommAxCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CCommAxCtrl::AboutBox - 向用户显示“关于”框

void CCommAxCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_COMMAX);
	dlgAbout.DoModal();
}



// CCommAxCtrl 消息处理程序

string	CCommAxCtrl::cis_Bytes2HexString( const string &str ) 
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



int		CCommAxCtrl::CalcNetDataLength( const string &strHead ) 
{
	
	int nLength = 0 ;
	if( _stricmp( m_HeadCode.c_str() , "ascii" ) == 0 )	//ascii
	{
		nLength = atoi( strHead.c_str() ) ;
	}
	else if( _stricmp( m_HeadCode.c_str() , "bcd" ) == 0 ) //bcd
	{
		nLength	= atoi( cis_Bytes2HexString(strHead).c_str()) ;
	}
	else if( _stricmp( m_HeadCode.c_str() , "bin" ) == 0 )//bin
	{
		memcpy( &nLength , strHead.data() , min( sizeof(nLength) , strHead.size() ) ) ;
	}
	else if( _stricmp( m_HeadCode.c_str() , "nbin" ) == 0 )
	{
		string strTemp = strHead ;
		reverse( strTemp.begin() , strTemp.end() ) ;
		memcpy( &nLength , strTemp.data() , min( sizeof(nLength) , strTemp.size() ) ) ;
	}
	if( m_bIncludeSelf )
	{
		nLength -= m_HeadLength ;//减去头自身的长度
	}
	
	if( nLength <= 0 )
	{
		TrcWritef( EC_WARN , "parse head<%s> failed,result = %d" , strHead.c_str() , nLength ) ;
	}
	
	return nLength ;
}

string		CCommAxCtrl::cis_HexString2Bytes( const string &str ) 
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


string  CCommAxCtrl::GenerateHead(int nDataLen)
{
	
	int nPacklen ;

	if (m_bIncludeSelf)
	{
		nPacklen = nDataLen  + m_HeadLength;
	}
	else
	{
		nPacklen = nDataLen;
	}

	char	szFmt[32] = {0};
	string	strRet;

	if( _stricmp( m_HeadCode.c_str() , "ascii" ) == 0 )	//ascii
	{
		sprintf_s(szFmt,"%0*d",min(m_HeadLength,31),nPacklen);
		strRet = szFmt ;
	}
	else if( _stricmp( m_HeadCode.c_str() , "bcd" ) == 0 ) //bcd
	{
		sprintf_s(szFmt,"%0*d",min(m_HeadLength*2,31),nPacklen);
		strRet = cis_HexString2Bytes( szFmt ) ;		
	}
	else if( _stricmp( m_HeadCode.c_str() , "bin" ) == 0 )//bin
	{
		memcpy(szFmt,&nPacklen,min(m_HeadLength,4));
		for (int i = 0 ; i < m_HeadLength; i ++)
		{
			strRet.push_back( szFmt[i]);
		}
	}
	else if( _stricmp( m_HeadCode.c_str() , "nbin" ) == 0 )
	{
		memcpy(szFmt,&nPacklen,min(m_HeadLength,4));
		for (int i = 0 ; i < m_HeadLength; i ++)
		{
			strRet.push_back( szFmt[m_HeadLength-1-i]);
		}
	}

	return strRet;
}

string	CCommAxCtrl::cis_Utf162Ansi(const wstring& strUtf16)
{
	if(strUtf16.size() == 0)
		return string();

	int len = WideCharToMultiByte(CP_ACP, 0, strUtf16.c_str(), -1, NULL, 0, NULL, NULL);
	char *szAnsi = new char[len + 1]; 
	memset(szAnsi, 0, len + 1); 
	WideCharToMultiByte (CP_ACP, 0, strUtf16.c_str(), -1, (LPSTR)szAnsi, len, NULL,NULL);

	std::string result(szAnsi);
	delete[] szAnsi;
	return result;
}
string		CCommAxCtrl::cis_Utf82Ansi(const string& strUtf8)
{
	if (strUtf8.size() < 1)
	{
		return string();
	}

	int len = MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, NULL, 0);
	wchar_t* wszUtf16 = new wchar_t[len+1]; 
	memset(wszUtf16, 0, (len +1)*sizeof(wchar_t));  
	MultiByteToWideChar(CP_UTF8, 0, strUtf8.c_str(), -1, (LPWSTR)wszUtf16, len);
	
	string sAnsi = cis_Utf162Ansi(wstring((LPCWSTR)wszUtf16));
	delete[] wszUtf16;
	return sAnsi;
} 

BSTR CCommAxCtrl::NETSendAndReceive(LPCTSTR strJson)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	Json::Reader reader;
	Json::Value jsValue;
	string ip;
	string port;
	string data;
	string strInclude;
	string strTimeout;
	ACE_INET_Addr addr ;//(31001,"10.10.10.10");
	ACE_SOCK_Stream     stream;
	ACE_SOCK_Connector  connector;
	string strReceive;
	Json::Value ret;
	ret["result"] = "failure";
	ret["detail"] = "";
	do{

		if ( !reader.parse(strJson, jsValue) )
		{
			TrcWritef(EC_INFO,"Json Para Wrong for NETSendAndReceive");
			ret["detail"] = "Json Para Wrong for NETSendAndReceive";
			break;
		}
		ip				= jsValue["ip"].asString() ; 
		port			= jsValue["port"].asString() ; 
		m_HeadCode		= jsValue["headercode"].asString() ; 
		m_HeadLength	= atoi(jsValue["headerlength"].asString().c_str()) ; 
		strInclude		= jsValue["strInclude"].asString() ; 
		m_bIncludeSelf  = ( strInclude == string("true") ? true : false ) ;
		data			= jsValue["data"].asString() ;  
		strTimeout		= jsValue["Timeout"].asString() ; 
		string header	= GenerateHead(data.size());

		string senddata =  header+ data;

		addr.set(atoi(port.c_str()),ip.c_str()) ; 

		int iTimeout = atoi(strTimeout.c_str()); // 超时

		ACE_Time_Value  tv(iTimeout, MAX_CONN_TIMEOUT_MILLISECOND);

		if ( ! connector.connect(stream, addr, &tv) == -1 )
		{
			TrcWritef(EC_INFO,"NETSend 连接服务器不通 ,INET_SEND_ERROR");
			ret["detail"] = "INET_SEND_ERROR";
			break;
		}

		if ( ! stream.send_n( senddata.c_str(),senddata.length())  )
		{
			TrcWritef(EC_INFO,"NETSend 发送数据不全 ,INET_SEND_ERROR");
			ret["detail"] = "INET_SEND_ERROR";
			break;
		}

		char head[32] = {0};

		if ( ! stream.recv_n( head,m_HeadLength,&tv )  )
		{

			TrcWritef(EC_INFO,"NETSend 接送报文头位失败 ,INET_RECEIVE_ERROR");
			ret["detail"] = "INET_RECEIVE_ERROR";
			break;
		}

		TrcWritef(EC_INFO,"header = %s",head);
		int nLength = CalcNetDataLength ( head ) ;
		TrcWritef(EC_INFO,"NETSend 要接收报文的长度=%d",nLength);
		char * pReceive = new char [nLength] ;

		if ( ! stream.recv_n( pReceive,nLength,&tv))
		{
			delete pReceive;
			TrcWritef(EC_INFO,"NETSend 接送报文体失败 ,INET_RECEIVE_ERROR");
			ret["detail"] = "INET_RECEIVE_ERROR";
			break;
		}

		strReceive = string( pReceive ,nLength );
		ret["result"] = "success";
		ret["detail"] = cis_Utf82Ansi(strReceive).c_str();

		TrcWritef(EC_INFO,"NETSend 接收的报文 =%s",strReceive.c_str());
		delete pReceive;

		HANDLE              fh;         

		fh = CreateFile("c:\\test.xml", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);      

		if (fh == INVALID_HANDLE_VALUE)      
		{    
			break;
		}

		DWORD lpNum = 1;
		WriteFile(fh, strReceive.c_str(),strReceive.size(), &lpNum, NULL); 

		CloseHandle(fh); 

		
	}while(false);


	stream.close_writer();
	stream.close_reader();
	stream.close();

	strResult = ret.toStyledString().c_str();
	return strResult.AllocSysString();
}


BSTR CCommAxCtrl::NETSendAndReceiveImg(LPCTSTR strJson, LPCTSTR strImgPath)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 在此添加调度处理程序代码
	//Json::Reader reader;
	//Json::Value jsValue;
	//string ip;
	//string port;
	//string data;
	//string strInclude;
	//string strTimeout;
	//ACE_INET_Addr addr ;//(31001,"10.10.10.10");
	//ACE_SOCK_Stream     stream;
	//ACE_SOCK_Connector  connector;
	//string strReceive;
	//Json::Value ret;
	//ret["result"] = "failure";
	//ret["detail"] = "";
	//do{

	//	if ( !reader.parse(strJson, jsValue) )
	//	{
	//		TrcWritef(EC_INFO,"Json Para Wrong for NETSendAndReceive");
	//		ret["detail"] = "Json Para Wrong for NETSendAndReceive";
	//		break;
	//	}
	//	/*ip				= jsValue["ip"].asString() ; 
	//	port			= jsValue["port"].asString() ; 
	//	m_HeadCode		= jsValue["headercode"].asString() ; 
	//	m_HeadLength	= atoi(jsValue["headerlength"].asString().c_str()) ; 
	//	strInclude		= jsValue["strInclude"].asString() ; 
	//	m_bIncludeSelf  = ( strInclude == string("true") ? true : false ) ;
	//	data			= jsValue["data"].asString() ;  
	//	strTimeout		= jsValue["Timeout"].asString() ; */
	//	string header	= GenerateHead(data.size());

	//	string senddata =  header+ data;

	//	addr.set(atoi(port.c_str()),ip.c_str()) ; 

	//	int iTimeout = atoi(strTimeout.c_str()); // 超时

	//	ACE_Time_Value  tv(iTimeout, MAX_CONN_TIMEOUT_MILLISECOND);

	//	if ( ! connector.connect(stream, addr, &tv) == -1 )
	//	{
	//		TrcWritef(EC_INFO,"NETSend 连接服务器不通 ,INET_SEND_ERROR");
	//		ret["detail"] = "INET_SEND_ERROR";
	//		break;
	//	}

	//	if ( ! stream.send_n( senddata.c_str(),senddata.length())  )
	//	{
	//		TrcWritef(EC_INFO,"NETSend 发送数据不全 ,INET_SEND_ERROR");
	//		ret["detail"] = "INET_SEND_ERROR";
	//		break;
	//	}

	//	char head[32] = {0};

	//	if ( ! stream.recv_n( head,m_HeadLength,&tv )  )
	//	{

	//		TrcWritef(EC_INFO,"NETSend 接送报文头位失败 ,INET_RECEIVE_ERROR");
	//		ret["detail"] = "INET_RECEIVE_ERROR";
	//		break;
	//	}

	//	TrcWritef(EC_INFO,"header = %s",head);
	//	int nLength = CalcNetDataLength ( head ) ;
	//	TrcWritef(EC_INFO,"NETSend 要接收报文的长度=%d",nLength);
	//	char * pReceive = new char [nLength] ;

	//	if ( ! stream.recv_n( pReceive,nLength,&tv))
	//	{
	//		delete pReceive;
	//		TrcWritef(EC_INFO,"NETSend 接送报文体失败 ,INET_RECEIVE_ERROR");
	//		ret["detail"] = "INET_RECEIVE_ERROR";
	//		break;
	//	}

	//	strReceive = string( pReceive ,nLength );
	//	ret["result"] = "success";
	//	ret["detail"] = cis_Utf82Ansi(strReceive).c_str();

	//	TrcWritef(EC_INFO,"NETSend 接收的报文 =%s",strReceive.c_str());
	//	delete pReceive;

	//	HANDLE              fh;         

	//	fh = CreateFile("c:\\test.xml", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);      

	//	if (fh == INVALID_HANDLE_VALUE)      
	//	{    
	//		break;
	//	}

	//	DWORD lpNum = 1;
	//	WriteFile(fh, strReceive.c_str(),strReceive.size(), &lpNum, NULL); 

	//	CloseHandle(fh); 


	//}while(false);


	//stream.close_writer();
	//stream.close_reader();
	//stream.close();

	//strResult = ret.toStyledString().c_str();

	return strResult.AllocSysString();
}
