#include "dirrun.h"


#ifndef INVALID_FILE_ATTRIBUTES
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1) // PARA MSVC60
#endif

#define _DIRRUN_H_MAX_PATH (MAX_PATH * 5) 

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
    sprintf(szTempPath, "%s\\%s", szPath, szWildCard);
    hFind=FindFirstFile(szTempPath, &FindFileData);

    if(hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
            {
                sprintf(szNextPath, "%s\\%s", szPath, FindFileData.cFileName);
                function(szNextPath, ptrToStruct);
            }

        }while(FindNextFile(hFind, &FindFileData) != 0);
        
		FindClose(hFind);
    }

    if(bSubDirs)
    {
    // PROCESAR "*" DIRECTORIOS
        sprintf(szTempPath, "%s\\%s", szPath, "*");
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
                    sprintf(szNextPath, "%s\\%s", szPath, FindFileData.cFileName, szWildCard);
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

    sprintf(szTempPath, "%s\\%s", szPath, szWildCard);
    hFind=FindFirstFile(szTempPath, &FindFileData);
    if(hFind == INVALID_HANDLE_VALUE) return 0;
    do
    {
        sprintf(szNextPath, "%s\\%s", szPath, FindFileData.cFileName);
        if(     // SI ES DIRECTORIO
          ((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
           && (strcmp(FindFileData.cFileName, "."))
           && (strcmp(FindFileData.cFileName, ".."))
           && bDirProc // Y QUIERO PROCESAR DIRECTORIOS..
          )
        {
       
            //strcat(szNextPath, "\\");
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


//////////////////////////////////////////////////////////////////
// EL PATH ES DE LA FORMA C:\dir1\dir2\<wildcard>
// NO PUEDE ACABAR CON UNA BARRA '\'
// EL ULTIMO ELEMENTO DEL PATH DEBE SER UN FICHERO O UN WILDCARD
/////////////////////////////////////////
//
int DirRun(const char *szPathAndWildCard, BOOL bSubDirs, DirEntryFunc function, void *ptrToStruct)
{
int result=drNoPath;
int lPath;
char *szPath;
char *szWildcard;

	lPath=GetFullPathName(szPathAndWildCard, 0, NULL, NULL);

	if(lPath > 0)
	{
		szPath=malloc(lPath);
	
		if(szPath)
		{

			lPath=GetFullPathName(szPathAndWildCard, lPath, szPath, NULL);

			szWildcard = strrchr(szPath, '\\');

			if(szWildcard != NULL) // ESTO NUNCA DEBERIA OCURRIR!!!
			{
				
				*szWildcard=0;
				szWildcard++; // APUNTAR A LO QUE HAYA TRAS 

				if(*szWildcard == 0) // NO HAY NADA EN LA PARTE DE WILDCARD
				{
					result = drNoWildcard;
				}
				else if(strchr(szPath, '*')) // WILDCARD EN LA PARTE DE DIRECTORIO!
				{
					result = drBadFormat;
				}
				else if(!strchr(szWildcard, '*'))  // NO HAY WILDCARD EN LA PARTE DE WILDCARD?
				{
					// PUES AL MENOS QUE SEA UN NOMBRE DE FICHERO...
				
					int fattr;
					*(szWildcard-1) = '\\'; // RESTAURAR szPath

					fattr=GetFileAttributes(szPath);

					if(fattr==INVALID_FILE_ATTRIBUTES)
					{
						result = drInvalidPath;
					}
					else if(fattr & FILE_ATTRIBUTE_DIRECTORY) // SI ES DIRECTORIO NO NOS VALE
					{
						result = drPathIsDirectory;
					}
					else
					{
						result = function(szPath, ptrToStruct); // szPath SE REFIERE A UN FICHERO
					}
				}
				else
				{
					result = _DirRun(szPath, szWildcard, bSubDirs, function, ptrToStruct);
				}
			}
			else
			{
				result = drUnknownError;
			}

			free(szPath);
		}

	}




return result;
}
