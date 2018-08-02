#include "stdAfx.h"
#include <stdio.h>
#include <opencv2/opencv.hpp>
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


float* Mat2data(cv::Mat img)
{
	int w = img.cols;
	int h = img.rows;
	int c = img.channels();

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

			return data;
}

int main(int argc, char* argv[])
{
	/*
	if (!loadDll())
	{
		fprintf(stderr, "loadDll failed.\n");
		return -1;
	}
	*/
	CIEIfaceliving CheckLiving;
	char weights_path[] = "E:\\faceDetect\\CIEIfaceliving.weights";
	const char key_path[] = "E:\\faceDetect\\CIEIfaceliving.key";
	int load_wi = CheckLiving.load_weight(weights_path, key_path);
	printf("CheckLiving.load_weight = %d.\n", load_wi);
	return -1;

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

	if(!capColor.open(1) && !capColor.isOpened())
	{
		fprintf(stderr, "Can not opened capColor\n");
		system("pause");
		return -1;
	}

	if(!capBW.open(0) && !capBW.isOpened())
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
			if (x < 0 || y < 0 || w >= gray.cols || h >= gray.rows)
				continue;
			Mat face = gray(Rect(x,y,w,h));
			cv::resize(face, face, Size(64, 64));
			float* face_data = Mat2data(face);
			int result = CheckLiving.detector(face_data);
			printf("(%d)result = CheckLiving.detector(face_data)\n", result);
			if (result)
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

			p = ((short*)(pResultsColor + 1));
			x = p[0];
			y = p[1];
			w = p[2];
			h = p[3];
			neighbors = p[4];
			angle = p[5];


			//printf("face_rect=[%d, %d, %d, %d], neighbors=%d, angle=%d\n", x, y, w, h, neighbors, angle);

			rectangle(result_frontalColor, Rect(x, y, w, h), Scalar(0, 255, 0), 2);

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