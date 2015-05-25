#ifndef _CONREDIR_H_
#define _CONREDIR_H_

#include <windows.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdio.h>

#define CON_INIT() { \
                     AllocConsole();\
                     *stdout=*_fdopen(_open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");\
                     setvbuf(stdout, NULL, _IONBF, 0 );\
                                      }

#define CON_FREE() FreeConsole();


// redirect unbuffered WIN32 CONSOLE HANDLE IO to C/C++ stdin/stdout/stderr
//	USO ESTANDAR 
//	WIN32_IO_TO_C(STD_INPUT_HANDLE, stdin) --> REDIRIGIR ENTRADA DE CONSOLA
//	WIN32_IO_TO_C(STD_OUTPUT_HANDLE, stdout) --> REDIRIGIR SALIDA CONSOLA
//	WIN32_IO_TO_C(STD_ERROR_HANDLE, stderr)  --> REDIRIGIR SALIDA DE ERROR A CONSOLA

#define WIN32_IO_TO_C(STD_IO_HANDLE, stdio_handle) setvbuf(*stdio_handle = *_fdopen(_open_osfhandle( (long)GetStdHandle(STD_IO_HANDLE), _O_TEXT), "r"), NULL, _IONBF, 0)
// EQUIVALENTE AL SIGUIENTE CODIGO DE EJEMPLO:












#endif