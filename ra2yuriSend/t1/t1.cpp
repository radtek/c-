// t1.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

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

int _tmain(int argc, _TCHAR* argv[])
{
	int port = 6000;
	ACE_INET_Addr port_to_listen(port);        //�󶨵Ķ˿�
	ACE_SOCK_Acceptor acceptor;
	if (acceptor.open (port_to_listen,1) == -1)     //�󶨶˿�
	{
		cout << "bind port fail" <<endl;
		system("pause");
		return -1;
	}
	cout << "bind port = " << port <<endl;
	while(true)
	{
		ACE_SOCK_Stream peer;        //�Ϳͻ��˵�����ͨ·
		cout << endl << "accept waiting..." <<endl;
		int nRet = acceptor.accept (peer);

		if (nRet != -1)    //�����Ϳͻ��˵�����
		{
			cout<<"client connect "<<endl;
		}
	}
	return 0;
}

