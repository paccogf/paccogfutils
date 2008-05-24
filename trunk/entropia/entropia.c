#include "entropia.h"


double entropia(unsigned char *buffer, unsigned int size)
{
double freqs[256];
double result=0.0;
unsigned int i;

//memset(freqs, 0, 256*sizeof(double));
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
