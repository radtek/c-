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
	//获取该路径下的所有文件  
	GetFiles(livingPath, livingFiles );
	GetFiles(dummyPath, dummyFiles );
	//获取真假人脸图片集
	GetImageFace(livingFiles, dummyFiles, trainingImages, trainingLabels);

	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	//配置SVM训练器参数
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
	//训练
	CvSVM svm;
	svm.train(trainingData, classes, Mat(), Mat(), SVM_params);
	//保存模型
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
	//获取该路径下的所有文件  
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
	//文件句柄  
	long   hFile   =   0;  
	//文件信息  
	struct _finddata_t fileinfo;  
	string p;  
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)  
	{  
		do  
		{  
			//如果是目录,迭代之  
			//如果不是,加入列表  
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

//获取真假人脸图片集
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


//计算跳变次数
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

//等价模式LBP特征计算
void CFaceLivingHandler::GetUniformPatternLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors)
{
	Mat src = _src.getMat();
	//LBP特征图像的行数和列数的计算要准确
	_dst.create(src.rows-2*radius,src.cols-2*radius,CV_8UC1);
	Mat dst = _dst.getMat();
	dst.setTo(0);
	//LBP特征值对应图像灰度编码表，直接默认采样点为8位
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
	//是否进行UniformPattern编码的标志
	bool flag = false;
	//计算LBP特征图
	for(int k=0;k<neighbors;k++)
	{
		if(k==neighbors-1)
		{
			flag = true;
		}
		//计算采样点对于中心点坐标的偏移量rx，ry
		float rx = static_cast<float>(radius * cos(2.0 * CV_PI * k / neighbors));
		float ry = -static_cast<float>(radius * sin(2.0 * CV_PI * k / neighbors));
		//为双线性插值做准备
		//对采样点偏移量分别进行上下取整
		int x1 = static_cast<int>(floor(rx));
		int x2 = static_cast<int>(ceil(rx));
		int y1 = static_cast<int>(floor(ry));
		int y2 = static_cast<int>(ceil(ry));
		//将坐标偏移量映射到0-1之间
		float tx = rx - x1;
		float ty = ry - y1;
		//根据0-1之间的x，y的权重计算公式计算权重，权重与坐标具体位置无关，与坐标间的差值有关
		float w1 = (1-tx) * (1-ty);
		float w2 =    tx  * (1-ty);
		float w3 = (1-tx) *    ty;
		float w4 =    tx  *    ty;
		//循环处理每个像素
		for(int i=radius;i<src.rows-radius;i++)
		{
			for(int j=radius;j<src.cols-radius;j++)
			{
				//获得中心像素点的灰度值
				uchar center = src.at<uchar>(i,j);
				//根据双线性插值公式计算第k个采样点的灰度值
				uchar neighbor = src.at<uchar>(i+x1,j+y1) * w1 + src.at<uchar>(i+x1,j+y2) *w2 \
					+ src.at<uchar>(i+x2,j+y1) * w3 +src.at<uchar>(i+x2,j+y2) *w4;
				//LBP特征图像的每个邻居的LBP值累加，累加通过与操作完成，对应的LBP值通过移位取得
				dst.at<uchar>(i-radius,j-radius) |= (neighbor>center) <<(neighbors-k-1);
				//进行LBP特征的UniformPattern编码
				if(flag)
				{
					dst.at<uchar>(i-radius,j-radius) = table[dst.at<uchar>(i-radius,j-radius)];
				}
			}
		}
	}
}

//计算一个LBP特征图像块的直方图
Mat CFaceLivingHandler::GetLocalRegionLBPH(const Mat& src,int minValue,int maxValue,bool normed)
{
	//定义存储直方图的矩阵
	Mat result;
	//计算得到直方图bin的数目，直方图数组的大小
	int histSize = maxValue - minValue + 1;
	//定义直方图每一维的bin的变化范围
	float range[] = { static_cast<float>(minValue),static_cast<float>(maxValue + 1) };
	//定义直方图所有bin的变化范围
	const float* ranges = { range };
	//计算直方图，src是要计算直方图的图像，1是要计算直方图的图像数目，0是计算直方图所用的图像的通道序号，从0索引
	//Mat()是要用的掩模，result为输出的直方图，1为输出的直方图的维度，histSize直方图在每一维的变化范围
	//ranges，所有直方图的变化范围（起点和终点）
	calcHist(&src,1,0,Mat(),result,1,&histSize,&ranges,true,false);
	//归一化
	if(normed)
	{
		result /= (int)src.total();
	}
	//结果表示成只有1行的矩阵
	return result.reshape(1,1);
}

//计算LBP特征图像的直方图LBPH
Mat CFaceLivingHandler::GetLBPH(InputArray _src,int numPatterns,int grid_x,int grid_y,bool normed)
{
	Mat src = _src.getMat();
	int width = src.cols / grid_x;
	int height = src.rows / grid_y;
	//定义LBPH的行和列，grid_x*grid_y表示将图像分割成这么些块，numPatterns表示LBP值的模式种类
	Mat result = Mat::zeros(grid_x * grid_y,numPatterns,CV_32FC1);
	if(src.empty())
	{
		return result.reshape(1,1);
	}
	int resultRowIndex = 0;
	//对图像进行分割，分割成grid_x*grid_y块，grid_x，grid_y默认为8
	for(int i=0;i<grid_x;i++)
	{
		for(int j=0;j<grid_y;j++)
		{
			//图像分块
			Mat src_cell = Mat(src,Range(i*height,(i+1)*height),Range(j*width,(j+1)*width));
			//计算直方图
			Mat hist_cell = GetLocalRegionLBPH(src_cell,0,(numPatterns-1),normed);
			//将直方图放到result中
			Mat rowResult = result.row(resultRowIndex);
			hist_cell.reshape(1,1).convertTo(rowResult,CV_32FC1);
			resultRowIndex++;
		}
	}
	return result.reshape(1,1);
}