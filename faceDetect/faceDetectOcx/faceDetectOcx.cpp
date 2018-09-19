// faceDetectOcx.cpp : CfaceDetectOcxApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "faceDetectOcx.h"
#include "faceDetectOcxCtrl.h"
#include "DlgCamera.h"

#include <initguid.h>

#include "strmif.h"
#include "uuids.h"

#pragma comment(lib,"strmiids.lib")

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
		TrcErrInit( "faceDetect" ) ;
		TrcWritef( EC_INFO ,"=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=faceDetectOcx=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=&=");
		m_bThreadShowCamera = false;
		m_bInit = false;
		m_bStartCamera = false;
		m_LiveDetecting = false;
		m_nDoLandmark = 0;
		m_nFaceDetectSuccessCount = 0;

		m_strOcxPath = strGetOcxPath();

		m_faceRect.x = 0;
		m_faceRect.y = 0;
		m_faceRect.widht = 0;
		m_faceRect.height = 0;
		m_faceRect.detector = false;
		m_faceRect.living = false;

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

//初始化
LONG CfaceDetectOcxApp::Init()
{
	TrcWritef( EC_INFO ,"===============Init start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		//获取硬件序列号
		GetDeviceId();
		//获取摄像头序号
		GetCameraIndex();

		if ( m_nCameraIndexColor == -1 )
		{
			TrcWritef( EC_INFO ,"Init: 彩色摄像头VID/PID未找到对应设备");
			lRet = ERR_CAMERA_VID_PID_CONFIG_COLOR;
			break;
		}

		if ( m_nCameraIndexBW == -1 )
		{
			TrcWritef( EC_INFO ,"Init: 黑白摄像头VID/PID未找到对应设备");
			lRet = ERR_CAMERA_VID_PID_CONFIG_BW;
			break;
		}
		
		if (m_nCameraIndexColor == m_nCameraIndexBW)
		{
			TrcWritef( EC_INFO ,"Init: 摄像头VID/PID不能配置相同");
			lRet = ERR_CAMERA_VID_PID_CONFIG;
			break;
		}
		
		string strWeightsPath = m_strOcxPath + string("CIEIfaceliving.weights");
		string strKeyPath = m_strOcxPath + string("CIEIfaceliving.key");

		if( 1 != m_CheckLiving.load_weight(strWeightsPath.c_str(), strKeyPath.c_str()) )
		{
			TrcWritef( EC_INFO ,"Init: weights权重文件不存在 or key未授权 err");
			lRet = ERR_WEIGHTS_OR_KEY;
			break;
		}

		int nRet = nFaceDetectInit();
		if ( SUCCESS != nRet )
		{
			lRet = nRet;
			TrcWritef( EC_INFO ,"StartLiveDetect:nFaceDetectInit() = %d",lRet);
			break;	
		}
		
		m_nFaceDetectTotal = GetIniConfig("setting","FaceDetectTotal",20);

		//获取分辨率
		GetResolution();

		lRet = SUCCESS;
		m_bInit = true;

	} while (0);

	TrcWritef( EC_INFO ,"===============Init end return<%d>===============",lRet);
	return lRet;
}

//开始摄像头
LONG CfaceDetectOcxApp::StartCamera()
{
	TrcWritef( EC_INFO ,"===============StartCamera start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if ( !m_bInit )
		{
			lRet = ERR_NOT_INIT;
			TrcWritef( EC_INFO ,"StartCamera:未初始化");
			break;
		}

		if ( m_bStartCamera )
		{
			lRet = ERR_ALREADY_START_CAMERA;
			TrcWritef( EC_INFO ,"StartCamera:已经开过摄像头");
			break;
		}

		if( !m_capColor.open(m_nCameraIndexColor) && !m_capColor.isOpened() )
		{
			lRet = ERR_OPEN_CAMERA_COLOR;
			TrcWritef( EC_INFO ,"StartCamera:m_capColor.open failed");
			break;
		}

		if( !m_capBW.open(m_nCameraIndexBW) && !m_capBW.isOpened() )
		{
			lRet = ERR_OPEN_CAMERA_BW;
			TrcWritef( EC_INFO ,"StartCamera:m_capBW.open failed");
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
			TrcWritef( EC_INFO ,"StartCamera: !bStartShowCamera()");
			break;
		}
		
		m_bStartCamera = true;
		lRet = SUCCESS;


	} while (0);

	TrcWritef( EC_INFO ,"===============StartCamera end return<%d>===============",lRet);
	return lRet;
}

