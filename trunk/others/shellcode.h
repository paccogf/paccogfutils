#ifndef _SHELLCODE_H_
#define _SHELLCODE_H_
#include <windows.h>
#include <winbase.h>


// NECESITA:
//      pid:         PID DEL PROCESO EN EL QUE INYECTARSE
//      lpshell:     PUNTERO AL CODIGO A INYECTAR 
//      shellsize:   TAMAÑO DEL SHELLCODE
//      lpdata:      PUNTERO A LOS DATOS QUE EL SHELLCODE RECIBIRA COMO PARAMETRO
//      datasize:    TAMAÑO DE LOS DATOS
// DEVUELVE:
//      0 SI HUBO EXITO O GetLastError() SI NO
// COMENTARIO: EL SHELLCODE DEBE USAR LA CONVENCION DE LLAMADA __stdcall

DWORD InjectShellCode(DWORD pid, LPVOID lpshell, DWORD shellsize, LPVOID lpdata, DWORD datasize);


#endif
