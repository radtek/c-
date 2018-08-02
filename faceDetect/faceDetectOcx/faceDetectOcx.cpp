// faceDetectOcx.cpp : CfaceDetectOcxApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxCtrl.h"
#include "DlgCamera.h"

CfaceDetectOcxCtrl *m_pFDOCtrl;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CfaceDetectOcxApp g_theApp;

const GUID CDECL _tlid = { 0xE73220C, 0xA9F, 0x41B5, { 0xB0, 0x63, 0x90, 0x36, 0x93, 0x6A, 0x27, 0x9B } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

#define UNKNOWN_FLOW_THRESH 1e9

// CfaceDetectOcxApp::InitInstance - DLL 初始化

BOOL CfaceDetectOcxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。

		CLogger::Instance()->TraceInfo("&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&faceDetectOcx&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
		m_bThreadShowCamera = false;
		m_bInit = false;
		m_bStartCamera = false;
		m_LiveDetecting = false;
		m_nDoLandmark = 0;
		m_nFaceDetectSuccessCount = 0;

		m_strOcxPath = strGetOcxPath();

	}

	return bInit;
}



// CfaceDetectOcxApp::ExitInstance - DLL 终止

int CfaceDetectOcxApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。
	
	FaceDetectDestroy();
	destroyWindow("DISPLAY");
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

LONG CfaceDetectOcxApp::Init()
{
	CLogger::Instance()->TraceInfo("===============Init start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		m_nCameraIndexColor = GetIniConfig("setting","CameraIndexColor",0);
		m_nCameraIndexBW	= GetIniConfig("setting","CameraIndexBW",1);

		if (m_nCameraIndexColor == m_nCameraIndexBW)
		{
			CLogger::Instance()->TraceInfo("Init: 摄像头序列号不能配置相同");
			lRet = ERR_CAMERA_NUM_CONFIG;
			break;
		}

		string strWeightsPath = m_strOcxPath + string("CIEIfaceliving.weights");
		string strKeyPath = m_strOcxPath + string("CIEIfaceliving.key");

		if( 1 != m_CheckLiving.load_weight(strWeightsPath.c_str(), strKeyPath.c_str()) )
		{
			CLogger::Instance()->TraceInfo("Init: weights权重文件不存在 or key未授权 err");
			lRet = ERR_WEIGHTS_OR_KEY;
			break;
		}
		
		GetResolution();

		lRet = SUCCESS;
		m_bInit = true;

	} while (0);

	CLogger::Instance()->TraceInfo("===============Init end return<%d>===============",lRet);
	return lRet;
}

LONG CfaceDetectOcxApp::StartCamera()
{
	CLogger::Instance()->TraceInfo("===============StartCamera start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if ( !m_bInit )
		{
			lRet = ERR_NOT_INIT;
			CLogger::Instance()->TraceInfo("StartCamera:未初始化");
			break;
		}

		if ( m_bStartCamera )
		{
			lRet = ERR_ALREADY_START_CAMERA;
			CLogger::Instance()->TraceInfo("StartCamera:已经开过摄像头");
			break;
		}

		if( !m_capColor.open(m_nCameraIndexColor) && !m_capColor.isOpened() )
		{
			lRet = ERR_OPEN_CAMERA_COLOR;
			CLogger::Instance()->TraceInfo("StartCamera:m_capColor.open failed");
			break;
		}

		if( !m_capBW.open(m_nCameraIndexBW) && !m_capBW.isOpened() )
		{
			lRet = ERR_OPEN_CAMERA_BW;
			CLogger::Instance()->TraceInfo("StartCamera:m_capBW.open failed");
			break;
		}
		//彩色摄像头分辨率
		m_capColor.set(CV_CAP_PROP_FRAME_WIDTH, m_nResoWidth);
		m_capColor.set(CV_CAP_PROP_FRAME_HEIGHT, m_nResoHeight);
		//黑白摄像头分辨率
		m_capBW.set(CV_CAP_PROP_FRAME_WIDTH, m_nResoWidth);
		m_capBW.set(CV_CAP_PROP_FRAME_HEIGHT, m_nResoHeight);
		//预先读取一遍
		m_capColor.read(m_imageColor);
		m_capBW.read(m_imageBW);
		
		if ( !bStartShowCamera() )
		{
			lRet = ERR_THREAD_SHOW_CAMERA;
			CLogger::Instance()->TraceInfo("StartCamera: !bStartShowCamera()");
			break;
		}
		
		m_bStartCamera = true;
		lRet = SUCCESS;

	} while (0);

	CLogger::Instance()->TraceInfo("===============StartCamera end return<%d>===============",lRet);
	return lRet;
}


