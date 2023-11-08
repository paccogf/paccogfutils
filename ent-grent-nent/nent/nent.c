#include <stdio.h>
#include <windows.h>
#include "entropia.h"

int fgetsize(FILE *fs)
{
int currpos, retval;

currpos=ftell(fs);    
fseek(fs, 0, SEEK_END);
retval=ftell(fs);
fseek(fs, currpos, SEEK_SET);
return retval;
    
}



/************************* ENTROPIA.C *********************************/
/*

double autoinformation(int absfreqs[256], unsigned int size, double *sum)  // NECESITA LAS FRECUENCIAS DE CADA CARACTER ASCII
{
double result=0.0;
int i;

for(i=0; i<256; i++)
         if(absfreqs[i] != 0)
                result += absfreqs[i] * log2(absfreqs[i]);

if(sum != NULL) *sum=result;
result=log2(size) - result / size;

return result;
}

// ******************************************************** //

double entropia(unsigned char *buffer, unsigned int size)
{
int absfreqs[256];
int i;

if(size == 0) return 0.0;
//memset(freqs, 0, 256*sizeof(unsigned int));
for(i=0; i<256; i++) absfreqs[i]=0;  // PUESTA A 0
for(i=0; i<size; i++) // CONTEO INICIAL
   absfreqs[buffer[i]]++;

return autoinformation(absfreqs, size, NULL);
}

// ******************************************************** //

double * inter_entropy(BYTE * buffer, int leidos, int ENTORNO)
{
int i;    
int absfreqs[256];
int ENTORNO_2=0;
unsigned int leidos_2=0;
unsigned int auxchar;
double aux;
double log2_ENTORNO;
double * result;

result=malloc(sizeof(double) * leidos);
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
result[ENTORNO_2]=autoinformation(absfreqs, ENTORNO, &aux);

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

*/
//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
BYTE *buffer;
int buffersize, i, over7;
FILE *fs;
double *results;


fs=fopen(argv[1], "rb");
if(fs == 0) // DEBERIA SER UNA EXCEPCION!!!
    return MessageBox(0, "Error al abrir fichero", 0, MB_ICONERROR);
    
buffersize=fgetsize(fs);
if(buffersize == 0) 
        return MessageBox(0, "El tamaño del fichero no puede ser 0", 0, MB_ICONERROR);
    
buffer=malloc(sizeof(BYTE) * buffersize);  // BUFFER PARA EL FICHERO
if(buffer == NULL)
   return MessageBox(0, "Error al reservar memoria", 0, MB_ICONERROR);

if(fread(buffer, sizeof(BYTE), buffersize, fs) < buffersize)
{
   return MessageBox(0, "Error al leer de fichero", 0, MB_ICONERROR);
}


results=entropia_intervalo(buffer, buffersize, 512);    

over7=0;
for(i=0; i<buffersize; i++) if(results[i] > 7.0) over7++;
printf("Resultado %lf%% comprimido\n", 100 * ((double)over7 / (double)buffersize));
//for(i=0; i<buffersize; i++) printf("offset: %d - resultado: %lf\n", i, results[i]);
printf("Entropia total: %lf\n", entropia_fast(buffer, buffersize));

    
free(results);
free(buffer);
system("pause");
return 0;    
}
//---------------------------------------------------------------------------
