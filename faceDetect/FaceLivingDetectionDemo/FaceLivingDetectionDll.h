// FaceLivingDetection.h : FaceLivingDetection DLL ����ͷ�ļ�
//
#pragma once

#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

bool m_LoadDll = false;

static HINSTANCE m_FaceLivingHinstance = NULL;

/*
����:����������
����:����ͼ��
����:0Ϊ����  1�ǻ���  -1Ϊδ����ģ��
*/
typedef int (WINAPI *FuncLivingDetector)(Mat faceImage);
FuncLivingDetector	nLivingDetector = NULL;
/*
����:����SVMѵ��ģ��
����:ģ��·��
����:��
*/
typedef void (WINAPI *FuncLoadSvmModel)(string modelPathName);
FuncLoadSvmModel LoadSvmModel = NULL;
/*
����:ѵ��������ģ��
����:livingPath:������Ƭ�ļ���·��
����:dummyPath:������Ƭ�ļ���·��
����:saveModlePathName:ģ�͵ı���·��
����:��
*/
typedef void (WINAPI *FuncSvmTrain)(string livingPath, string dummyPath, string saveModlePathName);
FuncSvmTrain SvmTrain = NULL;
/*
����:ͼƬ�ļ���������������
����:imagePath:ԭʼ��Ƭ�ļ���·��
����:facePath:������Ƭ�����ļ���·��
����:��
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
