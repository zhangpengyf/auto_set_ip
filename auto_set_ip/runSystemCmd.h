#ifndef _run_system_cmd_h
#define _run_system_cmd_h
#include "baratol.h"
#include <map>
typedef std::map<baratol::CString, baratol::CString> MAPMAC2NAME;

bool runCmd(baratol::CString cmd, baratol::CString& result);

bool PraseString(baratol::CString str, MAPMAC2NAME& result);

#endif
