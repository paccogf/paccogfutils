#ifndef _WINMACROS_H_
#define _WINMACROS_H_
#include <windows.h>

// CONVIERTE EL VALOR DEVUELTO POR la API GetLastError() A SU MENSAJE DE TEXTO
#define LastErrorString(errcode, buffer, buffsize) \
               FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,\
               NULL,\
               errcode,\
               MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
               (LPTSTR) buffer,\
               buffsize, \
               NULL );
               
      
//  GetLibraryProc = GetProcAddress + LoadLibrary
#define GetLibraryProc(strLib, strProc) GetProcAddress(LoadLibrary(strLib), strProc)


// DEVUELVE ???? 
#define IsKeyDown(vk_code) ((GetAsyncKeyState(vk_code)&0x8000)?1:0)



               
#endif
