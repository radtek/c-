#pragma once

// Camera.h : Camera.DLL ����ͷ�ļ�

#if !defined( __AFXCTL_H__ )
#error "�ڰ������ļ�֮ǰ������afxctl.h��"
#endif

#include "resource.h"       // ������

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


// CCameraApp : �й�ʵ�ֵ���Ϣ������� Camera.cpp��

class CCameraApp : public COleControlModule
{
protected:
	//��ɫ����ͷ��Ƶ
	VideoCapture m_capColor;
	//��ɫͼƬ
	Mat m_imageColor; 
	//��ɫ����ͷindex
	int m_nCameraIndexColor;
	//��ʾ��ͼ��
	Mat m_imageShow;

	//��ɫ����ͷ����
	unsigned char * m_pBufferColor;

	//�Ƿ����й�hyInit
	bool m_bInit;
	//�Ƿ�������ͷ
	bool m_bStartCamera;
	//�߳�������ʾ����ͷ����
	bool m_LiveDetecting;

	bool m_bThreadShowCamera;
	//�߳̾�� ��ʾ����ͷ
	HANDLE				m_handleThreadShowCamera ;
	//�̵߳�ַ ��ʾ����ͷ
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