//关闭摄像头
LONG CfaceDetectOcxApp::StopCamera(bool bWait)
{
	TrcWritef( EC_INFO ,"===============StopCamera start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if ( !m_bInit )
		{
			TrcWritef( EC_INFO ,"StopCamera: 未初始化");
			lRet = ERR_NOT_INIT;
			break;
		}

		if ( !m_bStartCamera )
		{
			TrcWritef( EC_INFO ,"StopCamera: 未开启摄像头");
			lRet = ERR_NOT_START_CAMERA;
			break;
		}
		
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
		
		//FaceDetectDestroy();
		m_imageColor.release();
		m_imageBW.release();
		m_imageShow.release();

		m_capColor.release();
		m_capBW.release();

		m_bStartCamera = false;
		m_LiveDetecting = false;

		lRet = SUCCESS;

	} while (0);
	TrcWritef( EC_INFO ,"===============StopCamera end return<%d>===============",lRet);
	return lRet;
}

//开始活体检测
LONG CfaceDetectOcxApp::StartLiveDetect(LPCTSTR jpgImagePath)
{
	TrcWritef( EC_INFO ,"===============StartLiveDetect start===============");
	LONG lRet = ERR_OTHER;
	do 
	{
		if (!m_bInit)
		{
			lRet = ERR_NOT_INIT;
			TrcWritef( EC_INFO ,"StartLiveDetect: 未初始化");
			break;
		}

		if ( !m_bStartCamera )
		{
			lRet = ERR_NOT_START_CAMERA;
			TrcWritef( EC_INFO ,"StartLiveDetect: 未开启摄像头");
			break;
		}


		if (m_LiveDetecting)
		{
			lRet = ERR_ALREADY_LIVEING_DETECT;
			TrcWritef( EC_INFO ,"StartLiveDetect:nFaceDetectInit() = %d",lRet);
			break;
		}


		string path(jpgImagePath);
		if (!path.empty())
		{
			if ( !bSavePicturePath(path) )
			{
				lRet = ERR_PICTURE_PATH;
				TrcWritef( EC_INFO ,"StartLiveDetect: 图片保存路径有误");
				break;
			}
		}

		/*
		int nRet = nFaceDetectInit();
		if ( SUCCESS != nRet )
		{
			lRet = nRet;
			TrcWritef( EC_INFO ,"StartLiveDetect:nFaceDetectInit() = %d",lRet);
			break;	
		}
		*/

		if ( !bStartLiveDetect() )
		{
			lRet = -101;
			TrcWritef( EC_INFO ,"StartLiveDetect: !bStartLiveDetect()");
			break;
		}
		
		m_LiveDetecting = true;

		lRet = SUCCESS;

	} while (0);
	TrcWritef( EC_INFO ,"===============StartLiveDetect end return<%d>===============",lRet);
	return lRet;
}

//人脸检测初始化
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

//人脸检测的销毁
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
	

	//destroyAllWindows();
}


//开始显示摄像头线程
bool CfaceDetectOcxApp::bStartShowCamera()
{
	m_bThreadShowCamera = true;
	m_handleThreadShowCamera = (HANDLE)::_beginthreadex(0, 0, ThreadFuncStartShowCamera, (void*)this, 0, & m_dwThreadShowCameraId );

	TrcWritef( EC_INFO ,"bStartShowCamera: m_dwThreadShowCameraId = %d",m_dwThreadShowCameraId);
	if (m_handleThreadShowCamera == INVALID_HANDLE_VALUE)
	{
		TrcWritef( EC_FATAL,"bStartShowCamera: StartShowCamera err code = %d",::GetLastError());
		m_bThreadShowCamera = false;
		return false;
	}

	return true ;
}
//显示摄像头视频线程
UINT _stdcall CfaceDetectOcxApp::ThreadFuncStartShowCamera( LPVOID	lpPara ) 
{
	try
	{
		TrcWritef( EC_INFO ,"ThreadFuncStartShowCamera: begin");
		CfaceDetectOcxApp *pHandle = ( CfaceDetectOcxApp * ) lpPara ;

		while( pHandle->m_bThreadShowCamera )
		{
			pHandle->ShowCamera();
			//Sleep(30);
			
		}
		TrcWritef( EC_INFO ,"ThreadFuncStartShowCamera: end");
	}
	catch(...)
	{
		TrcWritef( EC_FATAL,"ThreadFuncStartShowCamera: thread exception!!!");
	}
	return 0 ;
}