LONG CfaceDetectOcxApp::StopCamera(bool bWait)
{
	CLogger::Instance()->TraceInfo("===============StopCamera start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if ( !m_bInit )
		{
			CLogger::Instance()->TraceInfo("StopCamera: 未初始化");
			lRet = ERR_NOT_INIT;
			break;
		}

		if ( !m_bStartCamera )
		{
			CLogger::Instance()->TraceInfo("StopCamera: 未开启摄像头");
			lRet = ERR_NOT_START_CAMERA;
			break;
		}
		/*
		//关闭人脸识别线程
		if (m_bThreadLiveDetect)
		{
			m_bThreadLiveDetect = false;
			if ( m_handleThreadLiveDetect != INVALID_HANDLE_VALUE && bWait )
			{
				::PostThreadMessage( m_dwThreadLiveDetectId , WM_QUIT,0,0 ) ;	//如果流程在执行事件管理器里的waitevent函数会自动终止

				DWORD dwRes = ::WaitForSingleObject( m_handleThreadLiveDetect, 1000 );
				if( dwRes != WAIT_OBJECT_0 ) 
				{
					////TrcWritef( EC_WARN ,"RSHandler::Stop will be terminated, errcode = %d." , ::GetLastError() ) ;
					::TerminateThread( m_handleThreadLiveDetect , -1 ) ;
				}
				m_handleThreadLiveDetect = INVALID_HANDLE_VALUE ;
			}
		}
		*/
		
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
		
		FaceDetectDestroy();

		m_bStartCamera = false;
		m_LiveDetecting = false;

		lRet = SUCCESS;

	} while (0);
	CLogger::Instance()->TraceInfo("===============StopCamera end return<%d>===============",lRet);
	return lRet;
}


LONG CfaceDetectOcxApp::StartLiveDetect(LPCTSTR jpgImagePath)
{
	CLogger::Instance()->TraceInfo("===============StartLiveDetect start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if (!m_bInit)
		{
			lRet = ERR_NOT_INIT;
			CLogger::Instance()->TraceInfo("StartLiveDetect: 未初始化");
			break;
		}

		if ( !m_bStartCamera )
		{
			lRet = ERR_NOT_START_CAMERA;
			CLogger::Instance()->TraceInfo("StartLiveDetect: 未开启摄像头");
			break;
		}

		if (m_LiveDetecting)
		{
			lRet = ERR_ALREADY_LIVEING_DETECT;
			CLogger::Instance()->TraceInfo("StartLiveDetect:nFaceDetectInit() = %d",lRet);
			break;
		}

		int nRet = nFaceDetectInit();
		if ( SUCCESS != nRet )
		{
			lRet = nRet;
			CLogger::Instance()->TraceInfo("StartLiveDetect:nFaceDetectInit() = %d",lRet);
			break;	
		}

		CString path(jpgImagePath);
		if (!path.IsEmpty())
		{
			SavePicturePath(jpgImagePath);
		}
		
		m_LiveDetecting = true;

		lRet = SUCCESS;

	} while (0);
	CLogger::Instance()->TraceInfo("===============StartLiveDetect end return<%d>===============",lRet);
	return lRet;
}


void CfaceDetectOcxApp::FaceDetectDestroy()
{
	//释放缓存
	if (m_pBufferColor)
	{
		free(m_pBufferColor);
		m_pBufferColor = NULL;
	}
	
	if (m_pBufferBW)
	{
		free(m_pBufferBW);
		m_pBufferBW = NULL;
	}

	m_imageColor.release();
	m_imageBW.release();
	m_imageShow.release();

	m_capColor.release();
	m_capBW.release();

	//destroyAllWindows();
}



