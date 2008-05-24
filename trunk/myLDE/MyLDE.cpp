#include <stdio.h>
#include <io.h>
#include "MyLDElib.h"
#include "exeinfo.h"

void procesa(BYTE *opcode, int opsize, int *base)
{
//static int fullsize;
int i;

printf(".%08X: [%2d bytes] ", *base, opsize);
for(i=0; i<opsize; i++)
    printf("%02X", opcode[i]);
printf("\n");

(*base)+=opsize;
}



int main(int argc, char *argv[])
{
int retvalue=0;
FILE *fFile;
HANDLE hFile;
EXEINFO xInfo;
DWORD opsize;
int base;
BYTE buffer[0x1000];
BYTE *opcode;
LPTSTR filename;
BOOL bPause=TRUE;

static BYTE ins00[]={ 0x89, 0x54, 0x0E, 0x0C };  // mov     [esi+ecx+0Ch], edx
static BYTE ins01[]={ 0x89, 0x44, 0x0E, 0x08 };  // mov     [esi+ecx+8], eax
static BYTE ins02[]={ 0x89, 0x5C, 0x0E, 0x04 };  // mov     [esi+ecx+4], ebx
static BYTE ins03[]={ 0x89, 0x3C, 0x0E };        // mov     [esi+ecx], edi

BYTE *ins0[]={ins00, ins01, ins02, ins03};



if(argc < 2) return -1;
filename=argv[1];
if(argc > 2) 
{
    if(!strcmp(filename, "-nopause"))
    {
         bPause=FALSE;
         filename=argv[2];
    }
}



fFile=fopen(filename, "rb");
if(fFile==NULL) return -2;



hFile=(HANDLE)_get_osfhandle(_fileno(fFile));
GetExeInfo(hFile, &xInfo);

base=xInfo.EntryPoint;

fseek(fFile, RVAToRaw(&xInfo, base), SEEK_SET);
fread(buffer, sizeof(buffer), 1, fFile);

base+=xInfo.ImageBase;
opcode=buffer;
opsize=LDE_Size(opcode);
if(opsize==0) 
{
    retvalue = -3; // POR SI ACASO;
}
else
{
    while((opcode-buffer) < 0xFF0)  // 0xC3=ret
    {
  
        procesa(opcode, opsize, &base);
 
        opcode+=opsize;
        opsize=LDE_Size(opcode);
        if(opsize==0) 
        {
            retvalue=-4; // POR SI ACASO;
            break;
        }
    }
    
}

fclose(fFile);
printf("\n%d\n", retvalue);
if(bPause) system("PAUSE");
return retvalue;
}