//显示摄像头视频
void CfaceDetectOcxApp::ShowCamera()
{
	do 
	{
		m_capColor.read(m_imageColor);
		if(m_imageColor.empty())
		{
			TrcWritef( EC_INFO ,"ShowCamera: m_imageColor.empty()");
			break;
		}

		m_capBW.read(m_imageBW); 
		if(m_imageBW.empty())
		{
			TrcWritef( EC_INFO ,"ShowCamera: m_imageBW.empty()");
			break;
		}

		m_imageShow = m_imageColor.clone();

		if ( m_imageShow.empty() )
		{
			TrcWritef( EC_INFO ,"ShowCamera: m_imageShow.empty()");
			break;
		}
		/*
		if ( m_LiveDetecting )
		{
			LiveDetect();
		}
		*/
		
		//imshow("camera",m_imageShow);
		
		if (m_faceRect.detector)
		{//检测到人脸
			//TrcWritef(EC_INFO,"检测到人脸 %d,%d,%d,%d",m_faceRect.x, m_faceRect.y, m_faceRect.widht, m_faceRect.height);
			if (m_faceRect.living)
			{//活体
				//TrcWritef(EC_INFO,"检测到活体");
				rectangle(m_imageShow, Rect(m_faceRect.x, m_faceRect.y, m_faceRect.widht, m_faceRect.height), Scalar(0, 255, 0), 2);
			}
			else
			{
				//TrcWritef(EC_INFO,"检测到非活体");
				rectangle(m_imageShow, Rect(m_faceRect.x, m_faceRect.y, m_faceRect.widht, m_faceRect.height), Scalar(0, 0, 255), 2);
			}
		}
		

		g_dlgCamera.showCameraPic(m_imageShow,m_cameraSize);


	} while (0);
	
}

//开始活体检测线程
bool CfaceDetectOcxApp::bStartLiveDetect()
{
	
	m_bThreadLiveDetect = true;
	m_handleThreadLiveDetect = (HANDLE)::_beginthreadex(0, 0, ThreadFuncStartLiveDetect, (void*)this, 0, & m_dwThreadLiveDetectId );
	TrcWritef( EC_INFO ,"bStartLiveDetect: m_dwThreadLiveDetectId = %d",m_dwThreadLiveDetectId);
	if (m_handleThreadLiveDetect == INVALID_HANDLE_VALUE)
	{
		TrcWritef( EC_FATAL,"bStartLiveDetect: ThreadFuncStartLiveDetect err code = %d",::GetLastError());
		m_bThreadLiveDetect = false;
		return false;
	}

	return true ;
}
//活体检测线程
UINT _stdcall CfaceDetectOcxApp::ThreadFuncStartLiveDetect( LPVOID	lpPara ) 
{
	
	CfaceDetectOcxApp *pHandle = ( CfaceDetectOcxApp * ) lpPara ;
	try
	{
		TrcWritef( EC_INFO ,"ThreadFuncStartLiveDetect: begin");
		pHandle->m_LiveDetecting = true;
		while( pHandle->m_bThreadLiveDetect )
		{
			pHandle->LiveDetect();
			Sleep(30);
		}
		pHandle->m_LiveDetecting = false;
		TrcWritef( EC_INFO ,"ThreadFuncStartLiveDetect: end");
	}
	catch(...)
	{
		pHandle->m_LiveDetecting = false;
		TrcWritef( EC_FATAL,"ThreadFuncStartLiveDetect: thread exception");
	}
	return 0 ;
}
/*
//活体检测(于仕琪老师)
void CfaceDetectOcxApp::LiveDetect()
{
	bool bFaceDetect = false;
	int * pResultsBW = NULL;
	int * pResultsColor = NULL; 
	short * pBW = NULL;
	short * pColor = NULL;

	do 
	{
		
		Mat gray;
		//黑白摄像
		cvtColor(m_imageBW, gray, CV_BGR2GRAY);

		//int * pResultsBW = NULL; 
		//facedetect_frontal//正面
		//facedetect_frontal_surveillance//正面监视
		//facedetect_multiview//多视角
		//facedetect_multiview_reinforce//多视角加强
		pResultsBW = facedetect_frontal(m_pBufferBW, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 1, 48, 0, 0);
		//gray.release();
		//TrcWritef( EC_INFO ,"黑白%d faces detected", (pResultsBW ? *pResultsBW : 0));
		
		if ( !(pResultsBW && *pResultsBW == 1))
		{
			//TrcWritef( EC_INFO ,"LiveDetect: 黑白识别人数为0或大于1");
			m_faceRect.detector = false;
			break;
		}
		
		pBW = ((short*)(pResultsBW + 1));
		int xBW = pBW[0];
		int yBW = pBW[1];
		int wBW = pBW[2];
		int hBW = pBW[3];


		//TrcWritef( EC_INFO ,"LiveDetect:before xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);


		Mat matFaceCut = m_imageBW.clone();
		FaceImgEnlarge(matFaceCut,xBW,yBW,wBW,hBW);
		//TrcWritef( EC_INFO ,"LiveDetect:after xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);


		//彩色摄像
		cvtColor(m_imageColor(Rect(xBW,yBW,wBW,hBW)), gray, CV_BGR2GRAY);

		//int * pResultsColor = NULL; 
		
		pResultsColor = facedetect_frontal_surveillance(m_pBufferColor, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 1, 48, 0, m_nDoLandmark);
		
		//TrcWritef( EC_INFO ,"彩色%d faces detected.", (pResultsColor ? *pResultsColor : 0));

		if ( !(pResultsColor && *pResultsColor == 1) )
		{
			//TrcWritef( EC_INFO ,"LiveDetect: 彩色识别人数为0或大于1 ");
			m_faceRect.detector = false;
			break;
		}

		pColor = ((short*)(pResultsColor + 1));
		int xColor = pColor[0];
		int yColor = pColor[1];
		int wColor = pColor[2];
		int hColor = pColor[3];

		

		xColor += xBW;
		yColor += yBW;
		//TrcWritef( EC_INFO ,"face_rect=[%d, %d, %d, %d]\n", xColor, yColor, wColor, hColor);
		
		if (m_nDoLandmark)
		{
			//眼眶36-47()
			for (int j = 36; j <= 47; j++)//for (int j = 0; j < 68; j++)
			{
				circle(m_imageShow, Point((int)pColor[6 + 2 * j], (int)pColor[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
			}
		}


		float* faceData = Mat2data(m_imageBW(Rect(xBW,yBW,wBW,hBW)));
		//matFaceCut.release();
		int nDetector = m_CheckLiving.detector(faceData);
		
		//TrcWritef( EC_INFO ,"LiveDetect:m_CheckLiving.detector(faceData) = %d",nDetector);
		m_faceRect.x = xColor;
		m_faceRect.y = yColor;
		m_faceRect.widht = wColor;
		m_faceRect.height = hColor;
		m_faceRect.detector = true;

		if (!nDetector)
		{//nDetector==0是检测失败
			m_faceRect.living = false;
			break;
		}

		m_faceRect.living = true;

		bFaceDetect = true;
		
	} while (0);

	if (bFaceDetect)
	{
		m_nFaceDetectSuccessCount++;
		if (m_nFaceDetectSuccessCount >= m_nFaceDetectTotal)
		{
			string strSend = "";
			//TrcWritef( EC_INFO ,"LiveDetect: 识别成功");
			SavePicture(m_imageColor);
			//TrcWritef( EC_INFO ,"LiveDetect: 保存图片完毕");
			bool bBlur = bCheckBlur(m_imageColor);
			
			string strImgBase64 = strMatToBase64(m_imageColor);
			//strImgBase64 = "abcd";
			if ( !strImgBase64.empty() )
			{
				strSend += "{\"Result\":0";
				strSend += ",\"Blur\":\"";
				strSend += bBlur?"1":"0";
				strSend += ",\"ImgBase64\":\"" + strImgBase64 + "\"";
				strSend += "\"}" ;
				//TrcWritef( EC_INFO ,"LiveDetect: 转换BASE64 = %s完毕",strImgBase64.c_str());
			}
			else
			{
				strSend += "{\"Result\":-1,\"Blur\":\"\",\"ImgBase64\":\"\"}";
				TrcWritef( EC_WARN ,"LiveDetect: 转换BASE64为空");
			}
			
			if ( bPostMessageToMainThread(strSend) )
			{
				//TrcWritef( EC_INFO ,"LiveDetect: 子线程发送完毕");
				//MessageBox(NULL,strImgBase64.c_str(),NULL,NULL);
				m_nFaceDetectSuccessCount = 0;
				m_LiveDetecting = false;
				m_bThreadLiveDetect = false;

				m_faceRect.x = 0;
				m_faceRect.y = 0;
				m_faceRect.widht = 0;
				m_faceRect.height = 0;
				m_faceRect.detector = false;
				m_faceRect.living = false;

			}
			else
			{
				TrcWritef( EC_WARN ,"LiveDetect: bPostMessageToMainThread return false");
			}
		}
	}
	else
	{
		m_nFaceDetectSuccessCount = 0;
	}
	
	pResultsBW = NULL;
	pBW = NULL;

	pResultsColor = NULL;
	pColor = NULL;
	
}
*/

