// FaceLivingDetection.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "FaceLivingDetection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CFaceLivingDetectionApp

BEGIN_MESSAGE_MAP(CFaceLivingDetectionApp, CWinApp)
END_MESSAGE_MAP()


// CFaceLivingDetectionApp ����

CFaceLivingDetectionApp::CFaceLivingDetectionApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CFaceLivingDetectionApp ����

CFaceLivingDetectionApp theApp;


// CFaceLivingDetectionApp ��ʼ��

BOOL CFaceLivingDetectionApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


/*
����:����������
����:����ͼ��
����:0Ϊ����  -1�ǻ���
*/
int CFaceLivingDetectionApp::nLivingDetector(Mat faceImage)
{
	return g_Handler.nLivingDetector_(faceImage);
}

/*
����:����SVMѵ��ģ��
����:ģ��·��
����:��
*/
void CFaceLivingDetectionApp::LoadSvmModel(string modelPathName)
{
	return g_Handler.LoadSvmModel_(modelPathName);
}

/*
����:ѵ��������ģ��
����:livingPath:������Ƭ�ļ���·��
����:dummyPath:α����Ƭ�ļ���·��
����:saveModlePathName:ģ�͵ı���·��
����:��
*/
void CFaceLivingDetectionApp::SvmTrain(string livingPath, string dummyPath, string saveModlePathName)
{
	return g_Handler.SvmTrain_(livingPath,dummyPath,saveModlePathName);
}

/*
����:ͼƬ�ļ���������������
����:imagePath:ԭʼ��Ƭ�ļ���·��
����:facePath:������Ƭ�����ļ���·��
����:��
*/
void CFaceLivingDetectionApp::FaceDetectionSave(string imagePath, string facePath)
{
	return g_Handler.FaceDetectionSave_(imagePath,facePath);
}