bool CfaceDetectOcxApp::bStartShowCamera()
{
	m_bThreadShowCamera = true;
	m_handleThreadShowCamera = (HANDLE)::_beginthreadex(0, 0, ThreadFuncStartShowCamera, (void*)this, 0, & m_dwThreadShowCameraId );

	CLogger::Instance()->TraceInfo("bStartShowCamera: m_dwThreadShowCameraId");
	if (m_handleThreadShowCamera == INVALID_HANDLE_VALUE)
	{
		CLogger::Instance()->TraceFatal("bStartShowCamera: StartShowCamera err code = %d",::GetLastError());
		m_bThreadShowCamera = false;
		return false;
	}

	return true ;
}

UINT _stdcall CfaceDetectOcxApp::ThreadFuncStartShowCamera( LPVOID	lpPara ) 
{
	try
	{
		CLogger::Instance()->TraceInfo("ThreadFuncStartShowCamera: begin");
		CfaceDetectOcxApp *pHandle = ( CfaceDetectOcxApp * ) lpPara ;

		while( pHandle->m_bThreadShowCamera )
		{
			pHandle->ShowCamera();
			//Sleep(30);
			
		}
		CLogger::Instance()->TraceInfo("ThreadFuncStartShowCamera: end");
	}
	catch(...)
	{
		CLogger::Instance()->TraceFatal("ThreadFuncStartShowCamera: thread exception!!!");
	}
	return 0 ;
}

void CfaceDetectOcxApp::ShowCamera()
{
	do 
	{
		m_capColor.read(m_imageColor);
		if(m_imageColor.empty())
		{
			CLogger::Instance()->TraceInfo("ShowCamera: m_imageColor.empty()");
			break;
		}

		m_capBW.read(m_imageBW); 
		if(m_imageBW.empty())
		{
			CLogger::Instance()->TraceInfo("ShowCamera: m_imageBW.empty()");
			break;
		}

		m_imageShow = m_imageColor.clone();

		if ( m_imageShow.empty() )
		{
			CLogger::Instance()->TraceInfo("ShowCamera: m_imageShow.empty()");
			break;
		}

		if ( m_LiveDetecting )
		{
			LiveDetect();
		}

		
		//imshow("camera",m_imageShow);

		g_dlgCamera.showCameraPic(m_imageShow,m_cameraSize);


	} while (0);
	
}

