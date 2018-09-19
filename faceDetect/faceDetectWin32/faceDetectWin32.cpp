#include "stdAfx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/ml/ml.hpp>
#include <vector>
#include "facedetect-dll.h"

#pragma comment(lib,"libfacedetect.lib")

#include<windows.h>

#include "CIEIfaceliving.h"

#pragma comment(lib,"CIEIfaceliving.lib")

using namespace std;
using namespace cv;

#include<WinDef.h>

#define DETECT_BUFFER_SIZE 0x20000
#define UNKNOWN_FLOW_THRESH 1e9 

/*
void setup_detector();
bool load_weight(const char* path,const char* path_key);
bool detector(float* face_img);
*/
/*
typedef bool (* FuncLoadWeight)(const char* ,const char* );
typedef bool (* FuncDetector)(float*);

HINSTANCE m_hinstacne = NULL;

FuncLoadWeight	loadWeight	= NULL;
FuncDetector	detector	= NULL;

void freeDll()
{
	if (m_hinstacne)
	{
		::FreeLibrary(m_hinstacne);
		m_hinstacne = NULL;
	}
}

bool loadDll()
{
	bool bRet = false;
	do 
	{
		fprintf(stderr, "loadDll start.\n");
		m_hinstacne = ::LoadLibrary("livingDetection.dll");
		//m_hinstacne = ::LoadLibrary("CIEIfaceliving.dll");
		if (m_hinstacne == NULL) 
		{

			fprintf(stderr,"m_hinstacne == NULL.\n");
			break;
		}

		loadWeight = (FuncLoadWeight)::GetProcAddress(m_hinstacne, "load_weight");
		if (loadWeight == NULL)
		{
			fprintf(stderr, "loadWeight == NULL.\n");
			break;
		}

		detector = (FuncDetector)::GetProcAddress(m_hinstacne, "detector");
		if (detector == NULL)
		{
			fprintf(stderr, "detector == NULL.\n");
			break;
		}

		bRet = true;

	} while (0);

	if ( !bRet)
	{
		freeDll();
	}

	return bRet;
}

*/


void makecolorwheel(vector<Scalar> &colorwheel)
{
	int RY = 15;
	int YG = 6;
	int GC = 4;
	int CB = 11;
	int BM = 13;
	int MR = 6;

	int i;

	for (i = 0; i < RY; i++) colorwheel.push_back(Scalar(255,	   255*i/RY,	 0));
	for (i = 0; i < YG; i++) colorwheel.push_back(Scalar(255-255*i/YG, 255,		 0));
	for (i = 0; i < GC; i++) colorwheel.push_back(Scalar(0,		   255,		 255*i/GC));
	for (i = 0; i < CB; i++) colorwheel.push_back(Scalar(0,		   255-255*i/CB, 255));
	for (i = 0; i < BM; i++) colorwheel.push_back(Scalar(255*i/BM,	   0,		 255));
	for (i = 0; i < MR; i++) colorwheel.push_back(Scalar(255,	   0,		 255-255*i/MR));
}


void motionToColor(Mat flow, Mat &color)
{
	if (color.empty())
		color.create(flow.rows, flow.cols, CV_8UC3);

	static vector<Scalar> colorwheel; //Scalar r,g,b
	if (colorwheel.empty())
		makecolorwheel(colorwheel);

	// determine motion range:
	float maxrad = -1;

	// Find max flow to normalize fx and fy
	for (int i= 0; i < flow.rows; ++i) 
	{
		for (int j = 0; j < flow.cols; ++j) 
		{
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);
			float fx = flow_at_point[0];
			float fy = flow_at_point[1];
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
				continue;
			float rad = sqrt(fx * fx + fy * fy);
			maxrad = maxrad > rad ? maxrad : rad;
		}
	}

	for (int i= 0; i < flow.rows; ++i) 
	{
		for (int j = 0; j < flow.cols; ++j) 
		{
			uchar *data = color.data + color.step[0] * i + color.step[1] * j;
			Vec2f flow_at_point = flow.at<Vec2f>(i, j);

			float fx = flow_at_point[0] / maxrad;
			float fy = flow_at_point[1] / maxrad;
			if ((fabs(fx) >  UNKNOWN_FLOW_THRESH) || (fabs(fy) >  UNKNOWN_FLOW_THRESH))
			{
				data[0] = data[1] = data[2] = 0;
				continue;
			}
			float rad = sqrt(fx * fx + fy * fy);

			float angle = atan2(-fy, -fx) / CV_PI;
			float fk = (angle + 1.0) / 2.0 * (colorwheel.size()-1);
			int k0 = (int)fk;
			int k1 = (k0 + 1) % colorwheel.size();
			float f = fk - k0;
			//f = 0; // uncomment to see original color wheel

			for (int b = 0; b < 3; b++) 
			{
				float col0 = colorwheel[k0][b] / 255.0;
				float col1 = colorwheel[k1][b] / 255.0;
				float col = (1 - f) * col0 + f * col1;
				if (rad <= 1)
					col = 1 - rad * (1 - col); // increase saturation with radius
				else
					col *= .75; // out of range
				data[2 - b] = (int)(255.0 * col);
			}
		}
	}
}

