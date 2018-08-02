// Camera.cpp : CCameraApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "Camera.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCameraApp theApp;

const GUID CDECL _tlid = { 0x268B4873, 0x48B5, 0x4E89, { 0xA5, 0xB3, 0x42, 0xA2, 0xB5, 0x38, 0x3C, 0x7B } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str()); 

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, d);
	}

	return res;
}

// CCameraApp::InitInstance - DLL 初始化

BOOL CCameraApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。

		m_bStartCamera = false;
		m_bInit = false;
		m_bThreadShowCamera = false;
		m_strPicturePath = "";
		m_nCameraIndexColor = 0;
		m_nPicMaxWidth = 800;
		m_nPicMaxHeight = 600;
		m_strCameraWindow = "CAMERA_WINDOW";
		//namedWindow(m_strCameraWindow);
		m_nWindowX = 0;
		m_nWindowY = 0;
		m_nWindowWidth = 400;
		m_nWindowHeight = 300;

	}

	return bInit;
}



// CCameraApp::ExitInstance - DLL 终止

int CCameraApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	destroyWindow(m_strCameraWindow);

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}


LONG CCameraApp::StartCamera()
{
	//CLogger::Instance()->TraceInfo("===============StartCamera start===============");
	
	LONG lRet = -1;
	do 
	{
		if ( !m_bInit )
		{
			if ( 0 != nInit() )
			{
				lRet = -2;
				//CLogger::Instance()->TraceInfo("StartCamera:0 != nInit");
				break;
			}

			m_bInit = true;
		}
		
		showWindow(true);

		if ( m_bStartCamera )
		{
			lRet = -3;
			//CLogger::Instance()->TraceInfo("StartCamera:m_bStartCamera");
			break;
		}

		if( !m_capColor.open(m_nCameraIndexColor) && !m_capColor.isOpened() )
		{
			lRet = -4;
			//CLogger::Instance()->TraceInfo("StartCamera:m_capColor.open failed");
			break;
		}

		if ( !bStartShowCamera() )
		{
			lRet = -6;
			//CLogger::Instance()->TraceInfo("StartCamera: !bStartShowCamera()");
			break;
		}

		m_bStartCamera = true;
		lRet = 0;

	} while (0);

	//CLogger::Instance()->TraceInfo("===============StartCamera end return<%d>===============",lRet);
	return lRet;
}


bool CCameraApp::bStartShowCamera()
{
	m_bThreadShowCamera = true;
	m_handleThreadShowCamera = (HANDLE)::_beginthreadex(0, 0, ThreadFuncStartShowCamera, (void*)this, 0, & m_dwThreadShowCameraId );

	//CLogger::Instance()->TraceInfo("bStartShowCamera: m_dwThreadShowCameraId");
	if (m_handleThreadShowCamera == INVALID_HANDLE_VALUE)
	{
		//CLogger::Instance()->TraceFatal("bStartShowCamera: StartShowCamera err code = %d",::GetLastError());
		m_bThreadShowCamera = false;
		return false;
	}

	return true ;
}

UINT _stdcall CCameraApp::ThreadFuncStartShowCamera( LPVOID	lpPara ) 
{
	try
	{
		//CLogger::Instance()->TraceInfo("ThreadFuncStartShowCamera: begin");
		CCameraApp *pHandle = ( CCameraApp * ) lpPara ;

		while( pHandle->m_bThreadShowCamera )
		{
			pHandle->ShowCamera();
			Sleep(30);

		}
		//CLogger::Instance()->TraceInfo("ThreadFuncStartShowCamera: end");
	}
	catch(...)
	{
		//CLogger::Instance()->TraceFatal("ThreadFuncStartShowCamera: thread exception!!!");
	}
	return 0 ;
}


void CCameraApp::ShowCamera()
{
	do 
	{
		m_capColor.read(m_imageColor);
		if(m_imageColor.empty())
		{
			//CLogger::Instance()->TraceInfo("ShowCamera: m_imageColor.empty()");
			break;
		}

		m_imageShow = m_imageColor.clone();

		if ( m_imageShow.empty() )
		{
			//CLogger::Instance()->TraceInfo("ShowCamera: m_imageShow.empty()");
			break;
		}

		imshow(m_strCameraWindow,m_imageShow);


	} while (0);

}


