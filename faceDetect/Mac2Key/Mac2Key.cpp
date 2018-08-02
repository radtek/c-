// Mac2Key.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <direct.h>
#pragma comment(lib, "IPHLPAPI.lib")

using namespace std;


vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型
	char * strs = new char[str.length() + 1] ; //不要忘了
	strcpy(strs, str.c_str()); 

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; //分割得到的字符串转换为string类型
		res.push_back(s); //存入结果数组
		p = strtok(NULL, d);
	}

	return res;
}

bool GetMacByGetAdaptersAddresses(std::string& macOUT)
{
	bool ret = false;

	ULONG outBufLen = sizeof(IP_ADAPTER_ADDRESSES);
	PIP_ADAPTER_ADDRESSES pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
	if (pAddresses == NULL) 
		return false;
	// Make an initial call to GetAdaptersAddresses to get the necessary size into the ulOutBufLen variable
	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == ERROR_BUFFER_OVERFLOW)
	{
		free(pAddresses);
		pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
		if (pAddresses == NULL) 
			return false;
	}

	if(GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pAddresses, &outBufLen) == NO_ERROR)
	{
		// If successful, output some information from the data we received
		for(PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses; pCurrAddresses != NULL; pCurrAddresses = pCurrAddresses->Next)
		{
			// 确保MAC地址的长度为 00-00-00-00-00-00
			if(pCurrAddresses->PhysicalAddressLength != 6)
				continue;
			char acMAC[32];
			sprintf(acMAC, "%02X-%02X-%02X-%02X-%02X-%02X",
				int (pCurrAddresses->PhysicalAddress[0]),
				int (pCurrAddresses->PhysicalAddress[1]),
				int (pCurrAddresses->PhysicalAddress[2]),
				int (pCurrAddresses->PhysicalAddress[3]),
				int (pCurrAddresses->PhysicalAddress[4]),
				int (pCurrAddresses->PhysicalAddress[5]));
			macOUT = acMAC;
			ret = true;
			break;
		}
	} 

	free(pAddresses);

	vector<string> vecMac = split(macOUT,"-");
	macOUT = "";
	for (int i = 0; i < vecMac.size(); i++)
	{
		macOUT += vecMac[i];
	}

	return ret;
}



/*
0123456789abcdef  mac
ihkjm1ona`8;:=<?  request
:;89>?<=23khino1  key
*/


string Mac2Key(string strMac)
{
	string mac = "0123456789abcdef";
	string key = ":;89>?<=23khino1";
	transform(strMac.begin(),strMac.end(),strMac.begin(),::tolower);

	string strKey = "";
	for (int i = 0; i < strMac.length(); i++)
	{
		int pos = mac.find( strMac[i] );
		if (pos < 0)
		{
			return "-1";
		}
		strKey += key[pos];
	}
	
	return strKey;
}

void writeTxt(string path,string txt)
{
	FILE *fp;
	fp = fopen(path.c_str(),"w");
	for (int i = 0; i < txt.length(); i++)
	{
		fprintf(fp,"%c",txt[i]);
	}
	fflush(fp);
	fclose(fp);
}

string getCurrentPath()
{
	char szFullPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szFullPath,MAX_PATH);
	string strPath(szFullPath);
	int pos = strPath.find_last_of("\\");
	strPath = strPath.substr(0,pos);
	return strPath;
}


int _tmain(int argc, _TCHAR* argv[])
{
	string strMac = "";
	if (!GetMacByGetAdaptersAddresses(strMac))
	{
		printf("MAC获取失败\n");
		system("pause");
		return -1;
	}
	string strKey = Mac2Key(strMac);

	string strPath = getCurrentPath() + "\\CIEIfaceliving.key";

	writeTxt(strPath,strKey);

	return 0;
}