float fAverage(float *f, int n)
{
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += f[i];
	}
	return sum/n;
}

float fMD(float *f, int n)
{
	float fAve = fAverage(f,n);
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += abs(f[i] - fAve);
	}
	return sum/n;
}

float fD(float *f, int n)
{
	float fAve = fAverage(f,n);
	float sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += (f[i] - fAve)*(f[i] - fAve);
	}
	return sum/n;
}

float fSD(float *f, int n)
{
	return sqrt(fD(f,n));
}


float m_fdata[2][5000] = {{0},{0}};
float* Mat2data(cv::Mat img,string strName,int &nCount)
{
	int w = img.cols;
	int h = img.rows;
	int c = img.channels();
	//printf("w=%d,h=%d,c=%d  %d",w,h,c,w*h*c);

	float min = FLT_MAX;
	float max = -FLT_MAX;

	float* data = (float*)calloc(w*h*c, sizeof(float));
	int count = 0;
	for (int k = 0; k < c; ++k)
		for (int i = 0; i < h; ++i)
			for (int j = 0; j < w; ++j)
			{
				float pixel = (float)img.data[(i*w + j)*c + k];
				if (pixel > max)
					max = pixel;
				if (pixel < min)
					min = pixel;

				data[count++] = pixel;
			}

	for (int i = 0; i < count; ++i)
		data[i] = (data[i] - min) / (max - min);

	nCount = count;
	//printf("\t%d\n",nCount);
	/*
	float* fdata = (float*)calloc(w*h*c, sizeof(float));
	for (int i = 0; i < count; i++)
	{
		fdata[i] = data[i] * (max - min) + min;
	}
	
	count = 0;
	Mat fMat;
	for (int k = 0; k < c; ++k)
	{
		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				uchar d = (unsigned char)(int)fdata[count++];
				//fMat.data[(i*w + j)*c + k] = d;
				fMat.ptr<uchar>(i*w)[j*c + k] = d;
			}
		}
	}

	free(fdata);

	imshow(strName,fMat);
	*/
	/*
	printf("\n输入");
	int intput;
	cin >> intput;

	if (intput == 2)
	{
		
		for (int j = 0; j < count; j++)
		{
			printf("m_fdata[0][%d]=%f\tm_fdata[1][%d]=%f\n",j,m_fdata[0][j],j,m_fdata[1][j]);
			
		}
		printf("0 平均数:%f,平均差:%f,方差:%f,标准差:%f\n",fAverage(m_fdata[0],count),fMD(m_fdata[0],count),fD(m_fdata[0],count),fSD(m_fdata[0],count));	
		printf("1 平均数:%f,平均差:%f,方差:%f,标准差:%f\n",fAverage(m_fdata[1],count),fMD(m_fdata[1],count),fD(m_fdata[1],count),fSD(m_fdata[1],count));	
		
	}
	else
	{
		if (strName == "黑白活检")
		{
			for (int i = 0; i < 64; i++)
			{
				for (int j = 0; j < 64; j++)
				{
					printf("%d ",(int)data[i*64+j]*10);
				}
				printf("\n");
				
				//m_fdata[intput][i] = data[i];
			}
		}

	}
	
	*/
	return data;
}