//活体检测(中科院)
void CfaceDetectOcxApp::LiveDetect()
{
	bool bFaceDetect = false;
	int * pResultsBW = NULL;
	int * pResultsColor = NULL; 
	short * pBW = NULL;
	short * pColor = NULL;

	string strModelPath = m_strOcxPath + "seeta_fd_frontal_v1.0.bin";
	seeta::FaceDetection detector(strModelPath.c_str());

	detector.SetMinFaceSize(40);
	detector.SetScoreThresh(2.f);
	detector.SetImagePyramidScaleFactor(0.8f);
	detector.SetWindowStep(4, 4);

	do 
	{

		Mat gray;
		//黑白摄像
		cvtColor(m_imageBW, gray, CV_BGR2GRAY);

		seeta::ImageData img_data;
		img_data.data = gray.data;
		img_data.width = gray.cols;
		img_data.height = gray.rows;
		img_data.num_channels = 1;

		//人脸检测
		std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
		int32_t num_face = static_cast<int32_t>(faces.size());

		if ( !(num_face == 1))
		{
			//TrcWritef( EC_INFO ,"LiveDetect: 黑白识别人数为0或大于1");
			m_faceRect.detector = false;
			break;
		}

		int xBW = faces[0].bbox.x;
		int yBW = faces[0].bbox.y;
		int wBW = faces[0].bbox.width;
		int hBW = faces[0].bbox.height;
		//TrcWritef( EC_INFO ,"LiveDetect:before xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);

		Mat matFaceCut = m_imageBW.clone();
		FaceImgEnlarge(matFaceCut,xBW,yBW,wBW,hBW);
		//TrcWritef( EC_INFO ,"LiveDetect:after xBW = %d, yBW = %d, wBW = %d, hBW = %d",xBW,yBW,wBW,hBW);


		//彩色摄像
		cvtColor(m_imageColor(Rect(xBW,yBW,wBW,hBW)), gray, CV_BGR2GRAY);

		img_data.data = gray.data;
		img_data.width = gray.cols;
		img_data.height = gray.rows;
		img_data.num_channels = 1;
		//人脸检测
		faces = detector.Detect(img_data);
		num_face = static_cast<int32_t>(faces.size());

		if ( !(num_face == 1))
		{
			//TrcWritef( EC_INFO ,"LiveDetect: 彩色识别人数为0或大于1");
			m_faceRect.detector = false;
			break;
		}

		int xColor = faces[0].bbox.x;
		int yColor = faces[0].bbox.y;
		int wColor = faces[0].bbox.width;
		int hColor = faces[0].bbox.height;

		xColor += xBW;
		yColor += yBW;
		//TrcWritef( EC_INFO ,"face_rect=[%d, %d, %d, %d]\n", xColor, yColor, wColor, hColor);

		float* faceData = Mat2data(m_imageBW(Rect(xBW,yBW,wBW,hBW)));
		//matFaceCut.release();
		int nDetector = m_CheckLiving.detector(faceData);

		//TrcWritef( EC_INFO ,"LiveDetect:m_CheckLiving.detector(faceData) = %d",nDetector);
		m_faceRect.x = xColor;
		m_faceRect.y = yColor;
		m_faceRect.widht = wColor;
		m_faceRect.height = hColor;
		m_faceRect.detector = true;

		if (!nDetector)
		{//nDetector==0是检测失败
			m_faceRect.living = false;
			break;
		}

		m_faceRect.living = true;

		bFaceDetect = true;

	} while (0);

	if (bFaceDetect)
	{
		m_nFaceDetectSuccessCount++;
		if (m_nFaceDetectSuccessCount >= m_nFaceDetectTotal)
		{
			string strSend = "";
			//TrcWritef( EC_INFO ,"LiveDetect: 识别成功");
			Mat matSave = m_imageColor.clone();
			SavePicture(matSave);
			//TrcWritef( EC_INFO ,"LiveDetect: 保存图片完毕");
			bool bBlur = bCheckBlur(matSave);

			string strImgBase64 = strMatToBase64(matSave);
			//strImgBase64 = "abcd";
			if ( !strImgBase64.empty() )
			{
				strSend += "{\"Result\":0";
				strSend += ",\"Blur\":\"";
				strSend += bBlur?"1":"0";
				strSend += ",\"ImgBase64\":\"" + strImgBase64 + "\"";
				strSend += "\"}" ;
				//TrcWritef( EC_INFO ,"LiveDetect: 转换BASE64 = %s完毕",strImgBase64.c_str());
			}
			else
			{
				strSend += "{\"Result\":-1,\"Blur\":\"\",\"ImgBase64\":\"\"}";
				TrcWritef( EC_WARN ,"LiveDetect: 转换BASE64为空");
			}

			if ( bPostMessageToMainThread(strSend) )
			{
				//TrcWritef( EC_INFO ,"LiveDetect: 子线程发送完毕");
				//MessageBox(NULL,strImgBase64.c_str(),NULL,NULL);
				m_nFaceDetectSuccessCount = 0;
				m_LiveDetecting = false;
				m_bThreadLiveDetect = false;

				m_faceRect.x = 0;
				m_faceRect.y = 0;
				m_faceRect.widht = 0;
				m_faceRect.height = 0;
				m_faceRect.detector = false;
				m_faceRect.living = false;

			}
			else
			{
				TrcWritef( EC_WARN ,"LiveDetect: bPostMessageToMainThread return false");
			}
		}
	}
	else
	{
		m_nFaceDetectSuccessCount = 0;
	}

	pResultsBW = NULL;
	pBW = NULL;

	pResultsColor = NULL;
	pColor = NULL;

}

