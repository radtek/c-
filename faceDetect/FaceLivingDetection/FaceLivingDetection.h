// FaceLivingDetection.h : FaceLivingDetection DLL ����ͷ�ļ�
//

#pragma once
#include "FaceLivingHandler.h"
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CFaceLivingDetectionApp
// �йش���ʵ�ֵ���Ϣ������� FaceLivingDetection.cpp
//

class CFaceLivingDetectionApp : public CWinApp
{
public:
	CFaceLivingDetectionApp();

	/*
	����:����������
	����:����ͼ��
	����:0Ϊ����  1�ǻ���  -1Ϊδ����ģ��
	*/
	int nLivingDetector(Mat faceImage);

	/*
	����:����SVMѵ��ģ��
	����:ģ��·��
	����:��
	*/
	void LoadSvmModel(string modelPathName);

	/*
	����:ѵ��������ģ��
	����:livingPath:������Ƭ�ļ���·��
	����:dummyPath:������Ƭ�ļ���·��
	����:saveModlePathName:ģ�͵ı���·��
	����:��
	*/
	void SvmTrain(string livingPath, string dummyPath, string saveModlePathName);

	/*
	����:ͼƬ�ļ���������������
	����:imagePath:ԭʼ��Ƭ�ļ���·��
	����:facePath:������Ƭ�����ļ���·��
	����:��
	*/
	void FaceDetectionSave(string imagePath, string facePath);

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
