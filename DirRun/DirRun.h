#ifndef _DIRRUN_H_
#define _DIRRUN_H_ 

#include <windows.h>
#include <stdio.h> 


//
// PROTOTIPO FUNCION CALLBACK - EL PRIMER PARAMETRO ES EL NOMBRE DEL FICHERO, EL SEGUNDO, PUNTERO A LO QUE QUERAMOS
//
typedef int  (* DirEntryFunc)(char *, void *);

//
// CODIGOS DE ERROR
//
typedef enum {	

	drOK=0,
	drNoPath,
	drNoWildcard,
	drBadFormat,
	drInvalidPath,
	drPathIsDirectory,
	drUnknownError,
	drLastError 

} drError;

/////////////////////////////////////////////////////
// EL PATH ES DE LA FORMA C:\dir1\dir2\<wildcard> 
// PUEDE ACABAR CON UNA BARRA  
// EL ULTIMO ELEMENTO DEL PATH DEBE SER UN FICHERO O UN WILDCARD 
// ptrToStruct PUEDE SER NULL
//////////////////////////////////////////
int DirRun(const char *szPathAndWildCard, BOOL bSubDirs, DirEntryFunc function, void *ptrToStruct);


#endif