#ifndef _COMMON_H_
#define _COMMON_H_

#include <windows.h>
#include "Xinf.h"
#include "dirrun.h"
#include "md5.h"

typedef struct {

    BOOL bRecursive;
    BOOL bNoMD5;
    char * szDBFile;
    char * szPathSpec;

} ProgArgs;


extern ProgArgs gProgArgs;

char * GetMD5(BYTE *buffer, int size, char *literal);

#endif