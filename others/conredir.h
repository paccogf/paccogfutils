#ifndef _CONREDIR_H_
#define _CONREDIR_H_

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>


#define CON_INIT() { \
                     AllocConsole();\
                     *stdout=*_fdopen(_open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");\
                     setvbuf(stdout, NULL, _IONBF, 0 );\
                   }
                     
#define CON_FREE() FreeConsole();

#endif