bool faceLiving(float *f, int count)
{
	float f2[9] = {0};
	for (int i = 0; i < 9; i++)
	{
		f2[i] = f[i*count/9];
	}
	float fRet = fD(f,count);
	//printf("方差:%f",fRet);
	printf("平均数:%f,平均差:%f,方差:%f,标准差:%f\n",fAverage(f,count),fMD(f,count),fD(f,count),fSD(f,count));	
	if (fRet < 0.06)
	{
		return false;
	}
	else
	{
		return true;
	}
}

//原始LBP特征计算
//template <typename _tp>
void getOriginLBPFeature(InputArray _src,OutputArray _dst)
{
	Mat src = _src.getMat();
	_dst.create(src.rows-2,src.cols-2,CV_8UC1);
	Mat dst = _dst.getMat();
	dst.setTo(0);
	
	for(int i=1;i<src.rows-1;i++)
	{
		for(int j=1;j<src.cols-1;j++)
		{
			uchar center = src.at<uchar>(i,j);
			//_tp center = src.at<_tp>(i,j);
			uchar lbpCode = 0;
			lbpCode |= (src.at<uchar>(i-1,j-1) > center) << 7;
			lbpCode |= (src.at<uchar>(i-1,j  ) > center) << 6;
			lbpCode |= (src.at<uchar>(i-1,j+1) > center) << 5;
			lbpCode |= (src.at<uchar>(i  ,j+1) > center) << 4;
			lbpCode |= (src.at<uchar>(i+1,j+1) > center) << 3;
			lbpCode |= (src.at<uchar>(i+1,j  ) > center) << 2;
			lbpCode |= (src.at<uchar>(i+1,j-1) > center) << 1;
			lbpCode |= (src.at<uchar>(i  ,j-1) > center) << 0;
			dst.at<uchar>(i-1,j-1) = lbpCode;
			
			//circle(src,Point(j,i),1,Scalar(255,255,255),2);
			
		}
		//imshow("src",src);
		imshow("dst",dst);
		//waitKey();
	}
}

//圆形LBP特征计算，效率优化版本，声明时默认neighbors=8
//template <typename _tp>
void getCircularLBPFeatureOptimization(InputArray _src,OutputArray _dst,int radius,int neighbors)
{
	Mat src = _src.getMat();
	//LBP特征图像的行数和列数的计算要准确
	_dst.create(src.rows-2*radius,src.cols-2*radius,CV_8UC1);
	Mat dst = _dst.getMat();
	dst.setTo(0);
	for(int k=0;k<neighbors;k++)
	{
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
			}
		}
	}
}
//旋转不变圆形LBP特征计算，声明时默认neighbors=8
//template <typename _tp>
void getRotationInvariantLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors)
{
	Mat src = _src.getMat();
	//LBP特征图像的行数和列数的计算要准确
	_dst.create(src.rows-2*radius,src.cols-2*radius,CV_8UC1);
	Mat dst = _dst.getMat();
	dst.setTo(0);
	for(int k=0;k<neighbors;k++)
	{
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
			}
		}
	}
	
	//进行旋转不变处理
	for(int i=0;i<dst.rows;i++)
	{
		for(int j=0;j<dst.cols;j++)
		{
			uchar currentValue = dst.at<uchar>(i,j);
			uchar minValue = currentValue;
			for(int k=1;k<neighbors;k++)
			{
				//循环左移
				uchar temp = (currentValue>>(neighbors-k)) | (currentValue<<k);
				if(temp < minValue)
				{
					minValue = temp;
				}
			}
			dst.at<uchar>(i,j) = minValue;
		}
	}
	
}
#include <bitset>

//计算跳变次数
int getHopTimes(int n)
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
//template <typename _tp>
void getUniformPatternLBPFeature(InputArray _src,OutputArray _dst,int radius,int neighbors)
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
		if(getHopTimes(i)<3)
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
Mat getLocalRegionLBPH(const Mat& src,int minValue,int maxValue,bool normed)
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
Mat getLBPH(InputArray _src,int numPatterns,int grid_x,int grid_y,bool normed)
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
			Mat hist_cell = getLocalRegionLBPH(src_cell,0,(numPatterns-1),normed);
			//将直方图放到result中
			Mat rowResult = result.row(resultRowIndex);
			hist_cell.reshape(1,1).convertTo(rowResult,CV_32FC1);
			resultRowIndex++;
		}
	}
	return result.reshape(1,1);
}


