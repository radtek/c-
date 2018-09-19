#pragma once

// faceDetectOcx.h : faceDetectOcx.DLL 的主头文件

#if !defined( __AFXCTL_H__ )
#error "在包括此文件之前包括“afxctl.h”"
#endif

#include "resource.h"       // 主符号

#include "io.h"
#include "direct.h"
#include <vector>


#include <stdio.h>
#include <opencv2/opencv.hpp>
using namespace cv;

#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")

#include "CIEIfaceliving.h"
#pragma comment(lib,"CIEIfaceliving.lib")

#include "face_detection.h"
#pragma comment(lib,"FaceDetection.lib")

#define DETECT_BUFFER_SIZE 0x20000



//错误码
#define SUCCESS							0//成功
#define ERR_WEIGHTS_OR_KEY				-1//权重文件不存在或key文件不存在或key未授权
#define ERR_NOT_INIT					-2//未初始化
#define ERR_ALREADY_START_CAMERA		-3//摄像头已开启过
#define ERR_ALREADY_LIVEING_DETECT		-4//人脸识别已开启过
#define ERR_NOT_START_CAMERA			-5//未开启摄像头
#define ERR_OPEN_CAMERA_COLOR			-6//打开彩色摄像头失败
#define ERR_OPEN_CAMERA_BW				-7//打开黑白摄像头失败
#define ERR_THREAD_SHOW_CAMERA			-8//显示视频线程开启失败
#define ERR_FACE_DETECT_CACHE_COLOR		-9//彩色摄像头人脸识别缓存创建失败
#define ERR_FACE_DETECT_CACHE_BW		-10//黑白摄像头人脸识别缓存创建失败
#define ERR_CAMERA_VID_PID_CONFIG		-11//摄像头VID/PID配置不能相同
#define ERR_CAMERA_VID_PID_CONFIG_COLOR	-12//彩色摄像头VID/PID未找到对应设备
#define ERR_CAMERA_VID_PID_CONFIG_BW	-13//黑白摄像头VID/PID未找到对应设备
#define ERR_PICTURE_PATH				-14//图片保存路径有误
#define ERR_OTHER						-999//其它错误


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

	//开始活体检测
	bool bStartLiveDetect();
	//活体检测线程
	static UINT _stdcall ThreadFuncStartLiveDetect( LPVOID lpPara );
	//活体检测人脸识别
	void LiveDetect();

	//人脸识别初始化
	int nFaceDetectInit();
	//人脸识别销毁
	void FaceDetectDestroy();

	//头像图片增大些显示背景
	void FaceImgEnlarge(Mat &matSrc, int &x, int &y, int &w, int &h);
	//图像转数据(检测活体用)
	float* Mat2data(Mat img);

	//错误码翻译
	string strExplainErrCode(long lErrCode);

	//切割字符串到vector
	vector<string> split(const string& str, const string& delim);

	//保存图片
	void SavePicture(Mat &mat);
	//图片保存位置
	bool bSavePicturePath(string strPath);
	//图片转Base64
	string strMatToBase64(Mat &mat);

	//从子线程发送数据到主线程
	bool bPostMessageToMainThread(string data);

	//获取ini里的分辨率
	void GetResolution();

	//获取ocx位置
	string strGetOcxPath();
	//读取config.ini返回string
	string GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault);
	//读取config.ini返回int
	int GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault);
	//图片数据转base64
	string Base64Encode(const unsigned char* Data,int DataByte);

	/*
    功能：通过设备的VID与PID获取对应的ID
    入口参数：
        [in] vid：设备VID
		[in] pid：设备PID
    返回值：
		获取到的设备ID，-1:指定设备不存在
	*/
	int nGetDeviceIDfromVIDandPID(CString vid, CString pid);
	//获取摄像头序号
	void GetCameraIndex();
	//获取设备序列号
	void GetDeviceId();
	//cmd获取返回字符串
	CString strExecuteCmd(CString str);
	//string转TCHAR
	TCHAR* StringToChar(CString& str);
	//模糊检测
	bool bCheckBlur(Mat srcImage);

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
	bool m_bThreadLiveDetect;
	//线程句柄 人脸识别
	HANDLE				m_handleThreadLiveDetect ;
	//线程地址 人脸识别
	unsigned int		m_dwThreadLiveDetectId ;

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

	//识别成功判定总次数
	int m_nFaceDetectTotal;

	struct
	{
		string cpuID;
		string diskID;
		//string baseboardID;
		//string biosID;
		string macID;
	}deviceInfo;

	struct
	{
		bool detector;
		bool living;
		int x;
		int y;
		int widht;
		int height;
	}m_faceRect;
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
extern CfaceDetectOcxApp g_theApp;
