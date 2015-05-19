#ifndef _getmac_h_
#define _getmac_h_

typedef struct _ASTAT_
{
	ADAPTER_STATUS adapt;
	NAME_BUFFER NameBuff [30];
} ASTAT, *PASTAT;

void GetMacNetbios(char * mac);

bool GetMacByGetAdaptersInfo(baratol::CStringArray& macOUT);


#endif