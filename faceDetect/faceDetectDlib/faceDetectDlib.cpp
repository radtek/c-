// faceDetectDlib.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>

using namespace dlib;
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
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

	return 0;
}

