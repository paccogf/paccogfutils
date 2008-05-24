#include "resload.h"



//----------------------------------------------------------------
// build a .sys file on disk from a resource
//----------------------------------------------------------------

LPBYTE GetResource(LPTSTR theResourceName, LPDWORD aFileSize, LPCTSTR typeName)
{

HRSRC aResourceH;
HANDLE aResourceHGlobal;
LPBYTE aFilePtr;


//////////////////////////////////////////////////////////
// locate a named resource in the current binary EXE
//////////////////////////////////////////////////////////

aResourceH = FindResource(NULL, theResourceName, typeName);
if(!aResourceH) return NULL;

aResourceHGlobal = LoadResource(NULL, aResourceH);
if(!aResourceHGlobal) return NULL;


//Using the SizeOfResource call, the length of the embedded file is obtained:
*aFileSize = SizeofResource(NULL, aResourceH);
aFilePtr = (LPBYTE) LockResource(aResourceHGlobal);
if(!aFilePtr) return NULL;

return aFilePtr;
}
