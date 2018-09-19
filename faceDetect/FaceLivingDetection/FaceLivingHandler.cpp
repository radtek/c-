#include "StdAfx.h"
#include "FaceLivingHandler.h"

#include<io.h>

#include <bitset>

CFaceLivingHandler g_Handler;

CFaceLivingHandler::CFaceLivingHandler(void)
{
	m_bLoadSvmModel = false;
}


CFaceLivingHandler::~CFaceLivingHandler(void)
{
}


int CFaceLivingHandler::nLivingDetector_(Mat faceImage)
{
	
	if ( !m_bLoadSvmModel )
	{
		return -1;
	}
	if (faceImage.channels() != 1)
	{
		cvtColor(faceImage, faceImage, CV_BGR2GRAY);
	}

	resize(faceImage, faceImage, Size(64, 64));

	GetUniformPatternLBPFeature(faceImage,faceImage,1,8);

	faceImage = GetLBPH(faceImage,59,3,3,true);

	return (int)m_SVM.predict(faceImage);
}

void CFaceLivingHandler::LoadSvmModel_(string modelPathName)
{
	m_SVM.clear();
	m_SVM.load(modelPathName.c_str());
	m_bLoadSvmModel = true;
}

void CFaceLivingHandler::SvmTrain_(string livingPath, string dummyPath,string saveModlePathName)
{
	Mat classes;
	Mat trainingData;

	Mat trainingImages;
	vector<int> trainingLabels;

	vector<string> livingFiles;
	vector<string> dummyFiles;
	//��ȡ��·���µ������ļ�  
	GetFiles(livingPath, livingFiles );
	GetFiles(dummyPath, dummyFiles );
	//��ȡ�������ͼƬ��
	GetImageFace(livingFiles, dummyFiles, trainingImages, trainingLabels);

	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	//����SVMѵ��������
	CvSVMParams SVM_params;
	SVM_params.svm_type = CvSVM::C_SVC;
	SVM_params.kernel_type = CvSVM::LINEAR; 
	SVM_params.degree = 0;
	SVM_params.gamma = 1;
	SVM_params.coef0 = 0;
	SVM_params.C = 1;
	SVM_params.nu = 0;
	SVM_params.p = 0;
	SVM_params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 0.01);
	//ѵ��
	CvSVM svm;
	svm.train(trainingData, classes, Mat(), Mat(), SVM_params);
	//����ģ��
	svm.save(saveModlePathName.c_str());
}

void CFaceLivingHandler::FaceDetectionSave_(string imagePath, string facePath)
{
	unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if(!pBuffer)
	{
		free(pBuffer);
		return ;
	}

	if (facePath[facePath.length()-1] != '\\')
	{
		facePath += "\\";
	}
	vector<string> imageFiles;
	//��ȡ��·���µ������ļ�  
	GetFiles(imagePath, imageFiles);

	int nFaceDetectCount = 0;
	for (int i = 0; i < imageFiles.size(); i++)
	{
		Mat image = imread(imageFiles[i]);
		Mat gray = image;
		if (image.channels() != 1)
		{
			cvtColor(gray, gray, CV_BGR2GRAY);
		}

		int doLandmark = 0;
		int * pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step, 1.2f, 2, 48, 0, doLandmark);

		for(int j = 0; j < (pResults ? *pResults : 0); j++)
		{
			short * p = ((short*)(pResults+1))+142*j;
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			//int neighbors = p[4];
			//int angle = p[5];

			if (x <= 1 || y <= 1 || x + w >= image.cols -1 || y + h >= image.rows -1)
			{
				continue;
			}

			//rectangle(image, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
			if (doLandmark)
			{
				for (int k = 0; k < 68; k++)
					circle(image, Point((int)p[6 + 2 * k], (int)p[6 + 2 * k + 1]), 1, Scalar(0, 255, 0));
			}
			
			char szFileName[32] = {0};
			sprintf(szFileName,"%d.bmp",nFaceDetectCount++);
			imwrite( facePath + szFileName, image(Rect(x,y,w,h)) );
		}
	}
	
	free(pBuffer);
}

