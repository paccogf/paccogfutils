#ifndef _MyLDElib_h_
#define _MyLDElib_h_
#include <windows.h>

typedef struct {
                int size;
                BYTE opcode[16];
} LDE_Instruction;



int LDE_Size(BYTE *);




#endif