#ifndef _SHELLCODE_H_
#define _SHELLCODE_H_
#include <windows.h>
#include <winbase.h>

#include "cast.h"


// NECESITA: 
//      module:     NOMBRE DE LA DLL A CARGAR/INYECTAR
//      function:   NOMBRE DE LA FUNCION A INVOCAR DEL TIPO void (__cdecl(stdcall) *)(void)
// DEVUELVE:
//      PUNTERO AL SHELLCODE Y TAMA�O DE ESTE EN size
LPBYTE MakeShellCode(LPINT size, LPTSTR module,  LPTSTR function);


// NECESITA:
//      pid:        PID DEL PROCESO EN EL QUE INYECTARSE
//      shellcode:  VECTOR CON EL CODIGO A INYECTAR
//      size:       TAMA�O DEL VECTOR
// DEVUELVE:
//      0 SI HUBO EXITO O GetLastError() SI NO
DWORD InjectShellCode(DWORD pid, LPBYTE shellcode, INT size);


#endif