bool faceLiveMat(Mat face)
{
	int w = face.cols;
	int h = face.rows;
	int c = face.channels();
	//printf("w=%d,h=%d,c=%d  %d\n",w,h,c,w*h*c);

	float min = FLT_MAX;
	float max = -FLT_MAX;

	//float* data = (float*)calloc(w*h*c, sizeof(float));
	float* data = new float[w*h*c];
	int count = 0;
	for (int k = 0; k < c; ++k)
	{
		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				float pixel = (float)face.data[(i*w + j)*c + k];
				if (pixel > max)
					max = pixel;
				if (pixel < min)
					min = pixel;

				data[count++] = pixel;
			}
		}
	}
	
	for (int i = 0; i < count; ++i)
		data[i] = (data[i] - min) / (max - min);
	

	
	printf("平均数:%f,平均差:%f,方差:%f,标准差:%f,个数:%d\n",fAverage(data,count),fMD(data,count),fD(data,count),fSD(data,count),count);

	float fDr = fD(data,count);
	/*
	for (int i = 2; i < h; i+=5)
	{
		for (int j = 2; j < w; j+=5)
		{
			circle(face, Point(j, i), 1, Scalar(0, 255, 0));
			//GaussianBlur();
		}
	}
	*/
	//imshow("face",face);

	delete []data;

	return fDr>0.06?true:false;

}

Mat getHistogram(const Mat &image)
{
	Mat hist;
	int histSize = 256;
	float range[] = { 0,256 };
	const float *histRanges = { range };
	int channels[] = {0,1};
	calcHist(&image,1,channels,Mat(),hist,1,&histSize,&histRanges);
	//calcHist(&image, 1, 0, Mat(), hist, 1, &histSize, &histRanges, true, false);

	double maxVal = 0;
	double minVal = 0;
	minMaxLoc(hist, &minVal, &maxVal);

	histSize = hist.rows;

	Mat histImg(histSize,histSize,CV_8U,Scalar(255));

	int hpt = static_cast<int> (0.9*histSize);

	for (int h = 0; h < histSize; h++)
	{
		float binVal = hist.at<float>(h);
		if (binVal)
		{
			int intensity = static_cast<int> (binVal*hpt / maxVal);
			line(histImg,Point(h,histSize),Point(h,histSize-intensity),Scalar(0),1);
		}
	}

	return histImg;
}



void getImageFace(Mat& trainingImages, vector<int>& trainingLabels)
{
	for (int i = 1;i <= 10;i++)  
	{  
		char szBuf[MAX_PATH] = {0};
		sprintf(szBuf,"C:\\Users\\Administrator\\Desktop\\FACE\\%d.BMP",i);
		Mat SrcImage = imread(szBuf);

		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!pBuffer)
		{
			fprintf(stderr, "Can not alloc pBuffer.\n");
			system("pause");
			return ;
		}

		Mat gray;
		cvtColor(SrcImage, gray, CV_BGR2GRAY);
		int * pResults = NULL; 
		int doLandmark = 0;
		pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 2, 48, 0, doLandmark);

		if ( !(pResults && *pResults == 1) )
		{
			fprintf(stderr, "i = %d !(pResults && *pResults == 1)\n",i);
			continue;
		}

		short * p = ((short*)(pResults+1));
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		if (x <= 0 || y <= 0 || w >= gray.cols || h >= gray.rows)
			continue;
		Mat face = gray(Rect(x,y,w,h));
		resize(face, face, Size(64, 64));
		Mat lbp;
		getUniformPatternLBPFeature(face,lbp,1,8);
		lbp = getLBPH(lbp,59,3,3,true);
		
		//SrcImage = SrcImage.reshape(1, 1);
		trainingImages.push_back(lbp);
		if (i <=5)
		{
			trainingLabels.push_back(0);
		}
		else
		{
			trainingLabels.push_back(1);
		}
		
	}  
}