void CFaceLivingHandler::GetFiles( string path, vector<string>& files )
{  
	//�ļ����  
	long   hFile   =   0;  
	//�ļ���Ϣ  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{  
			//�����Ŀ¼,����֮  
			//�������,�����б�  
			if((fileinfo.attrib &  _A_SUBDIR))  
			{  
				if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)  
					GetFiles( p.assign(path).append("\\").append(fileinfo.name), files );  
			}  
			else  
			{  
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );  
			}  
		}while(_findnext(hFile, &fileinfo)  == 0);  
		_findclose(hFile);  
	}  
}  

//��ȡ�������ͼƬ��
void CFaceLivingHandler::GetImageFace(vector<string> livingFiles, vector<string> dummyFiles , Mat& trainingImages, vector<int>& trainingLabels)
{ 
	for (int i = 0;i < livingFiles.size();i++)  
	{  
		//MessageBox(NULL,livingFiles[i].c_str(),NULL,NULL);  
		Mat SrcImage = imread(livingFiles[i]);

		if (SrcImage.channels() != 1)
		{
			cvtColor(SrcImage, SrcImage, CV_BGR2GRAY);
		}

		resize(SrcImage, SrcImage, Size(64, 64));

		GetUniformPatternLBPFeature(SrcImage,SrcImage,1,8);

		SrcImage = GetLBPH(SrcImage,59,3,3,true);

		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(0);
	} 

	for (int i = 0;i < dummyFiles.size();i++)  
	{  
		Mat SrcImage = imread(dummyFiles[i]);

		if (SrcImage.channels() != 1)
		{
			cvtColor(SrcImage, SrcImage, CV_BGR2GRAY);
		}

		resize(SrcImage, SrcImage, Size(64, 64));

		GetUniformPatternLBPFeature(SrcImage,SrcImage,1,8);

		SrcImage = GetLBPH(SrcImage,59,3,3,true);

		trainingImages.push_back(SrcImage);
		trainingLabels.push_back(1);
	}
}


//�����������
int CFaceLivingHandler::GetHopTimes(int n)
{
	int count = 0;
	bitset<8> binaryCode = n;
	for(int i=0;i<8;i++)
	{
		if(binaryCode[i] != binaryCode[(i+1)%8])
		{
			count++;
		}
	}
	return count;
}

//�ȼ�ģʽLBP��������
void CFaceLivingHandler::GetUniformPatternLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors)
{
	Mat src = _src.getMat();
	//LBP����ͼ��������������ļ���Ҫ׼ȷ
	_dst.create(src.rows-2*radius,src.cols-2*radius,CV_8UC1);
	Mat dst = _dst.getMat();
	dst.setTo(0);
	//LBP����ֵ��Ӧͼ��Ҷȱ����ֱ��Ĭ�ϲ�����Ϊ8λ
	uchar temp = 1;
	uchar table[256] = {0};
	for(int i=0;i<256;i++)
	{
		if(GetHopTimes(i)<3)
		{
			table[i] = temp;
			temp++;
		}
	}
	//�Ƿ����UniformPattern����ı�־
	bool flag = false;
	//����LBP����ͼ
	for(int k=0;k<neighbors;k++)
	{
		if(k==neighbors-1)
		{
			flag = true;
		}
		//���������������ĵ������ƫ����rx��ry
		float rx = static_cast<float>(radius * cos(2.0 * CV_PI * k / neighbors));
		float ry = -static_cast<float>(radius * sin(2.0 * CV_PI * k / neighbors));
		//Ϊ˫���Բ�ֵ��׼��
		//�Բ�����ƫ�����ֱ��������ȡ��
		int x1 = static_cast<int>(floor(rx));
		int x2 = static_cast<int>(ceil(rx));
		int y1 = static_cast<int>(floor(ry));
		int y2 = static_cast<int>(ceil(ry));
		//������ƫ����ӳ�䵽0-1֮��
		float tx = rx - x1;
		float ty = ry - y1;
		//����0-1֮���x��y��Ȩ�ؼ��㹫ʽ����Ȩ�أ�Ȩ�����������λ���޹أ��������Ĳ�ֵ�й�
		float w1 = (1-tx) * (1-ty);
		float w2 =    tx  * (1-ty);
		float w3 = (1-tx) *    ty;
		float w4 =    tx  *    ty;
		//ѭ������ÿ������
		for(int i=radius;i<src.rows-radius;i++)
		{
			for(int j=radius;j<src.cols-radius;j++)
			{
				//����������ص�ĻҶ�ֵ
				uchar center = src.at<uchar>(i,j);
				//����˫���Բ�ֵ��ʽ�����k��������ĻҶ�ֵ
				uchar neighbor = src.at<uchar>(i+x1,j+y1) * w1 + src.at<uchar>(i+x1,j+y2) *w2 \
					+ src.at<uchar>(i+x2,j+y1) * w3 +src.at<uchar>(i+x2,j+y2) *w4;
				//LBP����ͼ���ÿ���ھӵ�LBPֵ�ۼӣ��ۼ�ͨ���������ɣ���Ӧ��LBPֵͨ����λȡ��
				dst.at<uchar>(i-radius,j-radius) |= (neighbor>center) <<(neighbors-k-1);
				//����LBP������UniformPattern����
				if(flag)
				{
					dst.at<uchar>(i-radius,j-radius) = table[dst.at<uchar>(i-radius,j-radius)];
				}
			}
		}
	}
}

