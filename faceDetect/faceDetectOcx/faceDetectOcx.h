#pragma once

// faceDetectOcx.h : faceDetectOcx.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号

#include "io.h"
#include "direct.h"
#include <vector>

#include "Base64.h"

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")

#include "CIEIfaceliving.h"
#pragma comment(lib,"CIEIfaceliving.lib")

#define DETECT_BUFFER_SIZE 0x20000
using namespace cv;


//错误码
#define SUCCESS							0
#define ERR_WEIGHTS_OR_KEY				-1
#define ERR_NOT_INIT					-2
#define ERR_ALREADY_START_CAMERA		-3
#define ERR_ALREADY_LIVEING_DETECT		-4
#define ERR_NOT_START_CAMERA			-5
#define ERR_OPEN_CAMERA_COLOR			-6
#define ERR_OPEN_CAMERA_BW				-7
#define ERR_THREAD_SHOW_CAMERA			-8
#define ERR_FACE_DETECT_CACHE_COLOR		-9
#define ERR_FACE_DETECT_CACHE_BW		-10
#define ERR_CAMERA_NUM_CONFIG			-11
#define ERR_OTHER						-999


// CfaceDetectOcxApp : 有关实现的信息，请参阅 faceDetectOcx.cpp。

class CfaceDetectOcxApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();

	//初始化
	LONG Init();
	//开启摄像头
	LONG StartCamera();
	//停止摄像头
	LONG StopCamera(bool bWait = true);
	//活体检测(图片路径)
	LONG StartLiveDetect(LPCTSTR jpgImagePath);

	//设置图像大小
	void setCameraSize(int width, int height);
	//设置ctrl句柄hwnd
	void setHwnd(HWND hWnd);


protected:
	
	//开始显示摄像头
	bool bStartShowCamera();
	//显示摄像头视频线程
	static UINT _stdcall ThreadFuncStartShowCamera( LPVOID lpPara );
	//显示视频
	void ShowCamera();

	//bool bStartLiveDetect();
	//static UINT _stdcall ThreadFuncStartLiveDetect( LPVOID lpPara );

	//活体检测人脸识别
	void LiveDetect();

	//人脸识别初始化
	int nFaceDetectInit();
	//人脸识别销毁
	void FaceDetectDestroy();

	//头像图片增大些显示背景
	Mat FaceImgEnlarge(Mat matSrc, int &x, int &y, int &w, int &h);
	//图像转数据(检测活体用)
	float* Mat2data(Mat img);

	//错误码翻译
	string strExplainErrCode(long lErrCode);

	//切割字符串到vector
	vector<string> split(const string& str, const string& delim);

	//保存图片
	void savePicture(Mat &mat);
	//图片保存位置
	void SavePicturePath(LPCTSTR strPath);
	//图片转Base64
	string strMatToBase64(Mat mat);

	//从子线程发送数据到主线程
	void PostMessageToMainThread(string data);

	//获取ini里的分辨率
	void GetResolution();

	//获取ocx位置
	string strGetOcxPath();
	//读取config.ini返回string
	string GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault);
	//读取config.ini返回int
	int GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault);

protected:
	//彩色摄像头index
	int m_nCameraIndexColor;
	//黑白摄像头index
	int m_nCameraIndexBW;
	//彩色摄像头缓存
	unsigned char * m_pBufferColor;
	//黑白摄像头缓存
	unsigned char * m_pBufferBW;
	//彩色摄像头视频
	VideoCapture m_capColor;
	//黑白摄像头视频
	VideoCapture m_capBW;
	//彩色图片
	Mat m_imageColor; 
	//黑白图片
	Mat m_imageBW; 
	//显示的图像
	Mat m_imageShow;
	//是否运行过hyInit
	bool m_bInit;
	//是否开启过摄像头
	bool m_bStartCamera;
	//是否正在人脸识别
	bool m_LiveDetecting;

	//线程开始显示摄像头
	bool m_bThreadShowCamera;
	//线程句柄 显示摄像头
	HANDLE				m_handleThreadShowCamera ;
	//线程地址 显示摄像头
	unsigned int		m_dwThreadShowCameraId ;


	//线程开始人脸识别
	//bool m_bThreadLiveDetect;
	//线程句柄 人脸识别
	//HANDLE				m_handleThreadLiveDetect ;
	//线程地址 人脸识别
	//unsigned int		m_dwThreadLiveDetectId ;

	//显示视频大小
	Size m_cameraSize;

	//活体检测
	CIEIfaceliving m_CheckLiving;
	//脸上的点
	int m_nDoLandmark;
	
	//保存图片位置
	string m_strPicturePath;

	//连续识别成功次数
	int m_nFaceDetectSuccessCount;
	//主窗口句柄
	HWND m_hWnd;
	//ocx位置
	string m_strOcxPath;

	//分辨率宽
	int m_nResoWidth;
	//分辨率高
	int m_nResoHeight;
	//帧率(未使用)
	DWORD m_dwFrameRate;

};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
extern CfaceDetectOcxApp g_theApp;
