#include "Xinf.h"
//
//
//
// NOTA: EL FICHERO DEBE ESTAR ABIERTO
INT GetExeInfo(IN BYTE *lpFileBuffer, IN DWORD nFileBuffer, OUT EXEINFO *lpXInfo)
{
    //
    // COMPROBAR QUE HAY ESPACIO PARA CABECERA MZ
    if(nFileBuffer < sizeof(IMAGE_DOS_HEADER)) 
    {
       return EXEINFO_NOMZ;
    }

    //
    // LEER CABECERA MZ
    lpXInfo->MZ = (IMAGE_DOS_HEADER *) lpFileBuffer;
    
    //
    // COMPROBAR FIRMA MZ
    if(lpXInfo->MZ->e_magic != (WORD) 'ZM') 
    {
       return EXEINFO_NOMZ;
    }
    

    //
    // COMPROBAR SI HAY ESPACIO PARA CABECERA PE
    if(nFileBuffer < (lpXInfo->MZ->e_lfanew + sizeof(IMAGE_NT_HEADERS))) 
    {
       return EXEINFO_NOPE;
    }
    
    // 
    // LEER CABECERA PE
    lpXInfo->PE = (IMAGE_NT_HEADERS *) (lpFileBuffer + lpXInfo->MZ->e_lfanew);
    
    //
    // COMPROBAR FIRMA PE
    if(lpXInfo->PE->Signature != (ULONG) 'EP')
    {
       return EXEINFO_NOPE;   
    }

    //
    // COMPROBAR QUE HAYA ESPACIO PARA LA TABLA DE SECCIONES
    if(nFileBuffer < (lpXInfo->MZ->e_lfanew + 
                      sizeof(IMAGE_NT_HEADERS) + 
                      lpXInfo->PE->FileHeader.NumberOfSections * sizeof(IMAGE_SECTION_HEADER)
                     )
      ) 
    {
       return EXEINFO_NOSECTIONS;
    }
    
    //
    // LEER CABECERA DE SECCIONES
    lpXInfo->section = (IMAGE_SECTION_HEADER *) ((BYTE *)lpXInfo->PE + sizeof(IMAGE_NT_HEADERS));


    lpXInfo->size=nFileBuffer;

// ENCONTRAR PUNTERO A LA TABLA DE IMPORTS
/*
if(GetIAT(hFile, pInfo) != EXEINFO_SUCCESS) 
{
   return EXEINFO_NOIAT;
}
*/

return EXEINFO_SUCCESS;
}



SHORT _RVAToSection (IN PEXEINFO pInf, IN DWORD rva, OUT DWORD *section_offset)
{
SHORT i;
SHORT isect = -1;

    for(i=0; i < pInf->PE->FileHeader.NumberOfSections; i++)
    {
       if((rva >= pInf->section[i].VirtualAddress) &&
          //(rva < (pInf->section[i].VirtualAddress + pInf->section[i].Misc.VirtualSize)))
          (rva < (pInf->section[i].VirtualAddress + pInf->section[i].SizeOfRawData)))
       {
          isect=i;
          //break;
       }
    }

    if(section_offset != NULL) 
    {
        *section_offset = rva - pInf->section[isect].VirtualAddress;
    }

return isect;
}


DWORD RVAToRaw(IN PEXEINFO pInf, IN DWORD rva)
{
WORD section;
DWORD retval = (DWORD) -1;

    section = _RVAToSection(pInf, rva, &rva); // USAMOS rva como valor de retorno

    if((section != (WORD) -1) && (rva < pInf->section[section].SizeOfRawData)) // LA RVA DEBE DE ESTAR DENTRO DE LA SECCION FISICA!
    {
        retval = rva + pInf->section[section].PointerToRawData;
    }
    
return retval;
}


SHORT _RawToSection(IN PEXEINFO pInf, IN DWORD rawpos, IN DWORD *section_offset)
{
SHORT i;
SHORT isect = -1;

    for(i=0; i < pInf->PE->FileHeader.NumberOfSections; i++)
    {
       if((rawpos >= pInf->section[i].PointerToRawData) &&
          (rawpos < (pInf->section[i].PointerToRawData + pInf->section[i].SizeOfRawData)))
       {
          isect=i;
          //break;
       }
    }

    if(section_offset != NULL)
    {
        *section_offset = rawpos - pInf->section[isect].PointerToRawData;
    }

return isect;
}

DWORD RawToRVA(IN PEXEINFO pInf, IN DWORD raw)
{
WORD section;
DWORD retval = (WORD) -1;

    section = _RawToSection(pInf, raw, &raw); // USAMOS rva como valor de retorno

    if(section != (WORD) -1) 
    {
        retval = raw + pInf->section[section].VirtualAddress;
    }

return retval;
}

/****** FUNCIONES DE MAPEO DE FICHEROS ******/

// RETORNA 
PMAPHANDLE MapFile(LPCTSTR IN filename, BYTE OUT **filebuffer, DWORD OUT *nBytes)
{
HANDLE hFile, hFileMapping;
PMAPHANDLE pMapHandle;
 
    // ABRIR EL FICHERO
    hFile=CreateFile( filename,
                      GENERIC_READ | GENERIC_WRITE, 
                      FILE_SHARE_READ, 
                      NULL, 
                      OPEN_EXISTING, 
                      FILE_ATTRIBUTE_NORMAL, 
                      NULL);

    if(hFile == INVALID_HANDLE_VALUE) 
    {
        return NULL;
    }


    //
    // MAPEAR EL FICHERO
    hFileMapping = CreateFileMapping(hFile, 
                                     NULL, 
                                     PAGE_READWRITE, 
                                     0, 
                                     0, 
                                     NULL);

    if(hFileMapping == NULL) 
    {
        CloseHandle(hFile);
        return NULL;
    }


    //
    //  CREAR LA VISTA DE FICHERO
    *filebuffer = (BYTE *) MapViewOfFile( hFileMapping, 
                               FILE_MAP_WRITE, 
                               0, 
                               0, 
                               0);

    if(*filebuffer == NULL)
    {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return NULL;
    }


    //
    // DEVOLVER LA ESTRUCTURA CON LOS DATOS SI SE PUEDE, SI NO, CERRAR
    pMapHandle = (MAPHANDLE *) malloc(sizeof(MAPHANDLE));
    if(pMapHandle == NULL)
    {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return NULL;
    }


    *nBytes = GetFileSize(hFile, NULL);
    pMapHandle->hFile = hFile;
    pMapHandle->hFileMapping = hFileMapping;

return pMapHandle;
}


DWORD UnMapFile(PMAPHANDLE pmhFile)
{
    if(pmhFile == NULL) return (DWORD) -1;

    // CERRAR HANDLES
    CloseHandle(pmhFile->hFileMapping);
    CloseHandle(pmhFile->hFile);


    // LIBERAR MEMORIA
    free(pmhFile);

return 0;
}

/*

//#define _DEBUG
#ifdef _DEBUG
    #define DEBUG           printf
    #define DEBUGEXP(exp)   printf("%s = 0x%08X\n", #exp, exp)
#else
    #define DEBUG           voidfunc
    #define DEBUGEXP        voidfunc
    __inline void voidfunc()
    {
    }
#endif

  


typedef struct _IAT
{
IMAGE_IMPORT_DESCRIPTOR *descriptor;
INT nIAT;





} IAT, PIAT;

    
      
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



*/

