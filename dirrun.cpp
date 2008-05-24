#include "dirrun.h"


//#define _DIRRUN_H_RECURSIVE 
#ifndef _DIRRUN_H_RECURSIVE 

/******************************************* VERSION ITERATIVA **************************************/ 

int _DirRun(const char *szPath, const char *szWildCard, BOOL bSubDirs, DirEntryFunc function, void *ptrToStruct)
{
char szTempPath[_DIRRUN_H_MAX_PATH];
char szNextPath[_DIRRUN_H_MAX_PATH];
WIN32_FIND_DATA FindFileData;
HANDLE hFind; 

    // PRIMERO PROCESAR TODOS LOS FICHEROS CON EL PATRON ESPECIFICADO
    sprintf(szTempPath, "%s%s", szPath, szWildCard);
    hFind=FindFirstFile(szTempPath, &FindFileData);
    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
            {
                sprintf(szNextPath, "%s%s", szPath, FindFileData.cFileName);
                function(szNextPath, ptrToStruct);
            }
        }while(FindNextFile(hFind, &FindFileData) != 0);
        FindClose(hFind);
    }
    if(bSubDirs)
    {
    // PROCESAR "*" DIRECTORIOS
        sprintf(szTempPath, "%s%s", szPath, "*");
        hFind=FindFirstFile(szTempPath, &FindFileData);
        if(hFind != INVALID_HANDLE_VALUE)
        {
            do
            {
              if(
                  ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
                   && (strcmp(FindFileData.cFileName, "."))
                   && (strcmp(FindFileData.cFileName, ".."))
                  )
                {
                  //
                    sprintf(szNextPath, "%s%s\\", szPath, FindFileData.cFileName, szWildCard);
                    _DirRun(szNextPath, szWildCard, TRUE, function, ptrToStruct);
                }
   
            }while(FindNextFile(hFind, &FindFileData) != 0);
            FindClose(hFind);
        }
    }
return 0;
} 

#else /****************************************** VERSION RECURSIVA ALTERNA ******************************************/ 

typedef struct {
    char *szWildCard;
    BOOL bSubDirs;
    DirEntryFunc ptrfunc;
    void *ptrToStruct;
} stDirRun;
 


// LLAMADA RECURSIVA ALTERNA A _DirRunImpl 

int _FileRun(char *szPath, char *szWildCard, BOOL bDirProc, DirEntryFunc lpfunc, void *ptrToStruct)
{
int retval=0;
char szTempPath[_DIRRUN_H_MAX_PATH];
char szNextPath[_DIRRUN_H_MAX_PATH];
WIN32_FIND_DATA FindFileData;
HANDLE hFind; 

    sprintf(szTempPath, "%s%s", szPath, szWildCard);
    hFind=FindFirstFile(szTempPath, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE) return 0;
    do
    {
        sprintf(szNextPath, "%s%s", szPath, FindFileData.cFileName);
        if(     // SI ES DIRECTORIO
          ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
           && (strcmp(FindFileData.cFileName, "."))
           && (strcmp(FindFileData.cFileName, ".."))
           && bDirProc // Y QUIERO PROCESAR DIRECTORIOS..
          )
        {
        
            strcat(szNextPath, "\\");
            lpfunc(szNextPath, ptrToStruct);
        
        }
        else // SI NO ES DIRECTORIO
        {
            if(!bDirProc) lpfunc(szNextPath, ptrToStruct); // Y NO QUIERO PROCESAR DIRECTORIOS ...
        }
    }while(FindNextFile(hFind, &FindFileData) != 0);
    FindClose(hFind);
return retval;
} 

// LLAMADA RECURSIVA ALTERNA  A _FileRun
int __DirRunImpl(char *szPath, void *st)
{
stDirRun *stDR=(stDirRun *) st;
    _FileRun(szPath, stDR->szWildCard, FALSE, stDR->ptrfunc, stDR->ptrToStruct); // PROCESAR FICHEROS
    if(stDR->bSubDirs) _FileRun(szPath, "*", TRUE, __DirRunImpl, stDR);   // PROCESAR DIRECTORIOS 

return 0;
} 

