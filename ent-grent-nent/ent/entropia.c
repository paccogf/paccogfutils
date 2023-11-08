/******************************************************************************/
/************************* FICHERO ENTROPIA.C *********************************/
/******************************************************************************/
#include "entropia.h"
#include <stdio.h>
#include <stdlib.h>

double entropia_old(unsigned char *buffer, unsigned int size)
{
double freqs[256];
double result=0.0;
unsigned int i;

//memset(freqs, 0, 256*sizeof(double));
if(size == 0) return 0.0;

for(i=0; i<256; i++) freqs[i]=0.0;  // PUESTA A 0
for(i=0; i<size; i++) // CONTEO INICIAL
   freqs[buffer[i]]++;

for(i=0; i<256; i++)  // SACAR FRECUENCIAS
   if(freqs[i]!=0.0)
      freqs[i]/=size;

for(i=0; i<256; i++)   // CALCULAR MEDIA DE LAS AUTOINFORMACIONES
   if(freqs[i]!=0.0)
      result=result-freqs[i]*log2(freqs[i]); // FORMULA DE SHANNON

return result;
}

// ******************************************************
double entropia_sumatorio(int absfreqs[256], unsigned int size, double *sum)  // NECESITA LAS FRECUENCIAS DE CADA CARACTER ASCII
{
double result=0.0;
int i;

for(i=0; i<256; i++)
         if(absfreqs[i] != 0.0)
                result += absfreqs[i]*log2(absfreqs[i]);

if(sum) *sum=result;
result=log2(size) - result / size;
return result;
}

// ***********************************************************
double entropia(unsigned char *buffer, unsigned int size)
{
int absfreqs[256];
int i;

if(size == 0) return 0.0;

for(i=0; i<256; i++) absfreqs[i]=0;  // PUESTA A 0
for(i=0; i<size; i++) // CONTEO INICIAL
   absfreqs[buffer[i]]++;

return entropia_sumatorio(absfreqs, size, NULL);
}

// *********************************************************

// LAS DOS FUNCIONES ANTERIORES JUNTADAS EN UNA SOLA

double entropia_fast(unsigned char *buffer, unsigned int size)
{
double freqs[256];
double result=0.0;
unsigned int i;

if(size == 0) return 0.0;

//memset(freqs, 0, 256*sizeof(unsigned int));
for(i=0; i<256; i++) freqs[i]=0.0;  // PUESTA A 0

for(i=0; i<size; i++) // CONTEO INICIAL
   freqs[buffer[i]] += 1.0;


for(i=0; i<256; i++)
         if(freqs[i] != 0.0)
                result += freqs[i] * log2(freqs[i]);

result=log2(size) - result / size;

return result;
}
// ******************************************************** //

double * entropia_intervalo(unsigned char * buffer, int leidos, int ENTORNO)
{
int i;
int absfreqs[256];
int ENTORNO_2=0;
unsigned int leidos_2=0;
unsigned int auxchar;
double aux;
double log2_ENTORNO;
double * result;

result=(double *) malloc(sizeof(double) * leidos);
if(result == NULL) return NULL;

ENTORNO_2=ENTORNO / 2;
leidos_2=leidos / 2;
log2_ENTORNO=log2(ENTORNO);

for(i=0; i < ENTORNO_2; i++) result[i]=0.0;
for(i=leidos - ENTORNO_2; i < leidos_2; i++) result[i]=0.0;

// INICIALIZAR VECTOR DE FRECUENCIAS ABSOLUTAS
for(i=0; i < 256; i++) absfreqs[i]=0;
//memset(absfreqs, 0, sizeof(absfreqs)*sizeof(int));

for(i=0; i < ENTORNO; i++)   // ENTRE [0..ENTORNO-1]
  absfreqs[buffer[i]]++;
result[ENTORNO_2]=entropia_sumatorio(absfreqs, ENTORNO, &aux);

for(i=ENTORNO_2 + 1; i < leidos - ENTORNO_2; i++)
{
 #define __FAST__
 #ifdef __FAST__

  auxchar=buffer[i-ENTORNO_2-1];  // ASCII DEL CARACTER A QUITAR / PONER
  aux=aux-absfreqs[auxchar]*log2(absfreqs[auxchar]);
  absfreqs[auxchar]--; // QUITAMOS CARACTER ANTERIOR
  if(absfreqs[auxchar] > 0) aux=aux+absfreqs[auxchar]*log2(absfreqs[auxchar]);

  auxchar=buffer[i+ENTORNO_2-1];
  if(absfreqs[auxchar] > 0) aux=aux-absfreqs[auxchar]*log2(absfreqs[auxchar]);
  absfreqs[auxchar]++;   // PONEMOS EL ACTUAL
  aux=aux+absfreqs[auxchar]*log2(absfreqs[auxchar]);

  result[i]=log2_ENTORNO - aux / ENTORNO;
#else

  result[i]=entropia(buffer+i-ENTORNO_2, ENTORNO);
#endif

}

return result;
}

/************************** FIN DE ENTROPIA.C *********************************/


 