//设置播放窗口大小
void CfaceDetectOcxApp::setCameraSize(int width, int height)
{
	if (width != 0 && height != 0)
	{
		m_cameraSize = Size(width,height);
	}
	
}

//头像扩大显示一些背景
void CfaceDetectOcxApp::FaceImgEnlarge(Mat &matSrc, int &x, int &y, int &w, int &h)
{
	Mat mat = matSrc.clone();
	//TrcWritef( EC_INFO ,"FaceImgEnlarge:before x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	//识别图中心
	int x0 = x + w/2;
	int y0 = y + h/2;
	//TrcWritef( EC_INFO ,"FaceImgEnlarge:x0 = %d, y0 = %d",x0,y0);
	int maxWidth = w + 100;
	int maxHeight = h + 100;
	//TrcWritef( EC_INFO ,"FaceImgEnlarge:maxWidth = %d, maxHeight = %d",maxWidth,maxHeight);
	if (maxWidth > w)
	{
		int widthLeft = x0 - maxWidth/2 > 0 ? maxWidth : 2 * x0;
		int widthRight = x0 + maxWidth/2 < mat.cols ? maxWidth : 2 * (mat.cols - x0);

		w = widthLeft < widthRight ? widthLeft : widthRight;
		x = x0 - w/2;
	}

	if (maxHeight > h)
	{
		int heightUp = y0 - maxHeight/2 > 0 ? maxHeight : 2 * y0;
		int heightDown = y0 + maxHeight/2 < mat.rows ? maxHeight : 2 * (mat.rows - y0);
		h = heightUp < heightDown ? heightUp : heightDown;
		y = y0 - h/2;
	}

	//TrcWritef( EC_INFO ,"FaceImgEnlarge:after x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	x = x < 0 ? 0 : x;
	y = y < 0 ? 0 : y;
	w = w > mat.cols - x ? mat.cols - x : w;
	h = h > mat.rows - y ? mat.rows - y : h;

	//TrcWritef( EC_INFO ,"FaceImgEnlarge:after2 x = %d, y = %d, w = %d, h = %d",x,y,w,h);
	
	matSrc = mat(Rect(x,y,w,h));
	mat.release();
}

