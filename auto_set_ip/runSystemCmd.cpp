#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "runSystemCmd.h"
using namespace baratol;

bool runCmd(CString cmd, CString& result)
{
	bool ret = false;

	//��ʼ������MAC��ַ������
	SECURITY_ATTRIBUTES sa; 
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL; 
	sa.bInheritHandle = TRUE; 
	//�����ܵ�
	HANDLE hReadPipe,hWritePipe;
	if(CreatePipe(&hReadPipe, &hWritePipe, &sa, 0) == TRUE)
	{
		//���������д�����Ϣ
		STARTUPINFO si; 
		//���ؽ�����Ϣ
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO); 
		GetStartupInfo(&si); 
		si.hStdError = hWritePipe; 
		si.hStdOutput = hWritePipe; 
		si.wShowWindow = SW_HIDE; //���������д���
		si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

		//������ȡ�����н���
		if (CreateProcess(NULL, cmd.GetBuffer(0), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == TRUE) 
		{ 
			WaitForSingleObject(pi.hProcess, 3000); // ���ó�ʱʱ�䣬��ֹVista��Win7�Ȳ���ϵͳ����
			unsigned long count;
			std::string strBuffer(1024 * 10, '\0'); // ׼���㹻��Ļ�����
			if(ReadFile(hReadPipe, const_cast<char*>(strBuffer.data()), strBuffer.size() - 1, &count, 0) == TRUE)
			{
				strBuffer.resize(strBuffer.find_first_of('\0')); // �ص���������������'\0'
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

// 874	̩��	932	����
// 936	���ģ����壩	949	������
// 950	���ģ�̨�����۷��壩	1200	Unicode
// 1250	��ŷ����	1251	�������
// 1252	��������ŷ����	1253	ϣ����
// 1254	��������	1255	ϣ������
// 1256	��������	1257	���޵���
bool PraseString(baratol::CString str, MAPMAC2NAME& result)
{
   int ret = GetACP();
   CString strEthernet;
   CString strPhysicalAddr;
   if (ret = 936)//����
   {
		strEthernet = "��̫��������";
		strPhysicalAddr = "�����ַ";
   }
   else if (ret = 1252)//Ӣ��
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

	   //ȡ����̫�� ������
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