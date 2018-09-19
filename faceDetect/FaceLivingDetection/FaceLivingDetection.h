// FaceLivingDetection.h : FaceLivingDetection DLL 的主头文件
//

#pragma once
#include "FaceLivingHandler.h"
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CFaceLivingDetectionApp
// 有关此类实现的信息，请参阅 FaceLivingDetection.cpp
//

class CFaceLivingDetectionApp : public CWinApp
{
public:
	CFaceLivingDetectionApp();

	/*
	功能:人脸活体检测
	参数:人脸图像
	返回:0为活体  1非活体  -1为未加载模型
	*/
	int nLivingDetector(Mat faceImage);

	/*
	功能:加载SVM训练模型
	参数:模型路径
	返回:无
	*/
	void LoadSvmModel(string modelPathName);

	/*
	功能:训练活体检测模型
	参数:livingPath:活体照片文件夹路径
	参数:dummyPath:假体照片文件夹路径
	参数:saveModlePathName:模型的保存路径
	返回:无
	*/
	void SvmTrain(string livingPath, string dummyPath, string saveModlePathName);

	/*
	功能:图片文件裁切人脸并保存
	参数:imagePath:原始照片文件夹路径
	参数:facePath:人脸照片保存文件夹路径
	返回:无
	*/
	void FaceDetectionSave(string imagePath, string facePath);

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