//����һ��LBP����ͼ����ֱ��ͼ
Mat CFaceLivingHandler::GetLocalRegionLBPH(const Mat& src,int minValue,int maxValue,bool normed)
{
	//����洢ֱ��ͼ�ľ���
	Mat result;
	//����õ�ֱ��ͼbin����Ŀ��ֱ��ͼ����Ĵ�С
	int histSize = maxValue - minValue + 1;
	//����ֱ��ͼÿһά��bin�ı仯��Χ
	float range[] = { static_cast<float>(minValue),static_cast<float>(maxValue + 1) };
	//����ֱ��ͼ����bin�ı仯��Χ
	const float* ranges = { range };
	//����ֱ��ͼ��src��Ҫ����ֱ��ͼ��ͼ��1��Ҫ����ֱ��ͼ��ͼ����Ŀ��0�Ǽ���ֱ��ͼ���õ�ͼ���ͨ����ţ���0����
	//Mat()��Ҫ�õ���ģ��resultΪ�����ֱ��ͼ��1Ϊ�����ֱ��ͼ��ά�ȣ�histSizeֱ��ͼ��ÿһά�ı仯��Χ
	//ranges������ֱ��ͼ�ı仯��Χ�������յ㣩
	calcHist(&src,1,0,Mat(),result,1,&histSize,&ranges,true,false);
	//��һ��
	if(normed)
	{
		result /= (int)src.total();
	}
	//�����ʾ��ֻ��1�еľ���
	return result.reshape(1,1);
}

//����LBP����ͼ���ֱ��ͼLBPH
Mat CFaceLivingHandler::GetLBPH(InputArray _src,int numPatterns,int grid_x,int grid_y,bool normed)
{
	Mat src = _src.getMat();
	int width = src.cols / grid_x;
	int height = src.rows / grid_y;
	//����LBPH���к��У�grid_x*grid_y��ʾ��ͼ��ָ����ôЩ�飬numPatterns��ʾLBPֵ��ģʽ����
	Mat result = Mat::zeros(grid_x * grid_y,numPatterns,CV_32FC1);
	if(src.empty())
	{
		return result.reshape(1,1);
	}
	int resultRowIndex = 0;
	//��ͼ����зָ�ָ��grid_x*grid_y�飬grid_x��grid_yĬ��Ϊ8
	for(int i=0;i<grid_x;i++)
	{
		for(int j=0;j<grid_y;j++)
		{
			//ͼ��ֿ�
			Mat src_cell = Mat(src,Range(i*height,(i+1)*height),Range(j*width,(j+1)*width));
			//����ֱ��ͼ
			Mat hist_cell = GetLocalRegionLBPH(src_cell,0,(numPatterns-1),normed);
			//��ֱ��ͼ�ŵ�result��
			Mat rowResult = result.row(resultRowIndex);
			hist_cell.reshape(1,1).convertTo(rowResult,CV_32FC1);
			resultRowIndex++;
		}
	}
	return result.reshape(1,1);
}