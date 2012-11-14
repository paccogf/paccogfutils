#ifndef __RC4_H__
#define __RC4_H__

#include <stdlib.h>

#ifndef BYTE
typedef char BYTE
#endif

#pragma pack(push, 1)

#ifdef __cplusplus
extern "C" {
#endif

#define RC4_TABLE_SIZE 256

// ESTRUCTURA QUE ALMACENA EL CONTEXTO DE LA TABLA ENTRE LLAMADAS A RC4_NEXT
typedef struct _RC4_CTX
{
	BYTE Table[RC4_TABLE_SIZE];
	int i;
	int j;

} RC4_CTX;


// PROTOTIPOS
RC4_CTX * RC4_Init(BYTE key[], int keylen);

void RC4_Reset(RC4_CTX *rc4ctx, BYTE key[], int keylen);

BYTE RC4_Next(RC4_CTX *rc4ctx);

void RC4_Finish(RC4_CTX *rc4ctx);

void RC4_Crypt(RC4_CTX *rc4ctx, BYTE *cypher, int ncypher);


#ifdef __cplusplus
}
#endif


#pragma pack(pop)

#endif