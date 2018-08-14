// Color2BW.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Color2BW.h"
#include <Windows.h>
 #include<fstream>
#include <opencv2/opencv.hpp>
using namespace cv;

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

// CColor2BWApp

BEGIN_MESSAGE_MAP(CColor2BWApp, CWinApp)
END_MESSAGE_MAP()


// CColor2BWApp 构造

CColor2BWApp::CColor2BWApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CColor2BWApp 对象

CColor2BWApp theApp;


// CColor2BWApp 初始化

BOOL CColor2BWApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int  mat_to_binary( const  cv::Mat img,  int  line_byte,  char * data)
{
	int  width = img.cols;
	int  height = img.rows;
	size_t  line_size = line_byte * 8;
	size_t  bit_size = line_size * height;


	char  *p = data;  int  offset, v; unsigned  char  temp;
	for ( int  row=height-1; row>=0; row-- ) {       
		for  ( int  col=0; col<width; col++ ) {  
			offset = col % 8;
			v = img.at<uchar>(row, col);
			temp = 1;
			temp = temp << (8 - offset -1);
			if (v == 255 ) {                
				*(p + col/8) |= temp;
			}  else  {
				temp = ~temp;
				*(p + col/8) &= temp;
			}
		}
		for ( int  j = width/8 ; j < line_byte; j++)
			p[j] = 0;
		p = p + line_byte;
	}
	return  0;
}

int  save_bmp_image( const  cv::Mat img, std::string dst) 
{
	int  width = img.cols;
	int  height = img.rows;
	const  int  biBitCount = 1;


	//颜色表大小，以字节为单位，灰度图像颜色表为256*4字节，彩色图像颜色表大小为0,二值图为2*4
	int  color_type_num = 2;
	int  colorTablesize = color_type_num *  sizeof (RGBQUAD);
	RGBQUAD *pColorTable =  new  RGBQUAD[color_type_num];
	for ( int  i = 0; i < color_type_num; i++) {
		pColorTable[i].rgbBlue = i*255;
		pColorTable[i].rgbRed  = i*255;
		pColorTable[i].rgbGreen= i*255;
		pColorTable[i].rgbReserved = 0;
	}


	//待存储图像数据每行字节数为4的倍数
	int  line_byte = (width * biBitCount/8+3)/4*4;
	char * p_data = ( char *) malloc (line_byte*height);
	mat_to_binary(img, line_byte, p_data);


	std::ofstream fp(dst.c_str(), std::ios::binary | std::ios::out);


	if (!fp.is_open()) {
		// cout <<  "open "  << dst << " failed!"  << endl;
		return  -2;
	}


	//申请位图文件头结构变量，填写文件头信息
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;   //bmp类型


	fileHead.bfSize=  sizeof (BITMAPFILEHEADER) +  sizeof (BITMAPINFOHEADER)\
		+ colorTablesize + line_byte*height;               //bfSize是图像文件4个组成部分之和
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;    
	fileHead.bfOffBits = 54+colorTablesize;               //bfOffBits是图像文件前3个部分所需空间之和    


	fp.write(( char *)&fileHead,  sizeof (BITMAPFILEHEADER)); //写文件头进文件


	//申请位图信息头结构变量，填写信息头信息
	BITMAPINFOHEADER head; 
	head.biBitCount = biBitCount;
	head.biClrImportant = 0;
	head.biClrUsed = 0;
	head.biCompression = 0;
	head.biHeight = height;
	head.biPlanes = 1;
	head.biSize = 40;
	head.biSizeImage = line_byte*height;
	head.biWidth = width;
	head.biXPelsPerMeter = 0;
	head.biYPelsPerMeter = 0;


	//写位图信息头进内存  
	fp.write(( char *)&head,  sizeof (BITMAPINFOHEADER));


	//颜色表，写入文件 
	fp.write(( char *)pColorTable,  sizeof (RGBQUAD)*color_type_num);  


	//写位图数据进文件pBmpBuf
	fp.write(( char *)p_data, height*line_byte);
	fp.close();


	delete  []pColorTable;
	delete  []p_data;
	return  0;
}

//参数szSrcPath:输入图片地址
//参数szDstPath:输出图片地址
int Color2BW(const char * szSrcPath,const char * szDstPath)
{
	CString strSrcPath(szSrcPath);
	string strDstPath(szDstPath);
	if( !PathFileExists(strSrcPath) )
		return -1;

	Mat matSrc;
	matSrc = imread(szSrcPath);

	//彩色转灰色
	cvtColor(matSrc,matSrc,CV_BGR2GRAY); 
	//灰色转二值化
	threshold(matSrc, matSrc, 177, 255, CV_THRESH_BINARY);

	//位深度转1
	int nRet = save_bmp_image( matSrc, strDstPath);
	if (nRet != 0)
	{
		return nRet;
	}
	
	return 0;
}