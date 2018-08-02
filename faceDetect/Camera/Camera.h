#pragma once

// Camera.h : Camera.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号

#include "Base64.h"

#include "io.h"
#include "direct.h"
#include <vector>
#include <string>

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include "facedetect-dll.h"

#pragma comment(lib,"libfacedetect.lib")

using namespace cv;
using namespace std;

#define DETECT_BUFFER_SIZE 0x20000


// CCameraApp : 有关实现的信息，请参阅 Camera.cpp。

class CCameraApp : public COleControlModule
{
protected:
	//彩色摄像头视频
	VideoCapture m_capColor;
	//彩色图片
	Mat m_imageColor; 
	//彩色摄像头index
	int m_nCameraIndexColor;
	//显示的图像
	Mat m_imageShow;

	//彩色摄像头缓存
	unsigned char * m_pBufferColor;

	//是否运行过hyInit
	bool m_bInit;
	//是否开启摄像头
	bool m_bStartCamera;
	//线程运行显示摄像头窗口
	bool m_LiveDetecting;

	bool m_bThreadShowCamera;
	//线程句柄 显示摄像头
	HANDLE				m_handleThreadShowCamera ;
	//线程地址 显示摄像头
	unsigned int		m_dwThreadShowCameraId ;

	string m_strPicturePath;

	int m_nPicMaxWidth;
	int m_nPicMaxHeight;

	string m_strCameraWindow;
	int m_nWindowX;
	int m_nWindowY;
	int m_nWindowWidth;
	int m_nWindowHeight;

	HWND m_hParentWnd;

public:
	BOOL InitInstance();
	int ExitInstance();

	LONG StartCamera();
	void StopCamera(bool bWait = true);
	CString GetPicture();
	void SavePicturePath(LPCTSTR strPath);
	void SetPicMaxWidthHeight(int width, int height);
	void SetWindowPosition(int x, int y, int w, int h);

protected:
	bool bStartShowCamera();
	static UINT _stdcall ThreadFuncStartShowCamera( LPVOID lpPara );
	void ShowCamera();

	int nFaceDetect(Mat &mat);

	void saveJpg(Mat &mat);
	void showWindow(bool bShow);

	int nInit();
	void Destroy();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
extern CCameraApp theApp;
