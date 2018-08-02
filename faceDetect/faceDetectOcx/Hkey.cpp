#include "stdafx.h"
#include <Windows.h>
#include "Hkey.h"

//����ֲ�汾 wstring => string
std::string ws2s(const std::wstring& ws)
{
	std::string curLocale = setlocale(LC_ALL, "");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = wcstombs(NULL, _Source, 0) + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest,0,_Dsize);
	wcstombs(_Dest,_Source,_Dsize);
	std::string result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

//����ֲ�汾 string => wstring
std::wstring s2ws(const std::string& s)
{
	std::string curLocale = setlocale(LC_ALL, ""); 
	const char* _Source = s.c_str();
	size_t _Dsize = mbstowcs(NULL, _Source, 0) + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest,_Source,_Dsize);
	std::wstring result = _Dest;
	delete []_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::string GetRegValue(HKEY hKey, const std::string& strUrl, const std::string& strKey)
{
	std::string strValue("");
	HKEY  hKeyResult = NULL;
	DWORD dwSize     = 0;
	DWORD dwDataType = 0;
	std::wstring wstrUrl = s2ws(strUrl);
	std::wstring wstrKey = s2ws(strKey);

	
	//��ע���
	if(ERROR_SUCCESS == ::RegOpenKeyEx(hKey, wstrUrl.c_str(), 0, KEY_QUERY_VALUE, &hKeyResult))
	{
		// ��ȡ����ĳ���dwSize������dwDataType
		::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, NULL, &dwSize); 
		switch (dwDataType)
		{
		case REG_MULTI_SZ:
			{
				//�����ڴ��С
				BYTE* lpValue = new BYTE[dwSize];
				//��ȡע�����ָ���ļ�����Ӧ��ֵ
				LONG lRet = ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, lpValue, &dwSize);
				delete[] lpValue;
				break;
			}
		case REG_SZ:
			{
				//�����ڴ��С
				wchar_t* lpValue = new wchar_t[dwSize];
				memset(lpValue, 0, dwSize * sizeof(wchar_t));
				//��ȡע�����ָ���ļ�����Ӧ��ֵ
				if (ERROR_SUCCESS == ::RegQueryValueEx(hKeyResult, wstrKey.c_str(), 0, &dwDataType, (LPBYTE)lpValue, &dwSize))
				{
					std::wstring wstrValue(lpValue);
					strValue = ws2s(wstrValue);
				}
				delete[] lpValue;
				break;
			}
		default:
			break;
		}
	}

	//�ر�ע���
	::RegCloseKey(hKeyResult);


	return strValue;
}