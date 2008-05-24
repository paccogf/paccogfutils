#ifndef _XINF_H_
#define _XINF_H_
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>


#define EXEINFO_SUCCESS       0
#define EXEINFO_NOMZ          1
#define EXEINFO_NOPE          2
#define EXEINFO_NOSECTIONS    3
#define EXEINFO_ERROR         4
#define EXEINFO_NOIAT         5

#define EXEINFO_MAXSECTIONS   ??



typedef struct _maphandle 
{
   HANDLE hFile;
   HANDLE hFileMapping;   
} MAPHANDLE, *PMAPHANDLE;



typedef struct _ExeInfo
{
IMAGE_DOS_HEADER        *MZ;
IMAGE_NT_HEADERS        *PE;
IMAGE_SECTION_HEADER    *section;

DWORD size;
//IMAGE_IMPORT_DESCRIPTOR *IAT;

BOOL                    isDll;

//INT                     nIAT;
} EXEINFO, *PEXEINFO;



// FUNCION PPAL
INT GetExeInfo(IN BYTE *lpFileBuffer, IN DWORD nFileBuffer, OUT EXEINFO *lpXInfo);


// FUNCIONES QUE NECESITAN LA ESTRUCTURA EXEINFO
//INT   GetIAT      (IN HANDLE hFile, IN OUT PEXEINFO pInfo);
SHORT    _RVAToSection  (IN PEXEINFO pInf, IN DWORD rva, OUT DWORD *section_offset);
DWORD    RVAToRaw       (IN PEXEINFO pInf, IN DWORD rva);
SHORT    _RawToSection  (IN PEXEINFO pInf, IN DWORD rawpos, IN DWORD *section_offset);
DWORD    RawToRVA       (IN PEXEINFO pInf, IN DWORD raw);

#define RawToSection(pInf, rawpos)  (_RawToSection(pInf, rawpos, NULL))
#define RVAToSection(pInf, rva)     (_RVAToSection(pInf, rva, NULL))
#define GetFilePointer(hFile)       (SetFilePointer(hFile, 0, 0, FILE_CURRENT))

// FUNCIONES DE MAPEO DE FICHEROS
PMAPHANDLE  MapFile     (LPCTSTR IN filename, BYTE OUT **filebuffer, DWORD OUT *nBytes);
DWORD       UnMapFile   (PMAPHANDLE pmhFile);

#endif

