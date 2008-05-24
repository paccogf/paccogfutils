#include <windows.h>
#include <stdio.h>


// CONVIERTE EL VALOR DEVUELTO POR la API GetLastError() A SU MENSAJE DE TEXTO
#define ErrorToString(errcode, buffer, buffsize) \
               FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,\
               NULL,\
               errcode,\
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
               (LPTSTR) buffer,\
               buffsize, \
               NULL );


#define EXEINFO_SUCCESS       0
#define EXEINFO_NOMZ          1
#define EXEINFO_NOPE          2
#define EXEINFO_NOSECTIONS    3
#define EXEINFO_ERROR         4
#define EXEINFO_NOIAT         5

#define EXEINFO_MAXSECTIONS   16

//#define _DEBUG
#ifdef _DEBUG
    #define DEBUG           printf
    #define DEBUGEXP(exp)   printf("%s = 0x%08X\n", #exp, exp)
#else
    #define DEBUG           voidfunc
    #define DEBUGEXP        voidfunc
    void voidfunc()
    {
    }
#endif





/*
typedef struct _IAT
{
IMAGE_IMPORT_DESCRIPTOR *descriptor;
INT nIAT;





} IAT, PIAT;
*/
/*
// RETORNA GetLastError()
DWORD MapFile(LPCTSTR IN filename, BYTE OUT **filebuffer, DWORD OUT *nBytes)
{
HANDLE hFile, hFileMapping;

 
// ABRIR EL FICHERO
hFile=CreateFile(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
if(hFile == INVALID_HANDLE_VALUE) return GetLastError();

    // CREAR LA VISTA DE FICHERO
hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
if(hFileMapping == NULL) 
{
    CloseHandle(hFile);
    return GetLastError();
}

// MAPEAR EL FICHERO
*filebuffer=MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, 0);
if(*filebuffer == NULL)
{
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
    return GetLastError();
}

*nBytes=GetFileSize(hFile, NULL);


    
return ERROR_SUCCESS;    

}


*/


typedef struct _ExeInfo
{
IMAGE_DOS_HEADER        MZH;
IMAGE_NT_HEADERS        PEH;
IMAGE_SECTION_HEADER    section[EXEINFO_MAXSECTIONS];
IMAGE_IMPORT_DESCRIPTOR *IAT;


//IMAGE_
BOOL                    isDll;
INT                    nSections;
DWORD                   EntryPoint;
DWORD                   ImageBase;
INT                     nIAT;
} EXEINFO, *PEXEINFO;


// FUNCIONES QUE NO NECESITAN LA ESTRUCTRA EXEINFO
INT GetMZHeader      (IN HANDLE hFile, OUT PIMAGE_DOS_HEADER pMZH);
INT GetPEHeader      (IN HANDLE hFile, IN DWORD offset, OUT PIMAGE_NT_HEADERS pPEH);
INT GetSectionTable  (IN HANDLE hFile, IN DWORD offset,
                      OUT IMAGE_SECTION_HEADER section[EXEINFO_MAXSECTIONS],
                      IN INT nItems);

// FUNCION PPAL
INT GetExeInfo       (IN HANDLE hFile, OUT PEXEINFO pInfo);


// FUNCIONES QUE NECESITAN LA ESTRUCTURA EXEINFO
INT   GetIAT      (IN HANDLE hFile, IN OUT PEXEINFO pInfo);
INT   RVAToSection(IN PEXEINFO pInf, IN DWORD rva, OUT DWORD *section_offset);
DWORD RVAToRaw    (IN PEXEINFO pInf, IN DWORD rva);
INT   RawToSection(IN PEXEINFO pInf, IN DWORD rawpos, IN DWORD *section_offset);
DWORD RawToRVA    (IN PEXEINFO pInf, IN DWORD raw);



INT RVAToSection (IN PEXEINFO pInf, IN DWORD rva, OUT DWORD *section_offset)
{
INT i;
INT isect=-1;

for(i=0; i < pInf->nSections; i++)
   if((rva >= pInf->section[i].VirtualAddress) && (rva < (pInf->section[i].VirtualAddress+pInf->section[i].Misc.VirtualSize)))
      isect=i;
if(section_offset!=NULL) *section_offset-=pInf->section[isect].VirtualAddress;
return isect;
}


