// Color2BW.cpp : ���� DLL �ĳ�ʼ�����̡�
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

// CColor2BWApp

BEGIN_MESSAGE_MAP(CColor2BWApp, CWinApp)
END_MESSAGE_MAP()


// CColor2BWApp ����

CColor2BWApp::CColor2BWApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CColor2BWApp ����

CColor2BWApp theApp;


// CColor2BWApp ��ʼ��

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


	//��ɫ���С�����ֽ�Ϊ��λ���Ҷ�ͼ����ɫ��Ϊ256*4�ֽڣ���ɫͼ����ɫ���СΪ0,��ֵͼΪ2*4
	int  color_type_num = 2;
	int  colorTablesize = color_type_num *  sizeof (RGBQUAD);
	RGBQUAD *pColorTable =  new  RGBQUAD[color_type_num];
	for ( int  i = 0; i < color_type_num; i++) {
		pColorTable[i].rgbBlue = i*255;
		pColorTable[i].rgbRed  = i*255;
		pColorTable[i].rgbGreen= i*255;
		pColorTable[i].rgbReserved = 0;
	}


	//���洢ͼ������ÿ���ֽ���Ϊ4�ı���
	int  line_byte = (width * biBitCount/8+3)/4*4;
	char * p_data = ( char *) malloc (line_byte*height);
	mat_to_binary(img, line_byte, p_data);


	std::ofstream fp(dst.c_str(), std::ios::binary | std::ios::out);


	if (!fp.is_open()) {
		// cout <<  "open "  << dst << " failed!"  << endl;
		return  -2;
	}


	//����λͼ�ļ�ͷ�ṹ��������д�ļ�ͷ��Ϣ
	BITMAPFILEHEADER fileHead;
	fileHead.bfType = 0x4D42;   //bmp����


	fileHead.bfSize=  sizeof (BITMAPFILEHEADER) +  sizeof (BITMAPINFOHEADER)\
		+ colorTablesize + line_byte*height;               //bfSize��ͼ���ļ�4����ɲ���֮��
	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;    
	fileHead.bfOffBits = 54+colorTablesize;               //bfOffBits��ͼ���ļ�ǰ3����������ռ�֮��    


	fp.write(( char *)&fileHead,  sizeof (BITMAPFILEHEADER)); //д�ļ�ͷ���ļ�


	//����λͼ��Ϣͷ�ṹ��������д��Ϣͷ��Ϣ
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


	//дλͼ��Ϣͷ���ڴ�  
	fp.write(( char *)&head,  sizeof (BITMAPINFOHEADER));


	//��ɫ��д���ļ� 
	fp.write(( char *)pColorTable,  sizeof (RGBQUAD)*color_type_num);  


	//дλͼ���ݽ��ļ�pBmpBuf
	fp.write(( char *)p_data, height*line_byte);
	fp.close();


	delete  []pColorTable;
	delete  []p_data;
	return  0;
}

void alphaToWhite(Mat &srcAlpha)
{
	int alpha = 0;
	int nc = 3;
	for (int j = 0; j < srcAlpha.rows; j++)
	{
		for (int i = 0; i < srcAlpha.cols * 3; i += 3)
		{
			// Ŀ��ͼƬΪ��ͨ������������ͨ���ı�������ͨ����Դͼ
			// i/3*4��ʾ��i/3�����ص�λ�� 
			// i/3*4 + 3��ʾ�����ص�alphaͨ����ֵ
			alpha = srcAlpha.ptr<uchar>(j)[i / 3 * 4 + 3];
			//alpha = 255-alpha;
			if (alpha == 0) //4ͨ��ͼ���alpha�ж�
			{
				for (int k = 0; k < 3; k++)
				{
					// if (src1.ptr<uchar>(j)[i / nc*nc + k] != 0)
					if ((j  < srcAlpha.rows) && (j  >= 0) &&
						((i) / 3 * 3 + k < srcAlpha.cols * 3) && ((i) / 3 * 3 + k >= 0) &&
						(i / nc * 4 + k < srcAlpha.cols * 4) && (i / nc * 4 + k >= 0))
					{
						srcAlpha.ptr<uchar>(j)[(i) / nc*4 + k] = 255;
					}
				}

			}
		}
	}
}

//����szSrcPath:����ͼƬ��ַ
//����szDstPath:���ͼƬ��ַ
int Color2BW(const char * szSrcPath,const char * szDstPath)
{
	CString strSrcPath(szSrcPath);
	string strDstPath(szDstPath);

	if( !PathFileExists(strSrcPath) )
		return -1;

	Mat matSrc;
	matSrc = imread(szSrcPath, -1);

	if (matSrc.channels() == 4)
	{
		//͸������ת��
		alphaToWhite(matSrc);
	}

	//��ɫת��ɫ
	cvtColor(matSrc,matSrc,CV_BGR2GRAY); 

	//��ɫת��ֵ��
	threshold(matSrc, matSrc, 177, 255, CV_THRESH_BINARY);

	//λ���ת1
	int nRet = save_bmp_image( matSrc, strDstPath);
	if (nRet != 0)
	{
		return nRet;
	}
	
	return 0;
}