void CCameraApp::StopCamera(bool bWait)
{
	//CLogger::Instance()->TraceInfo("===============StopCamera start===============");
	LONG lRet = -1;
	do 
	{
		if ( !m_bInit )
		{
			//CLogger::Instance()->TraceInfo("StopCamera: !m_bInit");
			lRet = -2;
			break;
		}

		if ( !m_bStartCamera )
		{
			//CLogger::Instance()->TraceInfo("StopCamera: !m_bStartCamera");
			lRet = -3;
			break;
		}
		

		//关闭摄像头线程
		if (m_bThreadShowCamera)
		{
			m_bThreadShowCamera = false;
			if ( m_handleThreadShowCamera != INVALID_HANDLE_VALUE && bWait )
			{
				::PostThreadMessage( m_dwThreadShowCameraId , WM_QUIT,0,0 ) ;	//如果流程在执行事件管理器里的waitevent函数会自动终止

				DWORD dwRes = ::WaitForSingleObject( m_handleThreadShowCamera, 1000 );
				if( dwRes != WAIT_OBJECT_0 ) 
				{
					////TrcWritef( EC_WARN ,"RSHandler::Stop will be terminated, errcode = %d." , ::GetLastError() ) ;
					::TerminateThread( m_handleThreadShowCamera , -1 ) ;
				}
				m_handleThreadShowCamera = INVALID_HANDLE_VALUE ;
			}
		}

		showWindow(false);
		Destroy();

		m_bStartCamera = false;
		m_bInit = false;

		lRet = 0;

	} while (0);
	//CLogger::Instance()->TraceInfo("===============StopCamera end return<%d>===============",lRet);
}


int CCameraApp::nInit()
{
	int nRet = -1;
	do 
	{
		m_pBufferColor = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!m_pBufferColor)
		{
			nRet = -2;
			break;
		}

		namedWindow(m_strCameraWindow,0); //创建一个窗口 
		m_hParentWnd = ::GetParent((HWND)cvGetWindowHandle(m_strCameraWindow.c_str()));//获取父窗口HWND。父窗口是我们要用的
		//隐藏窗口标题栏 
		long style = GetWindowLong(m_hParentWnd,GWL_STYLE); 
		style &= ~(WS_CAPTION); 
		// style &= ~(WS_MAXIMIZEBOX); 
		SetWindowLong(m_hParentWnd,GWL_STYLE,style);
		::SetWindowPos(m_hParentWnd,HWND_TOPMOST,0,0,1000,1000,SWP_NOSIZE | SWP_NOMOVE); //修改窗口为最顶部
		::ShowWindow(m_hParentWnd,SW_HIDE);

		nRet = 0;
	} while (0);

	return nRet;
}


void CCameraApp::Destroy()
{
	//释放缓存
	if (m_pBufferColor)
	{
		free(m_pBufferColor);
		m_pBufferColor = NULL;
	}

	m_capColor.release();

	
}


CString CCameraApp::GetPicture()
{
	//CLogger::Instance()->TraceInfo("===============GetPicture start===============");
	CString strResult = "-1";
	string imgbase64 = "";
	do 
	{
		if (!m_bInit)
		{
			//CLogger::Instance()->TraceInfo("GetPicture: !m_bInit");
			break;
		}

		if (!m_bStartCamera)
		{
			//CLogger::Instance()->TraceInfo("GetPicture: !m_bStartCamera");
			break;
		}

		Mat img = m_imageColor.clone();

		if ( 0 != nFaceDetect(img) )
		{
			//CLogger::Instance()->TraceInfo("GetPicture: 0 != nFaceDetect(img)");
			break;
		}
		

		vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
		vector<int> vecCompression_params;
		vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
		vecCompression_params.push_back(90);
		imencode(".jpg", img, vecImg, vecCompression_params);

		Base64 base64;
		imgbase64 = base64.Encode(vecImg.data(), vecImg.size());     //实现图片的base64编码

		strResult = "0";


	} while (0);

	CString strRet = "{\"result\":" + strResult + ",\"base64\":\"" + imgbase64.c_str() + "\"}";

	//CLogger::Instance()->TraceInfo("GetPicture: base64 = %s",strRet);
	//CLogger::Instance()->TraceInfo("===============GetPicture end===============");

	return strRet;

}

void CCameraApp::SavePicturePath(LPCTSTR strPath)
{
	vector<string> vecPaths = split(strPath,"\\");
	string path = vecPaths[0];
	for(int i = 1; i < vecPaths.size(); i++)
	{
		path += "\\" + vecPaths[i];
		if ( access(path.c_str(),6) == -1 )
		{
			mkdir(path.c_str());
		}
	}
	m_strPicturePath = strPath;
}