void testSvm3()
{

	CvSVM svm;
	svm.clear();
	string modelpath = "svm.xml";
	svm.load(modelpath.c_str());

	for (int i = 1;i <= 10;i++)  
	{  
		char szBuf[MAX_PATH] = {0};
		sprintf(szBuf,"C:\\Users\\Administrator\\Desktop\\FACE\\%d.BMP",i);
		Mat SrcImage = imread(szBuf);

		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!pBuffer)
		{
			fprintf(stderr, "Can not alloc pBuffer.\n");
			system("pause");
			return ;
		}

		Mat gray;
		cvtColor(SrcImage, gray, CV_BGR2GRAY);
		int * pResults = NULL; 
		int doLandmark = 0;
		pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
			1.2f, 2, 48, 0, doLandmark);

		if ( !(pResults && *pResults == 1) )
		{
			fprintf(stderr, "!(pResults && *pResults == 1)\n");
			break;
		}

		short * p = ((short*)(pResults+1));
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		if (x <= 0 || y <= 0 || w >= gray.cols || h >= gray.rows)
			continue;
		Mat face = gray(Rect(x,y,w,h));
		resize(face, face, Size(64, 64));
		Mat lbp;
		getUniformPatternLBPFeature(face,lbp,1,8);
		lbp = getLBPH(lbp,59,3,3,true);

		int response = (int)svm.predict(lbp);
		fprintf(stderr, "path = %s,response = %d\n",szBuf,response);

	}  
	system("pause");
	return;
	
}

void testSvm2()
{
	//获取训练数据
	Mat classes;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;
	getImageFace(trainingImages, trainingLabels);
	Mat(trainingImages).copyTo(trainingData);
	trainingData.convertTo(trainingData, CV_32FC1);
	Mat(trainingLabels).copyTo(classes);

	for (int i = 1;i <= 10;i++)  
	{  
		char szBuf[MAX_PATH] = {0};
		sprintf(szBuf,"C:\\Users\\Administrator\\Desktop\\FACE\\%d.BMP",i);
		Mat SrcImage = imread(szBuf);

		unsigned char * pBuffer = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
		if(!pBuffer)
		{
			fprintf(stderr, "Can not alloc pBuffer.\n");
			system("pause");
			return ;     
		}

		//cvtColor(SrcImage, cielab, CV_BGR2Lab);

	}

	//配置SVM训练器参数
	CvSVMParams SVM_params;
	SVM_params.svm_type = CvSVM::C_SVC;//C类支撑向量分类机
	SVM_params.kernel_type = CvSVM::LINEAR; //线性内核
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
	svm.save("LivingDetectLBPH.xml");
	cout<<"LivingDetectLBPH训练好了！！！"<<endl;
	system("pause");
}


	
void testSvm1()
{
	//获取训练数据
	Mat classes;
	Mat trainingData;
	Mat trainingImages;
	vector<int> trainingLabels;
	getImageFace(trainingImages, trainingLabels);
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
	svm.save("LivingDetectLBPH.xml");
	cout<<"训练好了！！！"<<endl;
	system("pause");
}

