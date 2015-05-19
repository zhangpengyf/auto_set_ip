#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "runSystemCmd.h"
using namespace baratol;

bool runCmd(CString cmd, CString& result)
{
	bool ret = false;

	//初始化返回MAC地址缓冲区
	SECURITY_ATTRIBUTES sa; 
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL; 
	sa.bInheritHandle = TRUE; 
	//创建管道
	HANDLE hReadPipe,hWritePipe;
	if(CreatePipe(&hReadPipe, &hWritePipe, &sa, 0) == TRUE)
	{
		//控制命令行窗口信息
		STARTUPINFO si; 
		//返回进程信息
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO); 
		GetStartupInfo(&si); 
		si.hStdError = hWritePipe; 
		si.hStdOutput = hWritePipe; 
		si.wShowWindow = SW_HIDE; //隐藏命令行窗口
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//创建获取命令行进程
		if (CreateProcess(NULL, cmd.GetBuffer(0), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == TRUE) 
		{ 
			WaitForSingleObject(pi.hProcess, 3000); // 设置超时时间，防止Vista、Win7等操作系统卡死
			unsigned long count;
			std::string strBuffer(1024 * 10, '\0'); // 准备足够大的缓冲区
			if(ReadFile(hReadPipe, const_cast<char*>(strBuffer.data()), strBuffer.size() - 1, &count, 0) == TRUE)
			{
				strBuffer.resize(strBuffer.find_first_of('\0')); // 截掉缓冲区后面多余的'\0'
				result = strBuffer.c_str();
				ret = true;
			}
			CloseHandle(pi.hThread); 
			CloseHandle(pi.hProcess); 
		}
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
	}
	return ret;
}

void Trim(CString& str)
{
	str.TrimLeft("\r\n :\t\"\'");
	str.TrimRight("\r\n :\t\"\'");
}

// 874	泰语	932	日语
// 936	中文（简体）	949	朝鲜语
// 950	中文（台湾和香港繁体）	1200	Unicode
// 1250	东欧语言	1251	西里尔语
// 1252	美国和西欧语言	1253	希腊语
// 1254	土耳其语	1255	希伯来语
// 1256	阿拉伯语	1257	波罗的语
bool PraseString(baratol::CString str, MAPMAC2NAME& result)
{
   int ret = GetACP();
   CString strEthernet;
   CString strPhysicalAddr;
   if (ret = 936)//中文
   {
		strEthernet = "以太网适配器";
		strPhysicalAddr = "物理地址";
   }
   else if (ret = 1252)//英文
   {

   }
   else
   {
	   TL_LogEventV("system language not support");
	   return false;
   }

   int offset = 0;
   int indexEnd = 0;
   while (offset < str.GetLength())
   {
	   offset = str.Find(strEthernet.GetBuffer(0), offset);
	   if (offset < 0)
	   {
		   break;
	   }
	   offset += strEthernet.GetLength();
	   indexEnd = str.find_first_of('\n', offset);
	   if (indexEnd < 0)
	   {
		   break;
	   }

	   //取到以太网 网络名
	   CString strName;
	   strName = str.Mid(offset, indexEnd - offset);
	   Trim(strName);
	   if (strName.IsEmpty())
	   {
		   break;
	   }
	   offset += strName.GetLength();

	   offset = str.Find(strPhysicalAddr.GetBuffer(0), offset);
	   if (offset < 0)
	   {
		   break;
	   }
	   offset += strPhysicalAddr.GetLength();
	   int indexMaoHao = str.find_first_of(':', offset);
	   if (indexMaoHao < 0)
	   {
		   break;
	   }
	   indexEnd = str.find_first_of('\n', indexMaoHao);
	   if (indexEnd < 0)
	   {
		   break;
	   }
	   CString strMac = str.Mid(indexMaoHao, indexEnd - indexMaoHao);
	   offset += indexEnd;
	   Trim(strMac);
	   if (strMac.IsEmpty())
	   {
		   break;
	   }

	   strMac.Replace('-', ':');
	   result[strMac] = strName;
	   
   }
   return true;
}