int CCameraApp::nFaceDetect(Mat &mat)
{
	//CLogger::Instance()->TraceInfo("===============nFaceDetect start===============");
	int nRet = -1;
	do 
	{
		if(mat.empty())
		{
			//CLogger::Instance()->TraceInfo("nFaceDetect: mat.empty()");
			nRet = -2;
			break;
		}
		Mat gray;
		cvtColor(mat, gray, CV_BGR2GRAY);
		int * pResults = NULL; 
		int doLandmark = 1;
		//facedetect_frontal//正面
		//facedetect_frontal_surveillance
		//facedetect_multiview//多视角
		//facedetect_multiview_reinforce
		pResults = facedetect_frontal_surveillance(m_pBufferColor, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 2, 48, 0, doLandmark);
		//CLogger::Instance()->TraceInfo("%d faces detected.\n", (pResults ? *pResults : 0));

		if ( !(pResults && *pResults == 1))
		{
			//CLogger::Instance()->TraceInfo("nFaceDetect: !(pResultsBW && *pResultsBW == 1)");
			nRet = -3;
			break;
		}

		short * p = ((short*)(pResults + 1));
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];
		//rectangle(m_imageShow, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		//CLogger::Instance()->TraceInfo("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		
		int x0 = x + w/2;
		int y0 = y + h/2;

		if (m_nPicMaxWidth > w)
		{
			int widthLeft = x0 - m_nPicMaxWidth/2 > 0 ? m_nPicMaxWidth : 2 * x0;
			int widthRight = x0 + m_nPicMaxWidth/2 < mat.cols ? m_nPicMaxWidth : 2 * (mat.cols - x0);

			w = widthLeft < widthRight ? widthLeft : widthRight;
			x = x0 - w/2;
		}

		if (m_nPicMaxHeight > h)
		{
			int heightUp = y0 - m_nPicMaxHeight/2 > 0 ? m_nPicMaxHeight : 2 * y0;
			int heightDown = y0 + m_nPicMaxHeight/2 < mat.rows ? m_nPicMaxHeight : 2 * (mat.rows - y0);
			h = heightUp < heightDown ? heightUp : heightDown;
			y = y0 - h/2;
		}
		//CLogger::Instance()->TraceInfo("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		Rect rect(x,y,w,h);
		Mat matCut = mat(rect).clone();

		saveJpg(matCut);

		nRet = 0;
	} while (0);
	
	return nRet;
}

void CCameraApp::saveJpg(Mat &mat)
{
	if ( 0 != m_strPicturePath.length() )
	{
		//time_t curTime;
		//time(&curTime);
		//tm tm1;
		//localtime_s(&tm1, &curTime);
		//char picName[64] = {0};
		//sprintf(picName,"pic%04d%02d%02d_%02d%02d%02d.jpg",tm1.tm_year + 1900, tm1.tm_mon + 1, tm1.tm_mday, tm1.tm_hour, tm1.tm_min, tm1.tm_sec);

		char picName[]  = "picture.jpg";
		if (m_strPicturePath.substr(m_strPicturePath.length()-1,1) != "\\")
		{
			m_strPicturePath += "\\";
		}
		imwrite(m_strPicturePath + picName, mat);
	}
}

void CCameraApp::SetPicMaxWidthHeight(int width, int height)
{
	if (width <= 0 || height <= 0)
	{
		return;
	}
	m_nPicMaxWidth = width;
	m_nPicMaxHeight = height;
}

void CCameraApp::showWindow(bool bShow)
{
	/*
	HWND hWnd = (HWND)cvGetWindowHandle(m_strCameraWindow.c_str());//获取子窗口的HWND
	if (!hWnd)
	{
		namedWindow(m_strCameraWindow); //创建一个窗口 
		hWnd = (HWND)cvGetWindowHandle(m_strCameraWindow.c_str());//获取子窗口的HWND
	}
	HWND hParentWnd = ::GetParent(hWnd);//获取父窗口HWND。父窗口是我们要用的


	//改变窗口的位置和大小。这里主要前面的SetWindowPos不能改变位置和大小(为什么？)
	::MoveWindow(hParentWnd,m_nWindowX,m_nWindowY,m_nWindowWidth,m_nWindowHeight,0);

	//::SetWindowPos(hParentWnd,HWND_TOPMOST,0,0,1000,1000,SWP_NOSIZE | SWP_NOMOVE); //修改窗口为最顶部
	
	//隐藏窗口标题栏 
	long style = GetWindowLong(hParentWnd,GWL_STYLE); 
	style &= ~(WS_CAPTION); 
	// style &= ~(WS_MAXIMIZEBOX); 
	SetWindowLong(hParentWnd,GWL_STYLE,style);
	*/
	if (bShow)
	{
		//namedWindow(m_strCameraWindow, CV_GUI_NORMAL); //创建一个窗口 

		moveWindow(m_strCameraWindow,m_nWindowX,m_nWindowY);
		resizeWindow(m_strCameraWindow, m_nWindowWidth, m_nWindowHeight);
		::ShowWindow(m_hParentWnd,SW_SHOW);
	}
	else
	{
		::ShowWindow(m_hParentWnd,SW_HIDE);
	}
	
	
	
	
	/*
	HDC hdc=GetDC(NULL); //获得屏幕设备描述表句柄
	int ScrWidth=GetDeviceCaps(hdc,HORZRES); //获取屏幕水平分辨率
	int ScrHeight=GetDeviceCaps(hdc,VERTRES); //获取屏幕垂直分辨率
	ReleaseDC(NULL,hdc); //释放屏幕设备描述表 
	::MoveWindow(hParentWnd,0,0,ScrWidth,ScrHeight,0);
	Sleep(1000);
	*/
}

void CCameraApp::SetWindowPosition(int x, int y, int w, int h)
{
	if (x < 0 || y < 0 || w <= 0 || h <= 0)
	{
		return;
	}
	m_nWindowX = x;
	m_nWindowY = y;
	m_nWindowWidth = w;
	m_nWindowHeight = h;
}