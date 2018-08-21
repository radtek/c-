// imgTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <afx.h>

#include <string>
#include <iostream>

using namespace std;



#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_wait.h"
#include "ace/Thread.h"
#include "ace/Time_Value.h"
#include "ace/Thread_Manager.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Handle_Set.h"

#pragma comment(lib, "ACE.lib")
#pragma comment(lib, "Common.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	/*
	ACE_INET_Addr port_to_listen (9999);

	ACE_SOCK_Acceptor acceptor;

	acceptor.open (port_to_listen, 1);

	while (1)
	{
		ACE_SOCK_Stream peer;

		ACE_INET_Addr peer_addr;

		ACE_Time_Value timeout (10, 0);

		if( 0 == acceptor.accept (peer, &peer_addr, &timeout, 0))
		{
			char buffer[4096];
			ssize_t bytes_received;

			while ((bytes_received = peer.recv (buffer, sizeof(buffer))) != -1)
			{
				peer.send_n (buffer, bytes_received);
			}
			peer.close ();
		}
	}
	*/

	ACE_INET_Addr port_to_listen(5999);        //绑定的端口
	ACE_SOCK_Acceptor acceptor;
	if (acceptor.open (port_to_listen,1) == -1)     //绑定端口
	{
		cout << "bind port fail" <<endl;
		return -1;
	}
	cout << "bind port = 5999" <<endl;
	while(true)
	{
		ACE_SOCK_Stream peer;        //和客户端的数据通路
		ACE_Time_Value timeout (60, 0);
		cout << endl << "accept waiting..." <<endl;
		int nRet = acceptor.accept (peer);

		if (nRet != -1)    //建立和客户端的连接
		{
			cout<<"client connect "<<endl;
			char buffer[1024];
			ssize_t bytes_received;

			ACE_INET_Addr raddr;
			peer.get_remote_addr(raddr);
			cout<<"ip = "<<raddr.get_host_addr()<<"\tport = "<<raddr.get_port_number()<<endl;

			bytes_received = peer.recv_n (buffer,23);
			//cout << "peer.recv (buffer,23) ret = " << bytes_received <<endl;
			if (0 >= bytes_received)
			{
				continue;
			}

			string strLen = buffer;
			int nJsonLen = atoi( strLen.substr(0,5).c_str() );
			cout << "json长度 = " << nJsonLen <<endl;
			if (0 >= nJsonLen)
			{
				continue;
			}

			char *szJson = new char[nJsonLen+1];
			szJson[nJsonLen] = '0';
			bytes_received = peer.recv_n (szJson,nJsonLen);
			//cout << "peer.recv (szJson,nJsonLen) ret = " << bytes_received <<endl;
			if (0 >= bytes_received)
			{
				delete []szJson;
				continue;
			}

			CString strMessage(szJson,nJsonLen);
			cout <<"json = " << strMessage <<endl;
			delete []szJson;

			int nImgLen = atoi( strLen.substr(5,10).c_str() );

			char *szImg = new char[nImgLen];
			bytes_received = peer.recv_n (szImg,nImgLen);
			cout << "图片数据长度 = " << nImgLen <<endl;
			cout << "图片保存为e:/recv.bmp" <<endl;
			if (0 >= bytes_received)
			{
				delete []szImg;
				continue;
			}

			CString writeImg = "e:/recv.bmp";
			CFile file2;
			file2.Open(writeImg,CFile::modeCreate|CFile::modeWrite);
			file2.Write(szImg,nImgLen);
			file2.Flush();
			file2.Close();
			
			

			string strSend = "000050000000000procbank{abc}";
			peer.send_n(strSend.c_str(), strSend.length());    //对客户端发数据
			cout<<"send data to client = " << strSend <<endl;

			delete []szImg;
		}
		peer.close ();
		cout<<"client disconnect "<<endl;

	}
	return 0;
}