int main(int argc, char* argv[])
{
	//testSvm2();

	//system("pause");

	int nFaceCount = GetPrivateProfileInt("setting","num",0,"C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\faceDetect\\FaceImage\\config.ini");


	//摄像头取反用
	int cameraColor = 0;
	int cameraBW = 1 - cameraColor;

	CvSVM svm;
	svm.clear();
	string modelpath = "svm.xml";
	svm.load(modelpath.c_str());
	/*
	float f[] = {1,2,3,4,5,6,7,8};
	printf("平均数:%f,平均差:%f,方差:%f,标准差:%f",fAverage(f,8),fMD(f,8),fD(f,8),fSD(f,8));

	*/
	//system("pause");
	/*
	if (!loadDll())
	{
		fprintf(stderr, "loadDll failed.\n");
		return -1;
	}
	*/

	
	CIEIfaceliving CheckLiving;
	char weights_path[] = "..\\Release\\CIEIfaceliving.weights";
	const char key_path[] = "..\\Release\\CIEIfaceliving.key";
	int load_wi = CheckLiving.load_weight(weights_path, key_path);
	printf("CheckLiving.load_weight = %d.\n", load_wi);
	
	//return -1;

	unsigned char * pBufferColor = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if(!pBufferColor)
	{
		fprintf(stderr, "Can not alloc pBufferColor.\n");
		system("pause");
		return -1;
	}

	unsigned char * pBufferBW = (unsigned char *)malloc(DETECT_BUFFER_SIZE);
	if(!pBufferBW)
	{
		fprintf(stderr, "Can not alloc pBufferBW.\n");
		system("pause");
		return -1;
	}
	
	VideoCapture capColor;
	VideoCapture capBW;

	if(!capColor.open(0) && !capColor.isOpened())
	{
		fprintf(stderr, "Can not opened capColor\n");
		system("pause");
		return -1;
	}

	if(!capBW.open(1) && !capBW.isOpened())
	{
		fprintf(stderr, "Can not opened capBW\n");
		system("pause");
		return -1;
	}

	Mat imageColor; //彩色
	Mat imageBW; //黑白
	Mat gray;

	Mat prevgray;
	//Rect rectFace(0,0,0,0);

	Mat motion2color;

	Mat matSaveBW;
	Mat matSaveColor;

	Mat result_frontalBW;
	Mat result_frontalColor;
	Mat result_calcOpticalFlow;

	vector<Point2f> point1, point2, pointCopy;
	vector<uchar> status;
	vector<float> err;
	/*
	CIEIfaceliving CheckLiving;
	const char weights_path[] = "faceliving.weights";
	const char key_path[] = "key.txt";
	int load_wi=CheckLiving.load_weight(weights_path, key_path);
	printf("CheckLiving.load_weight = %d.\n", load_wi);
	if (1 != load_wi)
	{
		system("pause");
		return -1;
	}
	*/
	while(1) 
	{ 
		bool bCamera = true;
		//string strPath = "E:\\libfacedetection-master\\images\\4.jpg";
		//Mat image = imread(strPath);


		do 
		{
			capColor.read(imageColor);
			if(imageColor.empty())
			{
				fprintf(stderr, "Can not load the 彩色image0 file %s.\n", "null");
				//system("pause");
				bCamera = false;
				break;
			}
		
			capBW.read(imageBW); 
			if(imageBW.empty())
			{
				fprintf(stderr, "Can not load the 黑白image1 file %s.\n", "null");
				//system("pause");
				bCamera = false;
				break;
			}

			matSaveBW = imageBW.clone();
			matSaveColor = imageColor.clone();

			result_frontalBW = imageBW.clone();
			result_frontalColor = imageColor.clone();

			cvtColor(imageBW, gray, CV_BGR2GRAY);
			int * pResultsBW = NULL; 
			int doLandmark = 0;
			pResultsBW = facedetect_multiview(pBufferBW, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
				1.2f, 2, 48, 0, doLandmark);

			//printf("黑白%d faces detected.\n", (pResultsBW ? *pResultsBW : 0));
		
		
			if ( !(pResultsBW && *pResultsBW == 1) )
			{
				//fprintf(stderr, "!(pResultsBW && *pResultsBW == 1)\n");
				break;
			}
			
			short * p = ((short*)(pResultsBW+1));
			int x = p[0];
			int y = p[1];
			int w = p[2];
			int h = p[3];
			int neighbors = p[4];
			int angle = p[5];

			
			//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
			if (x <= 1 || y <= 1 || x + w >= gray.cols -1 || y + h >= gray.rows -1)
			{
				//MessageBox(NULL,"BW 出界",NULL,NULL);
				continue;
			}
			//fprintf(stderr, "BW x=%d y=%d w=%d h=%d gray.cols=%d gray.rows=%d\n",x,y,w,h,gray.cols,gray.rows);
			Mat face = gray(Rect(x,y,w,h));
			resize(face, face, Size(64, 64));
			int dataCount = 0;

			float* face_data = Mat2data(face,"黑白活检",dataCount);
			bool bResult = false;
			bResult = CheckLiving.detector(face_data);
			//printf("(%d)result = CheckLiving.detector(face_data)\n", bResult);
			Mat lbp;

			//getOriginLBPFeature(face,lbp);
			//getRotationInvariantLBPFeature(face,lbp,3,8);
			//getCircularLBPFeatureOptimization(face,lbp,1,8);
			getUniformPatternLBPFeature(face,lbp,1,8);
			//imshow("lbp",lbp);
			//lbp = getHistogram(lbp);
			lbp = getLBPH(lbp,59,3,3,true);
			//imshow("lbph",lbp);

			int response = (int)svm.predict(lbp);

			fprintf(stderr, "check response = %d \n",response);

			//bResult = faceLiveMat(lbp);

			if (bResult)
			{
				rectangle(result_frontalBW, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
			}
			else
			{
				rectangle(result_frontalBW, Rect(x, y, w, h), Scalar(0, 0, 255), 2);
			}

			//眼眶36-47
			if (doLandmark)
			{
				for (int j = 36; j < 48; j++)
				{
					point1.push_back(Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1])) ;
					circle(result_frontalBW, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
				}
			}
			
			/*
			if( prevgray.data )  
			{  
				calcOpticalFlowFarneback(prevgray, gray, result_calcOpticalFlow, 0.5, 3, 15, 3, 5, 1.2, 0);  
				motionToColor(result_calcOpticalFlow, motion2color);  
				imshow("光流场flow", motion2color);  
			}  

			swap(prevgray, gray); 
			*/
			/*
			int tr_num = 0;
			vector<unsigned char>::iterator status_itr = status.begin();
			while (status_itr != status.end()) 
			{
				if (*status_itr > 0)
					tr_num++;

				status_itr++;
			}

			if (tr_num < 6) 
			{
				//printf("===============tr_num = %d\n", tr_num);
			}
			printf("===============tr_num = %d\n", tr_num);
			if (prevgray.data)
			{
				calcOpticalFlowPyrLK(prevgray,gray,point1,point2,status,err,Size(100,100),5);
				result_calcOpticalFlow = imageBW.clone();
				for (int i = 0; i < point2.size(); i++)
				{
					circle(result_calcOpticalFlow, point2[i], 1, Scalar(0, 0, 255), 2);
					line(result_calcOpticalFlow, point1[i], point2[i], Scalar(255, 0, 0), 2);
				}
				imshow("光流场flow", result_calcOpticalFlow);  
				point1.clear();
				
			}
			swap(prevgray, gray);
			*/
			cvtColor(imageColor, gray, CV_BGR2GRAY);

			int * pResultsColor = NULL; 
			doLandmark = 1;
			//facedetect_frontal//正面
			//facedetect_frontal_surveillance//正面监督
			//facedetect_multiview//多视角
			//facedetect_multiview_reinforce//多视角增强
			pResultsColor = facedetect_frontal_surveillance(pBufferColor, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
				1.2f, 2, 48, 0, doLandmark);

			//printf("彩色%d faces detected.\n", (pBufferColor ? *pBufferColor : 0));
		
		
			if ( !(pResultsColor && *pResultsColor == 1) )
			{
				//fprintf(stderr, "!(pResultsColor && *pResultsColor == 1)\n");
				break;
			}

			char szFaceName[MAX_PATH] = {0};
			sprintf(szFaceName,"C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\faceDetect\\FaceImage\\bw%d.bmp",nFaceCount);
			imwrite(szFaceName,matSaveBW);
			sprintf(szFaceName,"C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\faceDetect\\FaceImage\\color%d.bmp",nFaceCount++);
			imwrite(szFaceName,matSaveColor);

			char szFaceCount[16] = {0};
			sprintf(szFaceCount,"%d",nFaceCount);
			WritePrivateProfileString("setting","num",szFaceCount,"C:\\Users\\Administrator\\Documents\\Visual Studio 2010\\Projects\\faceDetect\\FaceImage\\config.ini");

			p = ((short*)(pResultsColor + 1));
			x = p[0];
			y = p[1];
			w = p[2];
			h = p[3];
			neighbors = p[4];
			angle = p[5];

			if (x <= 1 || y <= 1 || x + w >= gray.cols -1 || y + h >= gray.rows -1)
			{
				//MessageBox(NULL,"color 出界",NULL,NULL);
				continue;
			}
			//fprintf(stderr, "color x=%d y=%d w=%d h=%d gray.cols=%d gray.rows=%d\n",x,y,w,h,gray.cols,gray.rows);
			//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
			face = gray(Rect(x,y,w,h));
			//cv::resize(face, face, Size(64, 64));
			face_data = Mat2data(face,"彩色活检",dataCount);
			bResult = CheckLiving.detector(face_data);
			//printf("(%d)result = CheckLiving.detector(face_data)\n", result);
			//bResult = false;//faceLiveMat(face);
			if (bResult)
			{
				rectangle(result_frontalColor, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

			}
			else
			{
				rectangle(result_frontalColor, Rect(x, y, w, h), Scalar(0, 0, 255), 2);
			}
			//rectangle(result_frontalColor, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

			if (doLandmark)
			{
				for (int j = 0; j < 68; j++)
					circle(result_frontalColor, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
			}

			/*
			for(int i = 0; i < (pResults0 ? *pResults0 : 0); i++)
			{
				short * p = ((short*)(pResults0+1))+142*i;
				int x = p[0];
				int y = p[1];
				int w = p[2];
				int h = p[3];
				int neighbors = p[4];
				int angle = p[5];

				printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);

				rectangle(result_frontal0, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

				if (doLandmark)
				{
					for (int j = 0; j < 68; j++)
						circle(result_frontal0, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
				}

			}
			*/
		
			

		} while (0);

		if (bCamera)
		{
			imshow("黑白Results_frontal", result_frontalBW);

			imshow("彩色Results_frontal", result_frontalColor);

			
		}

		if (waitKey(30) == 27)
		{
			return 0;
		}
		
	}
	/*
	///////////////////////////////////////////
	// frontal face detection designed for video surveillance / 68 landmark detection
	// it can detect faces with bad illumination.
	//////////////////////////////////////////
	//!!! The input image must be a gray one (single-channel)
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_frontal_surveillance(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		1.2f, 2, 48, 0, doLandmark);
	printf("%d faces detected.\n", (pResults ? *pResults : 0));
	Mat result_frontal_surveillance = image.clone();;
	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults+1))+142*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		rectangle(result_frontal_surveillance, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		if (doLandmark)
		{
			for (int j = 0; j < 68; j++)
				circle(result_frontal_surveillance, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
		}
	}
	imshow("Results_frontal_surveillance", result_frontal_surveillance);


	///////////////////////////////////////////
	// multiview face detection / 68 landmark detection
	// it can detect side view faces, but slower than facedetect_frontal().
	//////////////////////////////////////////
	//!!! The input image must be a gray one (single-channel)
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_multiview(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		1.2f, 2, 48, 0, doLandmark);

	printf("%d faces detected.\n", (pResults ? *pResults : 0));
	Mat result_multiview = image.clone();;
	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults+1))+142*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);
		rectangle(result_multiview, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		if (doLandmark)
		{
			for (int j = 0; j < 68; j++)
				circle(result_multiview, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
		}
	}
	imshow("Results_multiview", result_multiview);


	///////////////////////////////////////////
	// reinforced multiview face detection / 68 landmark detection
	// it can detect side view faces, better but slower than facedetect_multiview().
	//////////////////////////////////////////
	//!!! The input image must be a gray one (single-channel)
	//!!! DO NOT RELEASE pResults !!!
	pResults = facedetect_multiview_reinforce(pBuffer, (unsigned char*)(gray.ptr(0)), gray.cols, gray.rows, (int)gray.step,
		1.2f, 3, 48, 0, doLandmark);

	printf("%d faces detected.\n", (pResults ? *pResults : 0));
	Mat result_multiview_reinforce = image.clone();;
	//print the detection results
	for(int i = 0; i < (pResults ? *pResults : 0); i++)
	{
		short * p = ((short*)(pResults+1))+142*i;
		int x = p[0];
		int y = p[1];
		int w = p[2];
		int h = p[3];
		int neighbors = p[4];
		int angle = p[5];

		printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x,y,w,h,neighbors, angle);
		rectangle(result_multiview_reinforce, Rect(x, y, w, h), Scalar(0, 255, 0), 2);
		if (doLandmark)
		{
			for (int j = 0; j < 68; j++)
				circle(result_multiview_reinforce, Point((int)p[6 + 2 * j], (int)p[6 + 2 * j + 1]), 1, Scalar(0, 255, 0));
		}
	}
	imshow("Results_multiview_reinforce", result_multiview_reinforce);

	*/
	//waitKey();

	//release the buffer
	free(pBufferColor);
	free(pBufferBW);
	capColor.release();
	capBW.release();
	
	// Closes all the frames
	destroyAllWindows();

	return 0;
}