#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "baratol.h"
#include <map>
#include <string>
#include "getmac.h"
#include "cmd.h"
using namespace baratol;
using namespace std;

#define NETWORK_NAME "本地连接2"

#pragma comment(lib,"netapi32.lib")
// mac,hostname,ip,netmask,gateway,dns1,dns2
// 00:0C:29:CA:6C:14,myserver01,192.168.0.11,255.255.255.0,192.168.0.254,192.168.0.250,192.168.0.251
// 00:0C:29:CA:6C:20,myserver02,192.168.0.12,255.255.255.0,192.168.0.254,192.168.0.250,192.168.0.251

typedef struct line
{
	CString mac;
	CString hostname;
	CString ip;
	CString netmast;
	CString gateway;
	CString dns1;
	CString dns2;
}LINE;

typedef map<CString, LINE*> DATAMAP;

int ReadFilesToArray(const char* fileName, DATAMAP& data)
{
	CTLFileObj file;
	if (file.Open((LPCTSTR)fileName,PF_MEM_FILE | PF_OPEN_ONLY) != TL_FILE_OK)
	{
		printf("打开文件失败, %s", (LPCTSTR)fileName);
		return -1;
	}
	int nFileLen = file.GetLength();

	char* buff = (char*)TL_Allocate(nFileLen + 1);
	memset(buff, 0, nFileLen + 1);
	if (file.Read(buff, nFileLen) != nFileLen)
	{
		file.Close();
		TL_Deallocate(buff);
		printf("读取CSV文件失败, %s", (LPCTSTR)fileName);
		return -1;
	}
	file.Close();
	CStringArray ayOneFile;
	TL_SpliteString((LPCTSTR)buff, '\n', IntoContainer<baratol::CStringArray>(ayOneFile));

	int nLines = ayOneFile.GetSize();
	for (int i = 0; i < nLines; i++)
	{
		CStringArray ayOneLine;
		TL_SpliteString((LPCTSTR)ayOneFile[i], ',', IntoContainer<baratol::CStringArray>(ayOneLine));
		if (ayOneLine[0] == "mac")
		{
			continue;
		}
		{
			int index = 0;
			LINE* pLine = new LINE;
			pLine->mac = ayOneLine[index++];
			pLine->mac.TrimLeft();
			pLine->mac.TrimRight();

			pLine->hostname = ayOneLine[index++];
			pLine->hostname.TrimLeft();
			pLine->hostname.TrimRight();

			pLine->ip = ayOneLine[index++];
			pLine->ip.TrimLeft();
			pLine->ip.TrimRight();

			pLine->netmast = ayOneLine[index++];
			pLine->netmast.TrimLeft();
			pLine->netmast.TrimRight();

			pLine->gateway = ayOneLine[index++];
			pLine->gateway.TrimLeft();
			pLine->gateway.TrimRight();

			pLine->dns1 = ayOneLine[index++];
			pLine->dns1.TrimLeft();
			pLine->dns1.TrimRight();

			pLine->dns2 = ayOneLine[index++];
			pLine->dns2.TrimLeft();
			pLine->dns2.TrimRight();

			CString key = ayOneLine[0];
			key.TrimLeft();
			key.TrimRight();

			data[key] = pLine;
		}

	}

	TL_Deallocate(buff);

	return 0;
}


int main(int argc, char **argv)
{
	if(argc <= 1)
	{
		cout<<"请使用以下方式中的一种传递参数:"<<endl;
		cout<<"auto_set_ip filename [network]"<<endl;
		cout<<"例如:"<<endl;
		cout<<"auto_set_ip ./auto_set_ip.txt"<<endl;
		cout<<"auto_set_ip ./auto_set_ip.txt 本地连接"<<endl;
		return -1;
	}

	CString strNetwork = NETWORK_NAME;
	if (argc >= 3)
	{
		strNetwork = argv[2];
	}

	const char* fileName = argv[1];
	DATAMAP data;

	//取到了了数据
	if (ReadFilesToArray(fileName, data) != 0)
	{
		return -1;
	}

	//获取MAC
	char *mac = new char[32];
	GetMac(mac);
	printf("mac is:%s\n", mac);
	CString strMac = mac;
	delete[]mac;

	strMac.TrimRight();
	strMac.TrimLeft();

	DATAMAP::iterator iter = data.find(strMac);
	if (iter == data.end())
	{
		printf("can not found ip info in file:%s\n", argv[1]);
		return -1;
	}

	LINE* pRecord = iter->second;

	int ret = SetIp(strNetwork, pRecord->ip, pRecord->netmast, pRecord->gateway);
	if (ret != 0)
	{
		printf("set ip error:%d\n", ret);
		return ret;
	}

	ret = SetPrimaryDns(strNetwork, pRecord->dns1);
	if (ret != 0)
	{
		printf("set dns1 error:%d\n", ret);
		return ret;
	}

	ret = AddDns(strNetwork, pRecord->dns2);
	if (ret != 0)
	{
		printf("set dns2 error:%d\n", ret);
		return ret;
	}

	ret = SetHostname(pRecord->hostname);
	if (ret != 0)
	{
		printf("set hostname error:%d\n", ret);
		return ret;
	}
	
	return 0;
}