//mat转float
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

	img.release();
	return data;
}


//切割字符串
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

//保存图片路径
bool CfaceDetectOcxApp::bSavePicturePath(string strPath)
{
	vector<string> vecPaths = split(strPath,"\\");
	if ( 2 > vecPaths.size() )
	{//路径至少有盘符和文件名两个
		return false;
	}

	string name = vecPaths[vecPaths.size()-1];
	int nPos = name.find(".jpg");
	if ( nPos < 0 )
	{//文件名必须以.jpg结尾
		return false;
	}

	string path = vecPaths[0];
	if ( access(path.c_str(),6) == -1 )
	{//盘符必须存在
		return false;
	}

	for(int i = 1; i < vecPaths.size()-1; i++)
	{
		path += "\\" + vecPaths[i];
		if ( access(path.c_str(),6) == -1 )
		{
			mkdir(path.c_str());
		}
	}
	m_strPicturePath = strPath;

	return true;
}

//保存图片
void CfaceDetectOcxApp::SavePicture(Mat &mat)
{
	if ( !m_strPicturePath.empty() )
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

//mat转base64
string CfaceDetectOcxApp::strMatToBase64(Mat &mat)
{
	vector<uchar> vecImg;                               //Mat 图片数据转换为vector<uchar>
	vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imencode(".jpg", mat, vecImg, vecCompression_params);

	return Base64Encode(vecImg.data(), vecImg.size());     //实现图片的base64编码
}

//设置句柄
void CfaceDetectOcxApp::setHwnd(HWND hWnd)
{
	if (hWnd)
	{
		m_hWnd = hWnd;
	}
}

//给主线程发送消息
bool CfaceDetectOcxApp::bPostMessageToMainThread(string data)
{
	int len = data.size();
	//TrcWritef( EC_INFO ,"bPostMessageToMainThread: data = %s len = %d",data.c_str(),len);
	LPBYTE  * lpByte = new LPBYTE[len];
	//::CopyMemory( lpByte ,data.c_str() ,data.size()) ;
	memcpy(lpByte, data.c_str(), len);
	//memcpy_s(lpByte, data.size(), data.c_str(), data.size());
	COPYDATASTRUCT cds;
	cds.cbData = len;
	cds.dwData = 1001;
	cds.lpData = lpByte;
	if (!m_hWnd)
	{
		TrcWritef( EC_INFO ,"bPostMessageToMainThread: !m_hWnd");
		return false;
	}
	/*
	bool bRet = ::PostMessage( m_hWnd ,WM_MESSAGE_EVENT,(WPARAM)NULL,(LPARAM)&cds );
	if (!bRet)
	{
		TrcWritef( EC_INFO ,"bPostMessageToMainThread: PostMessage = false");
		return false;
	}
	*/
	LRESULT lRet = ::SendMessage( m_hWnd ,WM_COPYDATA,(WPARAM)NULL,(LPARAM)&cds );
	if (0 != lRet)
	{
		TrcWritef( EC_INFO ,"bPostMessageToMainThread: SendMessage lRet = %d",lRet);
		return false;
	}

	//TrcWritef( EC_INFO ,"bPostMessageToMainThread: err5");
	delete []lpByte ;
	return true;
}

//获取OCX路径
string CfaceDetectOcxApp::strGetOcxPath()
{
	CString strPath(this->m_pszHelpFilePath);
	int nPos = strPath.ReverseFind('\\') + 1;
	return strPath.Left(nPos).GetBuffer(0);
}


//获取ini的字符串
string CfaceDetectOcxApp::GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault)
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");

	char szBuf[256] = {0};
	::GetPrivateProfileString(lpAppName , lpKeyName, lpDefault , szBuf , sizeof(szBuf)/sizeof(char) , strIniPath );

	return szBuf;
}