INT RawToSection(IN PEXEINFO pInf, IN DWORD rawpos, IN DWORD *section_offset)
{
INT i;
INT isect=-1;

for(i=0; i < pInf->nSections; i++)
   if((rawpos >= pInf->section[i].PointerToRawData) && (rawpos < (pInf->section[i].PointerToRawData+pInf->section[i].SizeOfRawData)))
      isect=i;
if(section_offset!=NULL) *section_offset-=pInf->section[isect].PointerToRawData;
return isect;
}


DWORD RVAToRaw(IN PEXEINFO pInf, IN DWORD rva)
{
WORD temp;
temp=RVAToSection(pInf, rva, &rva); // USAMOS rva como valor de retorno
if(temp == (WORD) -1) return -1;
return  rva + pInf->section[temp].PointerToRawData;
}


DWORD RawToRVA(IN PEXEINFO pInf, IN DWORD raw)
{
WORD temp;
temp=RawToSection(pInf, raw, &raw); // USAMOS rva como valor de retorno
return raw + pInf->section[temp].VirtualAddress;
}


// LEER CABECERA MZ
INT GetMZHeader(IN HANDLE hFile, OUT PIMAGE_DOS_HEADER pMZH)
{
BOOL retval;
DWORD nBytesRead;
SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
retval=ReadFile(hFile, pMZH, sizeof(IMAGE_DOS_HEADER), &nBytesRead, NULL);

if(retval == FALSE)
   return EXEINFO_NOMZ; // ERROR DE LECTURA

if(nBytesRead < sizeof(IMAGE_DOS_HEADER)) 
   return EXEINFO_NOMZ;

if(pMZH->e_magic != IMAGE_DOS_SIGNATURE)
   return EXEINFO_NOMZ; // NO HAY MZ AL PPIO

return EXEINFO_SUCCESS;
}


// LEER CABECERA PE A PARTIR DE CIERTO OFFSET
INT GetPEHeader(IN HANDLE hFile, IN DWORD offset, OUT PIMAGE_NT_HEADERS pPEH)
{
BOOL retval;
DWORD nBytesRead;

DEBUG("Desplazamiento de la cabecera PE: 0x%08X\n", offset);
SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
retval = ReadFile(hFile, pPEH, sizeof(IMAGE_NT_HEADERS), &nBytesRead, NULL);

if(retval == FALSE)
   return EXEINFO_NOPE;

if(nBytesRead < sizeof(IMAGE_NT_HEADERS))   // UNA DE ESTAS 2 COMPROBACIONES ES REDUNDANTE
   return EXEINFO_NOPE;   
   
if(pPEH->Signature != IMAGE_NT_SIGNATURE)
   return EXEINFO_NOPE;
   
return EXEINFO_SUCCESS;
}


// LEER TABLA DE SECCIONES
INT GetSectionTable(IN HANDLE hFile, IN DWORD offset, OUT IMAGE_SECTION_HEADER section[EXEINFO_MAXSECTIONS], IN INT nItems)
{
DWORD nBytesRead;
BOOL retval;

DEBUG("Desplazamiento de la tabla de secciones: 0x%08X\n", offset);
// ENCONTRAR Y LEER TABLA DE SECCIONES
SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
retval=ReadFile(hFile, section, nItems*sizeof(IMAGE_SECTION_HEADER), &nBytesRead, NULL);
if(retval == FALSE) return EXEINFO_NOSECTIONS;

return EXEINFO_SUCCESS;
}


#define GetFilePointer(hFile) SetFilePointer(hFile, 0, 0, FILE_CURRENT)

