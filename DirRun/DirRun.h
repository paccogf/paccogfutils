#ifndef _DIRRUN_H_
#define _DIRRUN_H_ 

#include <windows.h>
#include <stdio.h> 

#define _DIRRUN_H_MAX_PATH (MAX_PATH * 5) 


typedef int  (* DirEntryFunc)(char *, void *); 


/////////////////////////////////////////////////////
// EL PATH ES DE LA FORMA C:\dir1\dir2\<wildcard> 
// PUEDE ACABAR CON UNA BARRA '\' 
// EL ULTIMO ELEMENTO DEL PATH DEBE SER UN FICHERO O UN WILDCARD 
//////////////////////////////////////////
int DirRun(const char *szPathAndWildCard, BOOL bSubDirs, DirEntryFunc function, void *ptrToStruct);


#endif



