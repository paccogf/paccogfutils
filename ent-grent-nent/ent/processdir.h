#ifndef _PROCESSDIR_H_
#define _PROCESSDIR_H_
#include <windows.h>

////////////////////////////////////////
 // APLICA CADA A FICHERO DE pattern LA EXPRESION do_expr
#define PROCESSDIR(pattern, do_expr)\
{\
    WIN32_FIND_DATA _PROCESSDIR_DATA_;\
    HANDLE _PROCESSDIR_FIND_;\
    INT _PROCESSDIR_ITR_;\
\
    _PROCESSDIR_FIND_ = FindFirstFile(pattern, &_PROCESSDIR_DATA_);\
    if(_PROCESSDIR_FIND_ == INVALID_HANDLE_VALUE) _PROCESSDIR_ITR_ = 0;\
    else _PROCESSDIR_ITR_ = 1;\
\
    while(_PROCESSDIR_ITR_ > 0)\
    {\
        if(_PROCESSDIR_ITR_ > 1)/* YA NO ESTAMOS EN LA PRIMERA ITERACION*/\
        {\
            if(FindNextFile(_PROCESSDIR_FIND_, &_PROCESSDIR_DATA_) == FALSE) break;\
        }\
        _PROCESSDIR_ITR_++;\
        do_expr;\
\
    }\
}

// HACER SANDWICH CON EL CÓDIGO A EJECUTAR POR FICHERO
#define PROCESSDIR_BEGIN(pattern, _PROCESSDIR_DATA_)\
{\
    WIN32_FIND_DATA _PROCESSDIR_DATA_;\
    HANDLE _PROCESSDIR_FIND_;\
    INT _PROCESSDIR_ITR_;\
\
    _PROCESSDIR_FIND_ = FindFirstFile(pattern, &_PROCESSDIR_DATA_);\
    if(_PROCESSDIR_FIND_ == INVALID_HANDLE_VALUE) _PROCESSDIR_ITR_ = 0;\
    else _PROCESSDIR_ITR_ = 1;\
\
    while(_PROCESSDIR_ITR_ > 0)\
    {\
        if(_PROCESSDIR_ITR_ > 1)/* YA NO ESTAMOS EN LA PRIMERA ITERACION*/\
        {\
            if(FindNextFile(_PROCESSDIR_FIND_, &_PROCESSDIR_DATA_) == FALSE) break;\
        }\
        _PROCESSDIR_ITR_++;

// CODIGO DE USUARIO AQUI...

#define PROCESSDIR_END\
    }\
}



// FUNCIONES DE RECORRIDO RECURSIVO

typedef VOID (*FILEFUNC)(WIN32_FIND_DATA *);
//////////////////////////////////////////////
//
INT ProcessDir(LPCTSTR pattern, FILEFUNC do_fun)
{
WIN32_FIND_DATA fdFindData;
HANDLE hIteration;

hIteration = FindFirstFile(pattern, &fdFindData);
if(hIteration == INVALID_HANDLE_VALUE) return GetLastError();


do
{
    do_fun(&fdFindData);

}while(FindNextFile(hIteration, &fdFindData));



return ERROR_SUCCESS;
}

///////////////////////////////////////////////
//
INT RecurListDir(LPCTSTR pattern, FILEFUNC do_fun, BOOL bRecursive)
{
CHAR buffer[MAX_PATH];
WIN32_FIND_DATA fdFindData;
HANDLE hIteration;

hIteration = FindFirstFile(pattern, &fdFindData);
if(hIteration == INVALID_HANDLE_VALUE) return GetLastError();


do
{
    if((fdFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && bRecursive) 
    {
       // SetCurrentDirectory();
        RecurListDir(buffer, do_fun, TRUE);
     
    }
    else
    {
        do_fun(&fdFindData);
    }

}while(FindNextFile(hIteration, &fdFindData));


return ERROR_SUCCESS;
}


///////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////
/*
#define PROCESSDIR(pattern, do_expr)\
{\
    WIN32_FIND_DATA PROCESSDIR_DATA;\
    HANDLE hIteration;\
    INT doItr;\
\
    hIteration = FindFirstFile(pattern, &PROCESSDIR_DATA);\
    if(hIteration == INVALID_HANDLE_VALUE) doItr=0;\
    else doItr=1;\
\
    while(doItr>0)\
    {\
        if(doItr>1)\
        {\
            if(FindNextFile(hIteration, &PROCESSDIR_DATA) == FALSE) break;\
        }\
        doItr++;\
        do_expr;\
    }\
}\
*/

////////////////////////////////////////////////////////////////
/*
#define PROCESSDIR(pattern, do_fun) \
{\
WIN32_FIND_DATA PROCESSDIR_DATA;\
HANDLE hIteration;\
\
hIteration = FindFirstFile(pattern, &PROCESSDIR_DATA);\
if(hIteration != INVALID_HANDLE_VALUE)\
\
do{\
    do_fun;\
\
}while(FindNextFile(hIteration, &PROCESSDIR_DATA));\
}\
*/

//// LOS DATOS DEL FICHERO EN PROCESO ESTÁN EN PROCESSDIR_FINDDATA
/*
#define PROCESSDIR_BEGIN(pattern) \
{\
WIN32_FIND_DATA PROCESSDIR_DATA;\
HANDLE _PD_hIteration_;\
\
_PD_hIteration_ = FindFirstFile(pattern, &PROCESSDIR_DATA);\
if(_PD_hIteration_ != INVALID_HANDLE_VALUE)\
do {\


#define PROCESSDIR_END \
}while(FindNextFile(_PD_hIteration_, &PROCESSDIR_DATA));\
}
*/

#endif
