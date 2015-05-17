#include "cmd.h"
#include "baratol.h"
using namespace baratol;

int SetIp(const char* network, const char* ip, const char* netmast, const char* gateway)
{
	CString strSetIP;
	strSetIP.Format("netsh interface ip set address \"%s\" static %s %s %s", network, ip, netmast, gateway);
	printf("%s\n", strSetIP.GetBuffer(0));
	int ret = system( strSetIP.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}
	return 0;
}

int SetPrimaryDns(const char* network, const char* dns)
{
	CString strSetDns;
	strSetDns.Format("netsh interface ip set dns \"%s\" static %s primary", network, dns);

	printf("%s\n", strSetDns.GetBuffer(0));
	int ret = system( strSetDns.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}
	return 0;
}

int AddDns(const char* network, const char* dns)
{
	CString strSetDns;

	strSetDns.Format("netsh interface ip add dns \"%s\" %s", network, dns);

	printf("%s\n", strSetDns.GetBuffer(0));
	int ret = system( strSetDns.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}
	return 0;
}


int SetHostname(const char* hostname)
{
	//设置主机名
	//@echo off 
	//set /p name=请输您的计算机名：
	//reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Control\ComputerName\ActiveComputerName" /v ComputerName /t reg_sz /d %name% /f
	//reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters" /v "NV Hostname" /t reg_sz /d %name% /f 
	//reg add "HKEY_LOCAL_MACHINE\System\CurrentControlSet\Services\Tcpip\Parameters" /v Hostname /t reg_sz /d %name% /f

	CString strSetHostname;
	strSetHostname.Format("reg add \"HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Control\\ComputerName\\ActiveComputerName\" /v ComputerName /t reg_sz /d %s /f", hostname);
	printf("%s\n", strSetHostname.GetBuffer(0));
	int ret = system( strSetHostname.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}
	strSetHostname.Format("reg add \"HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\Tcpip\\Parameters\" /v \"NV Hostname\" /t reg_sz /d %s /f", hostname);
	printf("%s\n", strSetHostname.GetBuffer(0));
	ret = system( strSetHostname.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}

	strSetHostname.Format("reg add \"HKEY_LOCAL_MACHINE\\System\\CurrentControlSet\\Services\\Tcpip\\Parameters\" /v Hostname /t reg_sz /d %s /f", hostname);
	printf("%s\n", strSetHostname.GetBuffer(0));
	ret = system( strSetHostname.GetBuffer(0));
	if (ret != 0)
	{
		return ret;
	}
	return 0;
}