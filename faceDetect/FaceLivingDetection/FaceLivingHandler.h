#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

#include "facedetect-dll.h"
#pragma comment(lib,"libfacedetect.lib")
#define DETECT_BUFFER_SIZE 0x20000

class CFaceLivingHandler
{
public:
	CFaceLivingHandler(void);
	~CFaceLivingHandler(void);

	int nLivingDetector_(Mat faceImage);

	void LoadSvmModel_(string modelPathName);

	void SvmTrain_(string livingPath, string dummyPath, string saveModlePathName);

	void FaceDetectionSave_(string imagePath, string facePath);

private:
	//获取文件夹的所有文件路径
	void GetFiles( string path, vector<string>& files );
	//获取真假人脸图片集
	void GetImageFace(vector<string> livingFiles, vector<string> dummyFiles , Mat& trainingImages, vector<int>& trainingLabels);
	//计算跳变次数
	int GetHopTimes(int n);
	//等价模式LBP特征计算
	void GetUniformPatternLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors);
	//计算一个LBP特征图像块的直方图
	Mat GetLocalRegionLBPH(const Mat& src,int minValue,int maxValue,bool normed);
	//计算LBP特征图像的直方图LBPH
	Mat GetLBPH(InputArray _src,int numPatterns,int grid_x,int grid_y,bool normed);

private:
	//训练模型
	CvSVM m_SVM;

	bool m_bLoadSvmModel;
};

extern CFaceLivingHandler g_Handler;