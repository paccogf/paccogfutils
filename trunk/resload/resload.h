#ifndef _RESLOAD_H_
#define _RESLOAD_H_

#include <windows.h>

#define GetBinRes(theResourceName, aFileSize) GetResource(theResourceName, aFileSize, typeName)

LPBYTE GetResource(LPTSTR theResourceName, LPDWORD aFileSize, LPCTSTR typeName);




#endif
