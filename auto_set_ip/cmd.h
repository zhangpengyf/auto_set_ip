#ifndef _cmd_h_
#define _cmd_h_
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int SetIp(const char* network, const char* ip, const char* netmast, const char* gateway);

int SetPrimaryDns(const char* network, const char* dns);

int AddDns(const char* network, const char* dns);

int SetHostname(const char* hostname);

#endif