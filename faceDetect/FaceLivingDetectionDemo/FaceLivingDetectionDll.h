// FaceLivingDetection.h : FaceLivingDetection DLL 的主头文件
//
#pragma once

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

bool m_LoadDll = false;

static HINSTANCE m_FaceLivingHinstance = NULL;

/*
功能:人脸活体检测
参数:人脸图像
返回:0为活体  1非活体  -1为未加载模型
*/
typedef int (WINAPI *FuncLivingDetector)(Mat faceImage);
FuncLivingDetector	nLivingDetector = NULL;
/*
功能:加载SVM训练模型
参数:模型路径
返回:无
*/
typedef void (WINAPI *FuncLoadSvmModel)(string modelPathName);
FuncLoadSvmModel LoadSvmModel = NULL;
/*
功能:训练活体检测模型
参数:livingPath:活体照片文件夹路径
参数:dummyPath:假体照片文件夹路径
参数:saveModlePathName:模型的保存路径
返回:无
*/
typedef void (WINAPI *FuncSvmTrain)(string livingPath, string dummyPath, string saveModlePathName);
FuncSvmTrain SvmTrain = NULL;
/*
功能:图片文件裁切人脸并保存
参数:imagePath:原始照片文件夹路径
参数:facePath:人脸照片保存文件夹路径
返回:无
*/
typedef void (WINAPI *FuncFaceDetectionSave)(string imagePath, string facePath);
FuncFaceDetectionSave	FaceDetectionSave = NULL;


void FaceFreeLibrary()
{
	if (m_FaceLivingHinstance)
	{
		FreeLibrary(m_FaceLivingHinstance);
		m_FaceLivingHinstance = NULL;
	}
	m_LoadDll = false;
}

int  FaceLoadLibrary()
{
	int nRet = -1;
	do 
	{
		if (m_LoadDll)
		{
			nRet = 0;
			break;
		}

		m_FaceLivingHinstance = LoadLibrary("FaceLivingDetection.dll");
		if (m_FaceLivingHinstance == NULL)
		{
			break;
		}

		nLivingDetector	= (FuncLivingDetector)GetProcAddress(m_FaceLivingHinstance, "nLivingDetector");
		if(NULL == nLivingDetector)
		{
			break;
		}

		LoadSvmModel = (FuncLoadSvmModel)GetProcAddress(m_FaceLivingHinstance, "LoadSvmModel");
		if(NULL == LoadSvmModel)
		{
			break;
		}

		SvmTrain = (FuncSvmTrain)GetProcAddress(m_FaceLivingHinstance, "SvmTrain");
		if(NULL == SvmTrain)
		{
			break;
		}

		FaceDetectionSave = (FuncFaceDetectionSave)GetProcAddress(m_FaceLivingHinstance, "FaceDetectionSave");
		if(NULL == FaceDetectionSave)
		{
			break;
		}

		nRet = 0;
		m_LoadDll = true;

	} while (0);
	
	if (nRet != 0)
	{
		FaceFreeLibrary();
	}

	return nRet;
}
