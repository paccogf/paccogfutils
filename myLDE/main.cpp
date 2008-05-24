#include <stdio.h>
#include <stdlib.h>
#include "lde.h"



int main(int argc, char *argv[])
{
BYTE buffer[] = { 0xFF,0x15,0xD0,0x50,0x40,0x00 };
  
  
  disasm(buffer);
  
  printf("disasm_len=%d\n", disasm_len);
  printf("operando: 0x%0.8X", *(DWORD *)disasm_data);
  system("PAUSE");	
  return 0;
}