int CfaceDetectOcxApp::nFaceDetectInit()
{
	int nRet = ERR_OTHER;
	do 
	{
		m_pBufferColor = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!m_pBufferColor)
		{
			nRet = ERR_FACE_DETECT_CACHE_COLOR;
			break;
		}

		m_pBufferBW = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!m_pBufferBW)
		{
			nRet = ERR_FACE_DETECT_CACHE_BW;
			break;
		}
		nRet = SUCCESS;
	} while (0);
	
	return nRet;
}
/*
bool CfaceDetectOcxApp::bStartLiveDetect()
{
	
	m_bThreadLiveDetect = true;
	m_handleThreadLiveDetect = (HANDLE)::_beginthreadex(0, 0, ThreadFuncStartLiveDetect, (void*)this, 0, & m_dwThreadLiveDetectId );
	CLogger::Instance()->TraceInfo("bStartLiveDetect: m_dwThreadLiveDetectId");
	if (m_handleThreadLiveDetect == INVALID_HANDLE_VALUE)
	{
		CLogger::Instance()->TraceFatal("bStartLiveDetect: ThreadFuncStartLiveDetect err code = %d",::GetLastError());
		m_bThreadLiveDetect = false;
		return false;
	}

	return true ;
}

UINT _stdcall CfaceDetectOcxApp::ThreadFuncStartLiveDetect( LPVOID	lpPara ) 
{
	
	CfaceDetectOcxApp *pHandle = ( CfaceDetectOcxApp * ) lpPara ;
	try
	{
		CLogger::Instance()->TraceInfo("ThreadFuncStartLiveDetect: begin");
		pHandle->m_LiveDetecting = true;
		while( pHandle->m_bThreadLiveDetect )
		{
			pHandle->LiveDetect();
			Sleep(30);
		}
		pHandle->m_LiveDetecting = false;
		CLogger::Instance()->TraceInfo("ThreadFuncStartLiveDetect: end");
	}
	catch(...)
	{
		pHandle->m_LiveDetecting = false;
		CLogger::Instance()->TraceFatal("ThreadFuncStartLiveDetect: thread exception");
	}
	return 0 ;
}
*/
void CfaceDetectOcxApp::LiveDetect()
{
	bool bFaceDetect = false;
	do 
	{
		//黑白摄像
		Mat gray;
		cvtColor(m_imageBW, gray, CV_BGR2GRAY);

		int * pResultsBW = NULL; 
		//facedetect_frontal//正面
		//facedetect_frontal_surveillance//正面监视
		//facedetect_multiview//多视角
		//facedetect_multiview_reinforce//多视角加强
		pResultsBW = facedetect_frontal_surveillance(m_pBufferBW, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 2, 48, 0, 0);

		//CLogger::Instance()->TraceInfo("黑白%d faces detected", (pResultsBW ? *pResultsBW : 0));

		if ( !(pResultsBW && *pResultsBW == 1))
		{
			CLogger::Instance()->TraceInfo("LiveDetect: 黑白识别人数为0或大于1");
			break;
		}
		
		short * pBW = ((short*)(pResultsBW + 1));
		int xBW = pBW[0];
		int yBW = pBW[1];
		int wBW = pBW[2];
		int hBW = pBW[3];

		//CLogger::Instance()->TraceInfo("LiveDetect:before xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);
		Mat matFaceCut = FaceImgEnlarge(m_imageBW,xBW,yBW,wBW,hBW);
		//CLogger::Instance()->TraceInfo("LiveDetect:after xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);

		float* faceData = Mat2data(matFaceCut);
		int nDetector = m_CheckLiving.detector(faceData);
		//CLogger::Instance()->TraceInfo("LiveDetect:m_CheckLiving.detector(faceData) = %d",nDetector);
		
		//彩色摄像
		cvtColor(m_imageColor, gray, CV_BGR2GRAY);

		int * pResultsColor = NULL; 

		pResultsColor = facedetect_frontal_surveillance(m_pBufferColor, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 2, 48, 0, m_nDoLandmark);

		//CLogger::Instance()->TraceInfo("彩色%d faces detected.", (pResultsColor ? *pResultsColor : 0));

		if ( !(pResultsColor && *pResultsColor == 1) )
		{
			CLogger::Instance()->TraceInfo("LiveDetect: 彩色识别人数为0或大于1 ");
			break;
		}

		short *pColor = ((short*)(pResultsColor + 1));
		int xColor = pColor[0];
		int yColor = pColor[1];
		int wColor = pColor[2];
		int hColor = pColor[3];
		//CLogger::Instance()->TraceInfo("face_rect=[%d, %d, %d, %d]\n", xColor, yColor, wColor, hColor);
		
		if (m_nDoLandmark)
		{
			//眼眶36-47()
			for (int j = 36; j <= 47; j++)//for (int j = 0; j < 68; j++)
			{
				circle(m_imageShow, Point((int)pColor[6 + 2 * j], (int)pColor[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
			}
		}

		if (!nDetector)
		{
			rectangle(m_imageShow, Rect(xColor, yColor, wColor, hColor), Scalar(0, 0, 255), 2);
			break;
		}
		
		rectangle(m_imageShow, Rect(xColor, yColor, wColor, hColor), Scalar(0, 255, 0), 2);

		bFaceDetect = true;
		
	} while (0);

	if (bFaceDetect)
	{
		m_nFaceDetectSuccessCount++;
		if (m_nFaceDetectSuccessCount >= 20)
		{
			CLogger::Instance()->TraceInfo("LiveDetect: 识别成功");
			savePicture(m_imageColor);
			CLogger::Instance()->TraceInfo("LiveDetect: 保存图片完毕");
			string strImgBase64 = strMatToBase64(m_imageColor);

			if ( !strImgBase64.empty() )
			{
				CLogger::Instance()->TraceInfo("LiveDetect: 转换BASE64 = %s完毕",strImgBase64.substr(30,10).c_str());
				PostMessageToMainThread(strImgBase64);
				CLogger::Instance()->TraceInfo("LiveDetect: 子线程发送完毕");
				m_nFaceDetectSuccessCount = 0;
				m_LiveDetecting = false;
			}
			else
			{
				CLogger::Instance()->TraceInfo("LiveDetect: 转换BASE64为空");
			}
			
		}
	}
	else
	{
		m_nFaceDetectSuccessCount = 0;
	}
}

void CfaceDetectOcxApp::setCameraSize(int width, int height)
{
	if (width != 0 && height != 0)
	{
		m_cameraSize = Size(width,height);
	}
	
}

Mat CfaceDetectOcxApp::FaceImgEnlarge(Mat matSrc, int &x, int &y, int &w, int &h)
{
	//CLogger::Instance()->TraceInfo("FaceImgEnlarge:before x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	//识别图中心
	int x0 = x + w/2;
	int y0 = y + h/2;
	//CLogger::Instance()->TraceInfo("FaceImgEnlarge:x0 = %d, y0 = %d",x0,y0);
	int maxWidth = w + 100;
	int maxHeight = h + 100;
	//CLogger::Instance()->TraceInfo("FaceImgEnlarge:maxWidth = %d, maxHeight = %d",maxWidth,maxHeight);
	if (maxWidth > w)
	{
		int widthLeft = x0 - maxWidth/2 > 0 ? maxWidth : 2 * x0;
		int widthRight = x0 + maxWidth/2 < matSrc.cols ? maxWidth : 2 * (matSrc.cols - x0);

		w = widthLeft < widthRight ? widthLeft : widthRight;
		x = x0 - w/2;
	}

	if (maxHeight > h)
	{
		int heightUp = y0 - maxHeight/2 > 0 ? maxHeight : 2 * y0;
		int heightDown = y0 + maxHeight/2 < matSrc.rows ? maxHeight : 2 * (matSrc.rows - y0);
		h = heightUp < heightDown ? heightUp : heightDown;
		y = y0 - h/2;
	}

	//CLogger::Instance()->TraceInfo("FaceImgEnlarge:after x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	w = w > matSrc.cols - x ? matSrc.cols - x : w;
	h = h > matSrc.rows - y ? matSrc.rows - y : h;

	//CLogger::Instance()->TraceInfo("FaceImgEnlarge:after2 x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	return matSrc(Rect(x,y,w,h));
}


float* CfaceDetectOcxApp::Mat2data(Mat img)
{
	cvtColor(img, img, CV_BGR2GRAY);
	resize(img, img, Size(64, 64));

	int w = img.cols;
	int h = img.rows;
	int c = img.channels();

	float min = FLT_MAX;
	float max = -FLT_MAX;

	float* data = (float*)calloc(w*h*c, sizeof(float));
	int count = 0;
	for (int k = 0; k < c; ++k)
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
			{
				float pixel = (float)img.data[(i*w + j)*c + k];
				if (pixel > max)
					max = pixel;
				if (pixel < min)
					min = pixel;

				data[count++] = pixel;
			}
			for (int i = 0; i < count; ++i)
				data[i] = (data[i] - min) / (max - min);

			return data;
}


string CfaceDetectOcxApp::strExplainErrCode(long lErrCode)
{
	switch(lErrCode)
	{
	case SUCCESS:
		return "成功";
	case ERR_WEIGHTS_OR_KEY:
		return "权重文件不存在或key文件不存在或key未授权";
	case ERR_NOT_INIT:
		return "未初始化";
	case ERR_ALREADY_START_CAMERA:
		return "摄像头已开启过";
	case ERR_ALREADY_LIVEING_DETECT:
		return "人脸识别已开启过";
	case ERR_NOT_START_CAMERA:
		return "未开启摄像头";
	case ERR_OPEN_CAMERA_COLOR:
		return "打开彩色摄像头失败";
	case ERR_OPEN_CAMERA_BW:
		return "打开黑白摄像头失败";
	case ERR_THREAD_SHOW_CAMERA:
		return "显示视频线程开启失败";
	case ERR_FACE_DETECT_CACHE_COLOR:
		return "彩色摄像头人脸识别缓存创建失败";
	case ERR_FACE_DETECT_CACHE_BW:
		return "黑白摄像头人脸识别缓存创建失败";
	case ERR_CAMERA_NUM_CONFIG:
		return "摄像头序列号配置不能相同";
		
	default:
		return "未知错误";
	}
}

vector<string> CfaceDetectOcxApp::split(const string& str, const string& delim) {
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

void CfaceDetectOcxApp::SavePicturePath(LPCTSTR strPath)
{
	vector<string> vecPaths = split(strPath,"\\");
	string path = vecPaths[0];
	for(int i = 1; i < vecPaths.size()-1; i++)
	{
		path += "\\" + vecPaths[i];
		if ( access(path.c_str(),6) == -1 )
		{
			mkdir(path.c_str());
		}
	}
	m_strPicturePath = strPath;
}

void CfaceDetectOcxApp::savePicture(Mat &mat)
{
	if ( 0 != m_strPicturePath.length() )
	{
		/*
		char picName[]  = "picture.jpg";
		if (m_strPicturePath.substr(m_strPicturePath.length()-1,1) != "\\")
		{
			m_strPicturePath += "\\";
		}
		imwrite(m_strPicturePath + picName, mat);
		*/
		imwrite(m_strPicturePath, mat);
	}
}

string CfaceDetectOcxApp::strMatToBase64(Mat mat)
{
	vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
	vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imencode(".jpg", mat, vecImg, vecCompression_params);

	Base64 base64;
	return base64.Encode(vecImg.data(), vecImg.size());     //实现图片的base64编码
}

void CfaceDetectOcxApp::setHwnd(HWND hWnd)
{
	if (hWnd)
	{
		m_hWnd = hWnd;
	}
}

void CfaceDetectOcxApp::PostMessageToMainThread(string data)
{
	try
	{
		CLogger::Instance()->TraceInfo("PostMessageToMainThread: err1");
		LPBYTE  * lpByte = new LPBYTE[data.size()];
		CLogger::Instance()->TraceInfo("PostMessageToMainThread: err2");
		::CopyMemory( lpByte ,data.c_str() ,data.size()) ;
		CLogger::Instance()->TraceInfo("PostMessageToMainThread: err3");
		COPYDATASTRUCT cds;
		cds.cbData = data.size();
		cds.dwData = 1001;
		cds.lpData = lpByte;
		CLogger::Instance()->TraceInfo("PostMessageToMainThread: err4");
		::PostMessage( m_hWnd ,WM_MESSAGE_EVENT,(WPARAM)NULL,(LPARAM)&cds ) ;
		Sleep(10);
		CLogger::Instance()->TraceInfo("PostMessageToMainThread: err5");
		delete lpByte ;
	}
	catch(...)
	{
		CLogger::Instance()->TraceFatal("PostMessageToMainThread: exception!!!");
	}
	
}

string CfaceDetectOcxApp::strGetOcxPath()
{
	CString strPath(this->m_pszHelpFilePath);
	int nPos = strPath.ReverseFind('\\') + 1;
	return strPath.Left(nPos).GetBuffer(0);
}


string CfaceDetectOcxApp::GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault)
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");

	char szBuf[256] = {0};
	::GetPrivateProfileString(lpAppName , lpKeyName, lpDefault , szBuf , sizeof(szBuf)/sizeof(char) , strIniPath );

	return szBuf;
}

int CfaceDetectOcxApp::GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault)
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");

	return GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, strIniPath );
}

void CfaceDetectOcxApp::GetResolution()
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");
	int nResolution = GetIniConfig("setting","resolution",0);

	char szResolution[64] = {0};
	sprintf(szResolution,"resolution%d",nResolution);
	m_nResoWidth = GetIniConfig(szResolution,"width",640);
	m_nResoHeight = GetIniConfig(szResolution,"height",480);
	
}