//获取ini的数字
int CfaceDetectOcxApp::GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault)
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");

	return GetPrivateProfileInt(lpAppName, lpKeyName, nDefault, strIniPath );
}

//获取分辨率
void CfaceDetectOcxApp::GetResolution()
{
	CString strIniPath = m_strOcxPath.c_str() + CString("config.ini");
	int nResolution = GetIniConfig("setting","resolution",0);

	char szResolution[64] = {0};
	sprintf(szResolution,"resolution%d",nResolution);
	m_nResoWidth = GetIniConfig(szResolution,"width",640);
	m_nResoHeight = GetIniConfig(szResolution,"height",480);

}

//错误码转描述
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
	case ERR_CAMERA_VID_PID_CONFIG:
		return "摄像头VID/PID配置不能相同";
	case ERR_CAMERA_VID_PID_CONFIG_COLOR:
		return "彩色摄像头VID/PID未找到对应设备";
	case ERR_CAMERA_VID_PID_CONFIG_BW:
		return "黑白摄像头VID/PID未找到对应设备";
	case ERR_PICTURE_PATH:
		return "图片保存路径有误";

	default:
		return "未知错误";
	}
}


//转base64
string CfaceDetectOcxApp::Base64Encode(const unsigned char* Data,int DataByte)
{
	//编码表
	const char EncodeTable[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	//返回值
	string strEncode;
	unsigned char Tmp[4]={0};
	int LineLength=0;
	for(int i=0;i<(int)(DataByte / 3);i++)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		Tmp[3] = *Data++;
		strEncode+= EncodeTable[Tmp[1] >> 2];
		strEncode+= EncodeTable[((Tmp[1] << 4) | (Tmp[2] >> 4)) & 0x3F];
		strEncode+= EncodeTable[((Tmp[2] << 2) | (Tmp[3] >> 6)) & 0x3F];
		strEncode+= EncodeTable[Tmp[3] & 0x3F];
		if(LineLength+=4,LineLength==76) {strEncode+="\r\n";LineLength=0;}
	}
	//对剩余数据进行编码
	int Mod=DataByte % 3;
	if(Mod==1)
	{
		Tmp[1] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4)];
		strEncode+= "==";
	}
	else if(Mod==2)
	{
		Tmp[1] = *Data++;
		Tmp[2] = *Data++;
		strEncode+= EncodeTable[(Tmp[1] & 0xFC) >> 2];
		strEncode+= EncodeTable[((Tmp[1] & 0x03) << 4) | ((Tmp[2] & 0xF0) >> 4)];
		strEncode+= EncodeTable[((Tmp[2] & 0x0F) << 2)];
		strEncode+= "=";
	}

	return strEncode;
}


//通过vid&pid获取摄像头序号
int CfaceDetectOcxApp::nGetDeviceIDfromVIDandPID(CString vid, CString pid)
{
	int nRet = -1;
	do 
	{
		if(FAILED(CoInitialize(NULL)))
		{
			//AfxMessageBox(L"CCaptureVideo CoInitialize Failed!\r\n"); 
			break;
		}

		vid.MakeLower();
		pid.MakeLower();

		int id = 0;
		ICreateDevEnum *pCreateDevEnum;
		HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_ICreateDevEnum, (void**)&pCreateDevEnum);
		if (hr != NOERROR)
			break;

		CComPtr<IEnumMoniker> pEm;
		hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,&pEm, 0);
		if (hr != NOERROR)
			break;

		pEm->Reset();

		ULONG cFetched;
		IMoniker *pM;
		while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK) 
		{
			CString	str = "";
			CString	strVid, strPid;
			int		Count = 0;

			IPropertyBag *pBag;
			hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
			if(SUCCEEDED(hr)) 
			{
				VARIANT var;
				var.vt = VT_BSTR;
				hr = pBag->Read(L"DevicePath", &var, NULL);

				if( hr == 0 )
				{
					while( var.bstrVal[Count] != 0x00 )
					{
						str += var.bstrVal[Count];
						Count++;
					}

					strVid = str.Mid(str.Find("vid_")+4, 4);
					strPid = str.Mid(str.Find("pid_")+4, 4);

					if( 0 == strcmp(strVid, vid) && 0 == strcmp(strPid, pid) )
					{
						nRet = id;
						break;
					}
				}

				id++;

				pBag->Release();
			}
			pM->Release();
		}
		
	} while (0);

	CoUninitialize();

	return nRet;
}

