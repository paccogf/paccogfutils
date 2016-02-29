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
result=log2(size) – result / size;
return result;
}
 
/**************************************************************/
double autoinformation(int absfreqs[256], unsigned int size, double *sum)  // NECESITA LAS FRECUENCIAS DE CADA CARACTER ASCII
{
double result=0.0;
int i;
for(i=0; i<256; i++)
         if(absfreqs[i] != 0)
                result += absfreqs[i] * log2(absfreqs[i]);
if(sum != NULL) *sum=result;
result=log2(size) – result / size;
return result;
}
 
//
double entropia(unsigned char *buffer, unsigned int size)
{
int absfreqs[256];
int i;
if(size == 0) return 0.0;
for(i=0; i<256; i++) absfreqs[i]=0;  // PUESTA A 0
for(i=0; i<size; i++) // CONTEO INICIAL
   absfreqs[buffer[i]]++;
return autoinformation(absfreqs, size, NULL);
}