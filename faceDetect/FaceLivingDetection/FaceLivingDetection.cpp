// FaceLivingDetection.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "FaceLivingDetection.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CFaceLivingDetectionApp

BEGIN_MESSAGE_MAP(CFaceLivingDetectionApp, CWinApp)
END_MESSAGE_MAP()


// CFaceLivingDetectionApp 构造

CFaceLivingDetectionApp::CFaceLivingDetectionApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CFaceLivingDetectionApp 对象

CFaceLivingDetectionApp theApp;


// CFaceLivingDetectionApp 初始化

BOOL CFaceLivingDetectionApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


/*
功能:人脸活体检测
参数:人脸图像
返回:0为活体  -1非活体
*/
int CFaceLivingDetectionApp::nLivingDetector(Mat faceImage)
{
	return g_Handler.nLivingDetector_(faceImage);
}

/*
功能:加载SVM训练模型
参数:模型路径
返回:无
*/
void CFaceLivingDetectionApp::LoadSvmModel(string modelPathName)
{
	return g_Handler.LoadSvmModel_(modelPathName);
}

/*
功能:训练活体检测模型
参数:livingPath:活体照片文件夹路径
参数:dummyPath:伪造照片文件夹路径
参数:saveModlePathName:模型的保存路径
返回:无
*/
void CFaceLivingDetectionApp::SvmTrain(string livingPath, string dummyPath, string saveModlePathName)
{
	return g_Handler.SvmTrain_(livingPath,dummyPath,saveModlePathName);
}

/*
功能:图片文件裁切人脸并保存
参数:imagePath:原始照片文件夹路径
参数:facePath:人脸照片保存文件夹路径
返回:无
*/
void CFaceLivingDetectionApp::FaceDetectionSave(string imagePath, string facePath)
{
	return g_Handler.FaceDetectionSave_(imagePath,facePath);
}