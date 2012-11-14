#include "rc4.h"

//
//	TO DO: QUE TODA LA ARITMETICA SEA "MODULO 256" (CON BYTES)
//




__inline
void SwapTable(BYTE Table[], int i, int j)
{
BYTE aux;

	aux=Table[i];
	Table[i]=Table[j];
	Table[j]=aux;

}

// REINICIALIZACION DE LA TABLA DE ESTADO DEL RC4
void RC4_Reset(RC4_CTX *rc4ctx, BYTE key[], int keylen)
{
int i, j;

	for(i=0; i<RC4_TABLE_SIZE; i++) // INICIALIZACION DE LA TABLA DE PERMUTACIONES
	{
		rc4ctx->Table[i] = i;
	}

	j=0;
	for(i=0; i<RC4_TABLE_SIZE; i++) // APLICAMOS EL KSA
	{

		j = (j + rc4ctx->Table[i] + key[i % keylen]) % RC4_TABLE_SIZE;
		//swapb(&rc4ctx->Table[i], &rc4ctx->Table[j]);
		SwapTable(rc4ctx->Table, i, j);
	}

	rc4ctx->i=0;
	rc4ctx->j=0;
}



RC4_CTX * RC4_Init(BYTE key[], int keylen)
{

RC4_CTX *retval = (RC4_CTX *) malloc(sizeof(RC4_CTX), 0);

	if(retval != NULL) 
	{
		
		RC4_Reset(retval, key, keylen);

	}

return retval;
}





BYTE RC4_Next(RC4_CTX *rc4ctx)
{
BYTE *S;
int *lpi, *lpj;
BYTE K;


	S = rc4ctx->Table;
	lpi = &rc4ctx->i;
	lpj = &rc4ctx->j;

	
	*lpi = (*lpi + 1) % RC4_TABLE_SIZE;
	*lpj = (*lpj + S[*lpi]) % RC4_TABLE_SIZE;
	SwapTable(S, *lpi, *lpj);

	K=S[(S[*lpi]+S[*lpj]) % RC4_TABLE_SIZE];


return K;
}





void RC4_Finish(RC4_CTX *rc4ctx)
{
	//FREE(rc4ctx->Table);
	free(rc4ctx);
}



// ENCRIPTAR/DECRIPTAR UN BLOQUE DE DATOS
void RC4_Crypt(RC4_CTX *rc4ctx, BYTE *data, int ndata)
{
int i;

	for(i=0; i<ndata; i++)
	{
		data[i] ^= RC4_Next(rc4ctx);
	}

}


/**********************************************************************************

//
// EXAMPLE OF USE
//
int main(int argc, char *argv[], char *argenv[])
{
RC4_CTX * rc4ctx;
static const char key[] = "MyKeY";
char message[] = "Hello, world";



	rc4ctx = RC4_Init("", sizeof(key)-1);
	
	RC4_Crypt(rc4ctx, message, sizeof(message)-1);

	RC4_Finish(rc4ctx);



return 0;
}


*****************************************************************************************/
