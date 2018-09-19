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
	//��ȡ�ļ��е������ļ�·��
	void GetFiles( string path, vector<string>& files );
	//��ȡ�������ͼƬ��
	void GetImageFace(vector<string> livingFiles, vector<string> dummyFiles , Mat& trainingImages, vector<int>& trainingLabels);
	//�����������
	int GetHopTimes(int n);
	//�ȼ�ģʽLBP��������
	void GetUniformPatternLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors);
	//����һ��LBP����ͼ����ֱ��ͼ
	Mat GetLocalRegionLBPH(const Mat& src,int minValue,int maxValue,bool normed);
	//����LBP����ͼ���ֱ��ͼLBPH
	Mat GetLBPH(InputArray _src,int numPatterns,int grid_x,int grid_y,bool normed);

private:
	//ѵ��ģ��
	CvSVM m_SVM;

	bool m_bLoadSvmModel;
};

extern CFaceLivingHandler g_Handler;