// 

// CONVERSION DE PARAMETROS
_inline int _DirRun(char *szPath, char *szWildCard, BOOL bSubDirs, DirEntryFunc lpdirfunc, void *ptrToStruct)
{
stDirRun stDR = {szWildCard, bSubDirs, lpdirfunc, ptrToStruct }; 
 
    __DirRunImpl(szPath, &stDR);

return 0;
}
 
#endif 


/******************** FUNCION PRINCIPAL ********************************************************************************/ 

/////////////// FALLA AL METERLE "C:\" -> LO INTERPRETA COMO "C:*"


//////////////////////////////////////////////////////
// EL PATH ES DE LA FORMA C:\dir1\dir2\<wildcard> 
// PUEDE ACABAR CON UNA BARRA '\' 
// EL ULTIMO ELEMENTO DEL PATH DEBE SER UN FICHERO O UN WILDCARD 
/////////////////////////////////////////

int DirRun(const char *szPathAndWildCard, BOOL bSubDirs, DirEntryFunc function, void *ptrToStruct)
{
char szPath[_DIRRUN_H_MAX_PATH]="";
char szWildCard[_DIRRUN_H_MAX_PATH]="";
char szAux[_DIRRUN_H_MAX_PATH]="";
int lastslab;
int i; 
char *c;

    // 
    // 1.- VALIDACION DEL PATH QUE HEMOS PASADO A LA FUNCION
    // 
    strcpy(szPath, szPathAndWildCard);
    i=strlen(szPath);
    

    lastslab=-1;
    c=strrchr(szPath, '\\');
    if(c == NULL)  // NO HAY '\\' 
    {

        c=strchr(szPath, ':');  // NO HAY ':' -> ruta relativa
        if(c == NULL)
        {
            // 3 OPCIONES, szPath PUEDE SER FICHERO, NOMBRE O COMODINES (PERO SIEMPRE RUTA RELATIVA)
            strcpy(szWildCard, szPath); // LO COPIAMOS COMO WILDCARD
            GetCurrentDirectory(_DIRRUN_H_MAX_PATH, szPath);


        }
        else   // RUTA TIPO C:<ALGO>
        {
            strcpy(szWildCard, szPath+2);
            
            *(WORD *) szAux = *(WORD *) szPath;
            szAux[2]=0;

            GetFullPathName(szAux, _DIRRUN_H_MAX_PATH, szPath, NULL);
         
        }

        strcat(szPath, "\\");

    }
    else // HAY AL MENOS UN '\\'
    {
        strcpy(szWildCard, c + 1);

        *(c+1) = 0; // PONER A CERO LO SIGUIENTE en szPath
        strcpy(szAux, szPath);
        GetFullPathName(szAux, _DIRRUN_H_MAX_PATH, szPath, NULL);

    }

    // szPath YA ES UN VALOR VALIDO. FALTA MIRAR szWildCard. 4 CASOS: VACIO, DIRECTORIO, FICHERO, WILDCARD

    // CASO 1: VACIO
    if(szWildCard[0] == 0) // SI NO HAY WILDCARD, SE PONE A *
    {
        strcpy(szWildCard, "*");
       
    }
    // CASO WILDCARD
    else if(strchr(szWildCard, '*') || strchr(szWildCard, '?'))
    {
        // YA ESTA TODO HECHO


    }
    // MIRAR A VER SI ES DIRECTORIO O FICHERO
    else
    {
        sprintf(szAux, "%s%s", szPath, szWildCard);
        i=GetFileAttributes(szAux);

        if((i != INVALID_FILE_ATTRIBUTES) && (i & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) // FICHERO O DIRECTORIO QUE NO EXISTE -> PATRON SIN COMODINES
        {
            strcat(szPath, szWildCard);
            strcat(szPath, "\\");
            strcpy(szWildCard, "*");
        }
    }


return _DirRun(szPath, szWildCard, bSubDirs, function, ptrToStruct);
}
  
