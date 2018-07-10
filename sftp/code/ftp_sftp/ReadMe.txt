========================================================================
    控制台应用程序：ftp_sftp 项目概述
========================================================================

应用程序向导已为您创建了此 ftp_sftp 应用程序。

本文件概要介绍组成 ftp_sftp 应用程序的每个文件的内容。


ftp_sftp.vcxproj
    这是使用应用程序向导生成的 VC++ 项目的主项目文件，
    其中包含生成该文件的 Visual C++ 
    的版本信息，以及有关使用应用程序向导选择的平台、配置和项目功能的信息。

ftp_sftp.vcxproj.filters
    这是使用“应用程序向导”生成的 VC++ 项目筛选器文件。 
    它包含有关项目文件与筛选器之间的关联信息。 在 IDE 
    中，通过这种关联，在特定节点下以分组形式显示具有相似扩展名的文件。
    例如，“.cpp”文件与“源文件”筛选器关联。

ftp_sftp.cpp
    这是主应用程序源文件。

/////////////////////////////////////////////////////////////////////////////
应用程序向导创建了下列资源：

ftp_sftp.rc
    这是程序使用的所有 Microsoft Windows 资源的列表。  它包括 RES 
    子目录中存储的图标、位图和光标。  此文件可以直接在 Microsoft Visual C++ 
    中进行编辑。

Resource.h
    这是标准头文件，可用于定义新的资源 ID。
    Microsoft Visual C++ 将读取并更新此文件。

/////////////////////////////////////////////////////////////////////////////
其他标准文件：

StdAfx.h，StdAfx.cpp
    这些文件用于生成名为 ftp_sftp.pch 的预编译头 (PCH) 文件和
    名为 StdAfx.obj 的预编译类型文件。

/////////////////////////////////////////////////////////////////////////////
其他注释：

应用程序向导使用“TODO:”注释来指示应添加或自定义的源代码部分。

/////////////////////////////////////////////////////////////////////////////

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: 更改错误代码以符合您的需要
			_tprintf(_T("错误: MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			/*
			//const char csOpen[] = "{\"ip\":\"172.20.10.3\",\"port\":\"21\",\"username\":\"dell\",\"password\":\"wsh\",\"mode\":\"0\"}";
			const char csOpen[] = "{\"ip\":\"127.0.0.1\",\"port\":\"2023\",\"username\":\"sftp\",\"password\":\"sftp\",\"mode\":\"1\"}";
		    const char csCd[] = "{\"dir\":\"/test\"}";
			const char csDownloadFile[] = "{\"srcfilename\":\"/gwi/txt/g.txt\",\"destfilename\":\"d:\\\\ftp_test\\\\sftp\\\\test1.txt\"}";
		    const char csDownloadDir[] = "{\"srcdir\":\"/gwi/dir/\",\"destdir\":\"D:\\\\ftp_test\\\\sftpdir\"}";
			const char csUpdateFile[] = "{\"srcfilename\":\"d:\\\\ftp_test\\\\gwi.txt\",\"destfilename\":\"/gwi/txt/g.txt\"}";
			const char csUpdateDir[] = "{\"srcdir\":\"d:\\\\ftp_test\",\"destdir\":\"/gwi/dir/\"}";
			const char* pcRet = NULL;
			pcRet = ftpopen(csOpen);
			printf("ftpopen=>%s\n",pcRet);
			//getchar();
			pcRet = cd(csCd);
			printf("cd=>%s\n",pcRet);
			//getchar();

			pcRet = uploadfile(csUpdateFile);
			printf("uploadfile=>%s\n",pcRet);
			//getchar();


			pcRet = uploaddir(csUpdateDir);
			printf("uploaddir=>%s\n",pcRet);
			//getchar();
			
			pcRet = downloadfile(csDownloadFile);
			printf("downloadfile=>%s\n",pcRet);
			//getchar();
			
			
			pcRet = downloaddir(csDownloadDir);
			printf("downloaddir=>%s\n",pcRet);
			getchar();
			
			pcRet = ftpclose(NULL);
			printf("ftpclose=>%s\n",pcRet);
			getchar();
			*/
			//GWI_SFTPClient::sftpOpen("127.0.0.1",2023,"sftp","sftp");
			//GWI_SFTPClient::uploadFile("/test","D:\\ftp_test\\gwi.txt",NULL);
			//GWI_SFTPClient::downloadFile("/test","d:\\ftp_test\\test");
			//CString csFile;
			//GWI_SFTPClient::getFileList("/test","",csFile);
			//int iret = GWI_SFTPClient::sftpDwnloadFileDir("/test","d:\\ftp_test\\gwi");
			//int iret = GWI_SFTPClient::sftpUpdateFileDir("/test","d:\\ftp_test\\gwi");
			//char path[MAX_PATH] = "/test/gwi/123HAO号";
			//int iret = GWI_SFTPClient::sftpCd(path);
			//	char errmsg[MAX_PATH] = {};
			//GWI_SFTPClient::getGWI_SFTPClient_ErrorMsg_By_Errorcode(iret,errmsg);
			//printf("%d==>%s",iret,errmsg);
		//	getchar();
			//GWI_SFTPClient::sftpClose();
			// TODO: 在此处为应用程序的行为编写代码。
			const char csOpen[] = "{\"ip\":\"127.0.0.1\",\"port\":\"21\",\"username\":\"dell\",\"password\":\"wsh\",\"mode\":\"0\"}";
		    const char csCd[] = "{\"dir\":\"/test\"}";
			const char csDownloadFile[] = "{\"srcfilename\":\"/test/gwi/test1/test2/test.txt\",\"destfilename\":\"d:\\\\ftp_test\\\\ftp_file\\\\test1.txt\"}";
		    const char csDownloadDir[] = "{\"srcdir\":\"/test\",\"destdir\":\"D:\\\\ftp_test\\\\ftpdir\"}";
			const char csUpdateFile[] = "{\"srcfilename\":\"d:\\\\ftp_test\\\\update\\\\gwi.txt\",\"destfilename\":\"/gwi/txt/file/g.txt\"}";
			const char csUpdateDir[] = "{\"srcdir\":\"d:\\\\ftp_test\\\\update\\\\dir\",\"destdir\":\"/gwi/txt\"}";
			const char* pcRet = NULL;
			pcRet = ftpopen(csOpen);
			printf("%s\n",pcRet);
		/*
			//getchar();
			pcRet = cd(csCd);
			printf("%s\n",pcRet);
			//getchar();
			*/
			pcRet = uploadfile(csUpdateFile);
			printf("%s\n",pcRet);
			getchar();

			pcRet = uploaddir(csUpdateDir);
			printf("%s\n",pcRet);
			//getchar();
			
			//pcRet = downloadfile(csDownloadFile);
			//printf("%s\n",pcRet);
			//getchar();
			
			
			//pcRet = downloaddir(csDownloadDir);
			//printf("%s\n",pcRet);
			//getchar();
			

			pcRet = ftpclose(NULL);
			printf("%s\n",pcRet);
			getchar();
		
		}
	}
	else
	{
		// TODO: 更改错误代码以符合您的需要
		_tprintf(_T("错误: GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}