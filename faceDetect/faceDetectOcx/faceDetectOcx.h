#pragma once

// faceDetectOcx.h : faceDetectOcx.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������

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


//������
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


// CfaceDetectOcxApp : �й�ʵ�ֵ���Ϣ������� faceDetectOcx.cpp��

class CfaceDetectOcxApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();

	//��ʼ��
	LONG Init();
	//��������ͷ
	LONG StartCamera();
	//ֹͣ����ͷ
	LONG StopCamera(bool bWait = true);
	//������(ͼƬ·��)
	LONG StartLiveDetect(LPCTSTR jpgImagePath);

	//����ͼ���С
	void setCameraSize(int width, int height);
	//����ctrl���hwnd
	void setHwnd(HWND hWnd);


protected:
	
	//��ʼ��ʾ����ͷ
	bool bStartShowCamera();
	//��ʾ����ͷ��Ƶ�߳�
	static UINT _stdcall ThreadFuncStartShowCamera( LPVOID lpPara );
	//��ʾ��Ƶ
	void ShowCamera();

	//bool bStartLiveDetect();
	//static UINT _stdcall ThreadFuncStartLiveDetect( LPVOID lpPara );

	//����������ʶ��
	void LiveDetect();

	//����ʶ���ʼ��
	int nFaceDetectInit();
	//����ʶ������
	void FaceDetectDestroy();

	//ͷ��ͼƬ����Щ��ʾ����
	Mat FaceImgEnlarge(Mat matSrc, int &x, int &y, int &w, int &h);
	//ͼ��ת����(��������)
	float* Mat2data(Mat img);

	//�����뷭��
	string strExplainErrCode(long lErrCode);

	//�и��ַ�����vector
	vector<string> split(const string& str, const string& delim);

	//����ͼƬ
	void savePicture(Mat &mat);
	//ͼƬ����λ��
	void SavePicturePath(LPCTSTR strPath);
	//ͼƬתBase64
	string strMatToBase64(Mat mat);

	//�����̷߳������ݵ����߳�
	void PostMessageToMainThread(string data);

	//��ȡini��ķֱ���
	void GetResolution();

	//��ȡocxλ��
	string strGetOcxPath();
	//��ȡconfig.ini����string
	string GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, LPCSTR lpDefault);
	//��ȡconfig.ini����int
	int GetIniConfig(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault);

protected:
	//��ɫ����ͷindex
	int m_nCameraIndexColor;
	//�ڰ�����ͷindex
	int m_nCameraIndexBW;
	//��ɫ����ͷ����
	unsigned char * m_pBufferColor;
	//�ڰ�����ͷ����
	unsigned char * m_pBufferBW;
	//��ɫ����ͷ��Ƶ
	VideoCapture m_capColor;
	//�ڰ�����ͷ��Ƶ
	VideoCapture m_capBW;
	//��ɫͼƬ
	Mat m_imageColor; 
	//�ڰ�ͼƬ
	Mat m_imageBW; 
	//��ʾ��ͼ��
	Mat m_imageShow;
	//�Ƿ����й�hyInit
	bool m_bInit;
	//�Ƿ���������ͷ
	bool m_bStartCamera;
	//�Ƿ���������ʶ��
	bool m_LiveDetecting;

	//�߳̿�ʼ��ʾ����ͷ
	bool m_bThreadShowCamera;
	//�߳̾�� ��ʾ����ͷ
	HANDLE				m_handleThreadShowCamera ;
	//�̵߳�ַ ��ʾ����ͷ
	unsigned int		m_dwThreadShowCameraId ;


	//�߳̿�ʼ����ʶ��
	//bool m_bThreadLiveDetect;
	//�߳̾�� ����ʶ��
	//HANDLE				m_handleThreadLiveDetect ;
	//�̵߳�ַ ����ʶ��
	//unsigned int		m_dwThreadLiveDetectId ;

	//��ʾ��Ƶ��С
	Size m_cameraSize;

	//������
	CIEIfaceliving m_CheckLiving;
	//���ϵĵ�
	int m_nDoLandmark;
	
	//����ͼƬλ��
	string m_strPicturePath;

	//����ʶ��ɹ�����
	int m_nFaceDetectSuccessCount;
	//�����ھ��
	HWND m_hWnd;
	//ocxλ��
	string m_strOcxPath;

	//�ֱ��ʿ�
	int m_nResoWidth;
	//�ֱ��ʸ�
	int m_nResoHeight;
	//֡��(δʹ��)
	DWORD m_dwFrameRate;

};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;
extern CfaceDetectOcxApp g_theApp;
