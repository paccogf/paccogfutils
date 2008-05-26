#include <stdio.h>
#include <io.h>
#include "MyLDElib.h"
#include "exeinfold.h"

void procesa(BYTE *opcode, int opsize, int *base, DWORD status)
{
//static int fullsize;
int i;

printf(".%08X: [%2d bytes] %08X ", *base, opsize, status);
for(i=0; i<opsize; i++)
    printf("%02X", opcode[i]);
printf("\n");

(*base)+=opsize;
}



int main(int argc, char *argv[])
{
EXEINFO xInfo;
BYTE buffer[0x1000];
FILE *fFile;
HANDLE hFile;
BYTE *opcode;
DWORD status;
int opsize, base, retvalue=0;


if(argc < 2) return -1;
printf("%s", argv[1]);
fFile=fopen(argv[1], "rb");
if(fFile==NULL) return -2;



hFile=(HANDLE)_get_osfhandle(_fileno(fFile));
                
                
GetExeInfo(hFile, &xInfo);

base=xInfo.EntryPoint;


//base=0x3A00;
fseek(fFile, RVAToRaw(&xInfo, base), SEEK_SET);
fread(buffer, sizeof(buffer), 1, fFile);

base+=xInfo.ImageBase;


opcode=buffer;
opsize=LDE_Size(opcode, &status);
if(opsize==0) 
{
    retvalue = -3; // POR SI ACASO;
}
else
{
    while((opcode-buffer) < 0xFF0)  // 0xC3=ret
    {
  
        procesa(opcode, opsize, &base, status);
 
        opcode+=opsize;
        opsize=LDE_Size(opcode, &status);
        if(opsize==0) 
        {
            retvalue=-4; // POR SI ACASO;
            break;
        }
    }
    
}

fclose(fFile);
printf("\n%d\n", retvalue);
system("PAUSE");
return retvalue;
}