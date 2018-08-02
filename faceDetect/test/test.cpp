// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	cout << argc <<endl;
	for (int i = 0; i < argc; i++)
	{
		 printf("Argument %d is %s.\n", i, argv[i]);
	}
	/*
	int *n = new int[3];
	n[0] = 0;
	n[1] = 3;
	n[2] = 5;
	*/
	/*
	for (int i = 0; i < 3; i++)
	{
		cout << *n + i <<endl;
	}
	*/
	/*
	for (int i = 0; i < 3; i++)
	{
		int *a = ((int*)(n+1)) + 1 * i;
		cout << *a << endl;
		cout << a[i] << endl;
	}
	*/
	//system("pause");
	return 0;
}

