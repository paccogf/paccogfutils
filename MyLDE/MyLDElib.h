#ifndef _MyLDElib_h_
#define _MyLDElib_h_
#include <windows.h>




#define LDE_ES      1
#define LDE_CS      2
#define LDE_SS      4
#define LDE_DS      8
#define LDE_FS      16
#define LDE_GS      32
#define LDE_LOCK    64
#define LDE_REPNE   128
#define LDE_REPE    256
#define LDE_66      512  // TAMAÑO OPERANDO
#define LDE_67      1024 // TAMAÑO DE DIRECCION


//int LDE_Size(BYTE *);
int LDE_Size(BYTE *, DWORD *);





#endif