//
INT GetIAT(IN HANDLE hFile, IN OUT PEXEINFO pInfo)
{
IMAGE_IMPORT_DESCRIPTOR import0, import;
DWORD nBytesRead, offset;
BOOL retval;
INT i;
CHAR name[100];


//memset(&import0, 0, sizeof(IMAGE_IMPORT_DESCRIPTOR));
ZeroMemory(&import0, sizeof(IMAGE_IMPORT_DESCRIPTOR));

// ENCONTRAR Y LEER TABLA DE DESCRIPTORES DE IMPORTACION
pInfo->nIAT=0;


offset=RVAToRaw(pInfo, pInfo->PEH.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
DEBUG("VA de la tabla de imports: 0x%08X\n", pInfo->PEH.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);
DEBUG("Offset a la tabla de imports: 0x%08X\n", offset);
SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
retval=ReadFile(hFile, &import, sizeof(IMAGE_IMPORT_DESCRIPTOR), &nBytesRead, NULL);
if(retval == FALSE) return EXEINFO_NOIAT;

// SACAR Nº DE ENTRADAS
while(memcmp(&import, &import0, sizeof(IMAGE_IMPORT_DESCRIPTOR))) //!!!!!!!!!!!!!!!!!! -> WINAPI
{
   pInfo->nIAT++;
   retval=ReadFile(hFile, &import, sizeof(IMAGE_IMPORT_DESCRIPTOR), &nBytesRead, NULL);
   if(retval == FALSE) return EXEINFO_NOIAT;
}

// ESTO ES COMO HACER UN malloc(), VAMOS
pInfo->IAT=(IMAGE_IMPORT_DESCRIPTOR *)HeapAlloc(GetProcessHeap(), 0, sizeof(IMAGE_IMPORT_DESCRIPTOR)*pInfo->nIAT);

SetFilePointer(hFile, offset, NULL, FILE_BEGIN);
retval=ReadFile(hFile, pInfo->IAT, sizeof(IMAGE_IMPORT_DESCRIPTOR)*pInfo->nIAT, &nBytesRead, NULL);
if(retval == FALSE) return EXEINFO_NOIAT;

for(i=0; i<pInfo->nIAT; i++)
{
   SetFilePointer(hFile, RVAToRaw(pInfo, pInfo->IAT[i].Name), 0, FILE_BEGIN);
   retval=ReadFile(hFile, name, 100, &nBytesRead, NULL);
   DEBUG("Modulo a importar: %s\n", name);

}

return EXEINFO_SUCCESS;
}


// NOTA: EL FICHERO DEBE ESTAR ABIERTO
INT GetExeInfo(IN HANDLE hFile, OUT PEXEINFO pInfo)
{

if(GetMZHeader(hFile, &pInfo->MZH) != EXEINFO_SUCCESS) return EXEINFO_NOMZ;
if(GetPEHeader(hFile, pInfo->MZH.e_lfanew, &pInfo->PEH) != EXEINFO_SUCCESS) return EXEINFO_NOPE;
pInfo->nSections=pInfo->PEH.FileHeader.NumberOfSections;
DEBUGEXP(pInfo->MZH.e_lfanew);
DEBUGEXP(sizeof(DWORD));
DEBUGEXP(sizeof(IMAGE_FILE_HEADER));
DEBUGEXP(pInfo->PEH.FileHeader.SizeOfOptionalHeader);
DEBUGEXP(pInfo->PEH.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

if(GetSectionTable(hFile,
                   pInfo->MZH.e_lfanew+sizeof(DWORD)+sizeof(IMAGE_FILE_HEADER)+pInfo->PEH.FileHeader.SizeOfOptionalHeader,
                   pInfo->section,
                   pInfo->nSections)
   != EXEINFO_SUCCESS)return EXEINFO_NOSECTIONS;




// ENCONTRAR PUNTERO A LA TABLA DE IMPORTS
if(GetIAT(hFile, pInfo) != EXEINFO_SUCCESS) return EXEINFO_NOIAT;

pInfo->EntryPoint = pInfo->PEH.OptionalHeader.AddressOfEntryPoint;
pInfo->ImageBase = pInfo->PEH.OptionalHeader.ImageBase;
return EXEINFO_SUCCESS;
}
