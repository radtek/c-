// faceDetectOcx.cpp : CfaceDetectOcxApp �� DLL ע���ʵ�֡�

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

// CfaceDetectOcxApp::InitInstance - DLL ��ʼ��

BOOL CfaceDetectOcxApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: �ڴ�������Լ���ģ���ʼ�����롣

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



// CfaceDetectOcxApp::ExitInstance - DLL ��ֹ

int CfaceDetectOcxApp::ExitInstance()
{
	// TODO: �ڴ�������Լ���ģ����ֹ���롣
	
	FaceDetectDestroy();
	destroyWindow("DISPLAY");
	return COleControlModule::ExitInstance();
}



// DllRegisterServer - ������ӵ�ϵͳע���

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - �����ϵͳע������Ƴ�

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
			CLogger::Instance()->TraceInfo("Init: ����ͷ���кŲ���������ͬ");
			lRet = ERR_CAMERA_NUM_CONFIG;
			break;
		}

		string strWeightsPath = m_strOcxPath + string("CIEIfaceliving.weights");
		string strKeyPath = m_strOcxPath + string("CIEIfaceliving.key");

		if( 1 != m_CheckLiving.load_weight(strWeightsPath.c_str(), strKeyPath.c_str()) )
		{
			CLogger::Instance()->TraceInfo("Init: weightsȨ���ļ������� or keyδ��Ȩ err");
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
			CLogger::Instance()->TraceInfo("StartCamera:δ��ʼ��");
			break;
		}

		if ( m_bStartCamera )
		{
			lRet = ERR_ALREADY_START_CAMERA;
			CLogger::Instance()->TraceInfo("StartCamera:�Ѿ���������ͷ");
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
		//��ɫ����ͷ�ֱ���
		m_capColor.set(CV_CAP_PROP_FRAME_WIDTH, m_nResoWidth);
		m_capColor.set(CV_CAP_PROP_FRAME_HEIGHT, m_nResoHeight);
		//�ڰ�����ͷ�ֱ���
		m_capBW.set(CV_CAP_PROP_FRAME_WIDTH, m_nResoWidth);
		m_capBW.set(CV_CAP_PROP_FRAME_HEIGHT, m_nResoHeight);
		//Ԥ�ȶ�ȡһ��
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
			CLogger::Instance()->TraceInfo("StopCamera: δ��ʼ��");
			lRet = ERR_NOT_INIT;
			break;
		}

		if ( !m_bStartCamera )
		{
			CLogger::Instance()->TraceInfo("StopCamera: δ��������ͷ");
			lRet = ERR_NOT_START_CAMERA;
			break;
		}
		/*
		//�ر�����ʶ���߳�
		if (m_bThreadLiveDetect)
		{
			m_bThreadLiveDetect = false;
			if ( m_handleThreadLiveDetect != INVALID_HANDLE_VALUE && bWait )
			{
				::PostThreadMessage( m_dwThreadLiveDetectId , WM_QUIT,0,0 ) ;	//���������ִ���¼����������waitevent�������Զ���ֹ

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
		
		//�ر�����ͷ�߳�
		if (m_bThreadShowCamera)
		{
			m_bThreadShowCamera = false;
			if ( m_handleThreadShowCamera != INVALID_HANDLE_VALUE && bWait )
			{
				::PostThreadMessage( m_dwThreadShowCameraId , WM_QUIT,0,0 ) ;	//���������ִ���¼����������waitevent�������Զ���ֹ

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
			CLogger::Instance()->TraceInfo("StartLiveDetect: δ��ʼ��");
			break;
		}

		if ( !m_bStartCamera )
		{
			lRet = ERR_NOT_START_CAMERA;
			CLogger::Instance()->TraceInfo("StartLiveDetect: δ��������ͷ");
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
	//�ͷŻ���
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
		//�ڰ�����
		Mat gray;
		cvtColor(m_imageBW, gray, CV_BGR2GRAY);

		int * pResultsBW = NULL; 
		//facedetect_frontal//����
		//facedetect_frontal_surveillance//�������
		//facedetect_multiview//���ӽ�
		//facedetect_multiview_reinforce//���ӽǼ�ǿ
		pResultsBW = facedetect_frontal_surveillance(m_pBufferBW, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 2, 48, 0, 0);

		//CLogger::Instance()->TraceInfo("�ڰ�%d faces detected", (pResultsBW ? *pResultsBW : 0));

		if ( !(pResultsBW && *pResultsBW == 1))
		{
			CLogger::Instance()->TraceInfo("LiveDetect: �ڰ�ʶ������Ϊ0�����1");
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
		
		//��ɫ����
		cvtColor(m_imageColor, gray, CV_BGR2GRAY);

		int * pResultsColor = NULL; 

		pResultsColor = facedetect_frontal_surveillance(m_pBufferColor, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 2, 48, 0, m_nDoLandmark);

		//CLogger::Instance()->TraceInfo("��ɫ%d faces detected.", (pResultsColor ? *pResultsColor : 0));

		if ( !(pResultsColor && *pResultsColor == 1) )
		{
			CLogger::Instance()->TraceInfo("LiveDetect: ��ɫʶ������Ϊ0�����1 ");
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
			//�ۿ�36-47()
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
			CLogger::Instance()->TraceInfo("LiveDetect: ʶ��ɹ�");
			savePicture(m_imageColor);
			CLogger::Instance()->TraceInfo("LiveDetect: ����ͼƬ���");
			string strImgBase64 = strMatToBase64(m_imageColor);

			if ( !strImgBase64.empty() )
			{
				CLogger::Instance()->TraceInfo("LiveDetect: ת��BASE64 = %s���",strImgBase64.substr(30,10).c_str());
				PostMessageToMainThread(strImgBase64);
				CLogger::Instance()->TraceInfo("LiveDetect: ���̷߳������");
				m_nFaceDetectSuccessCount = 0;
				m_LiveDetecting = false;
			}
			else
			{
				CLogger::Instance()->TraceInfo("LiveDetect: ת��BASE64Ϊ��");
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
	//ʶ��ͼ����
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
		return "�ɹ�";
	case ERR_WEIGHTS_OR_KEY:
		return "Ȩ���ļ������ڻ�key�ļ������ڻ�keyδ��Ȩ";
	case ERR_NOT_INIT:
		return "δ��ʼ��";
	case ERR_ALREADY_START_CAMERA:
		return "����ͷ�ѿ�����";
	case ERR_ALREADY_LIVEING_DETECT:
		return "����ʶ���ѿ�����";
	case ERR_NOT_START_CAMERA:
		return "δ��������ͷ";
	case ERR_OPEN_CAMERA_COLOR:
		return "�򿪲�ɫ����ͷʧ��";
	case ERR_OPEN_CAMERA_BW:
		return "�򿪺ڰ�����ͷʧ��";
	case ERR_THREAD_SHOW_CAMERA:
		return "��ʾ��Ƶ�߳̿���ʧ��";
	case ERR_FACE_DETECT_CACHE_COLOR:
		return "��ɫ����ͷ����ʶ�𻺴洴��ʧ��";
	case ERR_FACE_DETECT_CACHE_BW:
		return "�ڰ�����ͷ����ʶ�𻺴洴��ʧ��";
	case ERR_CAMERA_NUM_CONFIG:
		return "����ͷ���к����ò�����ͬ";
		
	default:
		return "δ֪����";
	}
}

vector<string> CfaceDetectOcxApp::split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//�Ƚ�Ҫ�и���ַ�����string����ת��Ϊchar*����
	char * strs = new char[str.length() + 1] ; //��Ҫ����
	strcpy(strs, str.c_str()); 

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //�ָ�õ����ַ���ת��Ϊstring����
		res.push_back(s); //����������
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
	vector<uchar> vecImg;                               //Mat ͼƬ����ת��Ϊvector<uchar>
	vector<int> vecCompression_params;
	vecCompression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
	vecCompression_params.push_back(90);
	imencode(".jpg", mat, vecImg, vecCompression_params);

	Base64 base64;
	return base64.Encode(vecImg.data(), vecImg.size());     //ʵ��ͼƬ��base64����
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