//获取摄像头序号
void CfaceDetectOcxApp::GetCameraIndex()
{
	string strTemp;
	CString strVidPid;
	CString strVid, strPid;

	strTemp = GetIniConfig("setting","CameraVidPidColor","vid_2b16&pid_bcd6");

	strVidPid = strTemp.c_str();
	strVidPid = strVidPid.MakeLower();
	strVid = strVidPid.Mid(strVidPid.Find("vid_")+4,4);
	strPid = strVidPid.Mid(strVidPid.Find("pid_")+4,4);
	m_nCameraIndexColor = nGetDeviceIDfromVIDandPID(strVid,strPid);

	strTemp = GetIniConfig("setting","CameraVidPidBW","vid_2b16&pid_bcd7");

	strVidPid = strTemp.c_str();
	strVidPid = strVidPid.MakeLower();
	strVid = strVidPid.Mid(strVidPid.Find("vid_")+4,4);
	strPid = strVidPid.Mid(strVidPid.Find("pid_")+4,4);
	m_nCameraIndexBW = nGetDeviceIDfromVIDandPID(strVid,strPid);
}

TCHAR* CfaceDetectOcxApp::StringToChar(CString& str)
{
	int len=str.GetLength();
	TCHAR* tr=str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr; 
}

//输出cmd并返回数据
CString CfaceDetectOcxApp::strExecuteCmd(CString str)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		//MessageBox(NULL,"Error on CreatePipe()!",NULL,NULL);
		return NULL;
	}
	STARTUPINFO si={sizeof(si)};
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline=StringToChar(str);
	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))
	{
		//MessageBox(NULL,"Error on CreateProcess()!",NULL,NULL);
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer,0,4096);
	CString output;
	DWORD byteRead;
	while(true)
	{
		if (ReadFile(hRead,buffer,4095,&byteRead,NULL) == NULL)
		{
			break;
		}
		output += buffer;
	}
	return output;
}

//获取电脑的硬盘序列号/CPU序列号/网卡mac
void CfaceDetectOcxApp::GetDeviceId()
{
	CString strCpu = strExecuteCmd("wmic CPU get ProcessorID");
	strCpu = strCpu.Mid(CString("ProcessorID ").GetLength()+1);

	CString strDisk = strExecuteCmd("wmic DISKDRIVE get SerialNumber").GetBuffer(0);
	strDisk = strDisk.Mid(CString("SerialNumber ").GetLength()+1);	
	//deviceInfo.baseboardID	= ExecuteCmd("wmic BASEBOARD get SerialNumber").GetBuffer(0);
	//CString strBios	= ExecuteCmd("wmic BIOS get SerialNumber");
	//strBios = strBios.Mid(CString("SerialNumber ").GetLength()+1);
	CString strMac = strExecuteCmd("wmic NIC get MACAddress");
	strMac = strMac.Mid(CString("MACAddress ").GetLength()+1,CString("MACAddress ").GetLength()+1+17);

	deviceInfo.cpuID		= strCpu.GetBuffer(0);
	deviceInfo.diskID		= strDisk.GetBuffer(0);
	deviceInfo.macID		= strMac.GetBuffer(0);

	//MessageBox(NULL,deviceInfo.cpuID.c_str(),NULL,NULL);
	//MessageBox(NULL,deviceInfo.diskID.c_str(),NULL,NULL);
	//MessageBox(NULL,deviceInfo.baseboardID.c_str(),NULL,NULL);
	//MessageBox(NULL,deviceInfo.biosID.c_str(),NULL,NULL);
	//MessageBox(NULL,deviceInfo.macID.c_str(),NULL,NULL);
}




//模糊检测，如果原图像是模糊图像，返回ture，否则返回false
bool CfaceDetectOcxApp::bCheckBlur(Mat srcImage)
{

	Mat gray;
	//进行灰度化
	cvtColor(srcImage, gray, CV_RGB2GRAY);
	
	Mat matMeanValue, matStandardDeviation;	//用来存储均值和方差
	double dMeanValue = 0, dStandardDeviation = 0;
	//使用3x3的Laplacian算子卷积滤波
	Laplacian(gray, gray, CV_16S, 3);
	//归到0~255
	convertScaleAbs(gray, gray);
	//计算均值和方差
	meanStdDev(gray, matMeanValue, matStandardDeviation);
	dMeanValue = matMeanValue.at<double>(0, 0);		//均值
	dStandardDeviation = matStandardDeviation.at<double>(0, 0);		//标准差
	//cout << "原图像：" << endl;
	//cout << "均值: " << m1 << " , 方差: " << sd1*sd1 << endl;
	//TrcWritef(EC_INFO,"%d",(int)dStandardDeviation);
	if ( dStandardDeviation * dStandardDeviation < 500 )
	{
		TrcWritef(EC_INFO,"模糊图像");
		return true;
	}
	else
	{
		TrcWritef(EC_INFO,"清晰图像");
		return false; 
	}
}