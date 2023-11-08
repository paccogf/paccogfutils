//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop


#pragma package(smart_init)

#include "Main.h"

//---------------------------------------------------------------------------

//   Important: Methods and properties of objects in VCL can only be
//   used in a method called using Synchronize, for example:
//
//      Synchronize(UpdateCaption);
//
//   where UpdateCaption could look like:
//
//      void __fastcall TWorkThread::UpdateCaption()
//      {
//        MainForm->Caption = "Updated in a thread";
//      }
//---------------------------------------------------------------------------



/************************* ENTROPIA.C *********************************/

double entropia_fast(unsigned char *buffer, unsigned int size)
{
int freqs[256];
double result=0.0;
unsigned int i;


if(size == 0) return 0.0;

//memset(freqs, 0, 256*sizeof(unsigned int));
for(i=0; i<256; i++) freqs[i]=0;  // PUESTA A 0

for(i=0; i<size; i++) // CONTEO INICIAL
   freqs[buffer[i]]++;


for(i=0; i<256; i++)
         if(freqs[i] != 0)
                result += freqs[i] * log2(freqs[i]);

result=log2(size) - result / size;

return result;
}

/////////////////////////////////

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


/******************************************************************************/



__fastcall TWorkThread::TWorkThread(AnsiString filename, int entorno, int first_n_bytes, TProgressCounter *ProgCounter)
        : FileName(filename), ENTORNO(entorno), result(0), buffer(0), maxfilesize(first_n_bytes),
          ProgressCounter(ProgCounter), TThread(true)
{
}

//---------------------------------------------------------------------------

__fastcall TWorkThread::~TWorkThread()
{
if(fs) delete fs;
fs=0;
if(buffer) delete [] buffer;
buffer=0;
if(result) delete [] result;
result=0;
}


//---------------------------------------------------------------------------
void __fastcall TWorkThread::Execute()
{
leidos=0;
if(result!=0) delete [] result;
result=0;
int absfreqs[256];
int ENTORNO_2=0;
unsigned int leidos_2=0;
double aux;
double log2_ENTORNO;
unsigned int auxchar;

fs=new TFileStream(FileName, fmOpenRead | fmShareDenyWrite);
if(fs != 0) // DEBERIA SER UNA EXCEPCION!!!
if(fs->Size > ENTORNO)
{
    if(maxfilesize == -1)    // SI QUEREMOS MIRAR TODO EL FICHERO...
    {
        maxfilesize=fs->Size;
    }
    else
    {
        maxfilesize=min(maxfilesize, fs->Size);
    }

    buffer=new BYTE[maxfilesize];
    if(buffer != 0)   // DEBERIA SER UNA EXCEPCION!!!;
    {
         leidos=fs->Read(buffer, maxfilesize);

         if(leidos == maxfilesize)
         {
              // INICIALIZAR VECTOR RESULTADOS
              if(result != 0) delete [] result;
              result=new double [leidos];
              ENTORNO_2=ENTORNO / 2;
              leidos_2=leidos / 2;
              log2_ENTORNO=log2(ENTORNO);

              for(currentIndex=0; currentIndex < ENTORNO_2; currentIndex++) result[currentIndex]=0.0;
              for(currentIndex=leidos - ENTORNO_2; currentIndex < leidos; currentIndex++) result[currentIndex]=0.0;

              // INICIALIZAR VECTOR DE FRECUENCIAS ABSOLUTAS
              for(currentIndex=0; currentIndex < 256; currentIndex++) absfreqs[currentIndex]=0;
              //memset(absfreqs, 0, sizeof(absfreqs)*sizeof(int));

              for(currentIndex=0; currentIndex < ENTORNO; currentIndex++)   // ENTRE [0..ENTORNO-1]
                  absfreqs[buffer[currentIndex]]++;
              result[ENTORNO_2]=autoinformation(absfreqs, ENTORNO, &aux);

/*BUCLE PPAL*/for(currentIndex=ENTORNO_2 + 1; currentIndex < leidos - ENTORNO_2; currentIndex++)
              {
                  if(Terminated) break;

                  auxchar=buffer[currentIndex-ENTORNO_2-1];  // ASCII DEL CARACTER A QUITAR / PONER
                  aux=aux-absfreqs[auxchar]*log2(absfreqs[auxchar]);
                  absfreqs[auxchar]--; // QUITAMOS CARACTER ANTERIOR
                  if(absfreqs[auxchar] > 0) aux=aux+absfreqs[auxchar]*log2(absfreqs[auxchar]);

                  auxchar=buffer[currentIndex+ENTORNO_2-1];
                  if(absfreqs[auxchar] > 0) aux=aux-absfreqs[auxchar]*log2(absfreqs[auxchar]);
                  absfreqs[auxchar]++;   // PONEMOS EL ACTUAL
                  aux=aux+absfreqs[auxchar]*log2(absfreqs[auxchar]);

                  result[currentIndex]=log2_ENTORNO - aux / ENTORNO;

               /*   if(currentIndex == VALOR_CALCULADO_EN_FUNCION_DE_TAMANO)
                      result[currentIndex]=autoinformation(absfreqs, ENTORNO, NULL);*/

                  if(currentIndex == 0) continue;
                  if((currentIndex % (leidos / 10)) != 0) continue;
                  Synchronize(SendProgress);
              }
              currentIndex=leidos;
              Synchronize(SendProgress); // PARA QUE ACABE DE LLENAR LA BARRA
         }
         else // if(leidos == maxfilesize)
         {
              MessageBox(0, "Error al leer de fichero", 0, MB_ICONERROR);

         }
         delete [] buffer;     // LIBERAR MEMORIA
         buffer=0;
    }
    else // if(buffer != 0)
    {
        MessageBox(0, "Error al reservar memoria", 0, MB_ICONERROR);
    }
    delete fs; // MEJOR LIBERAR (CERRAR) EL FICHERO AQUI, SI NO EL ACCESO ES EXCLUSIVO
    fs=0;
}
else // if(fs != 0)
{
    MessageBox(0, "Tamaño del entorno mayor que el del fichero", 0, MB_ICONERROR);
    leidos-1;
}

Synchronize(FinalizeProgress);
}
//---------------------------------------------------------------------------
void __fastcall TWorkThread::SendProgress()            // MANDAR PROGRESO ENTRE 0 Y 100
{
ProgressCounter->SetProgress(100 * currentIndex / leidos);
}

// ---------------------------------------------------------------------

void __fastcall TWorkThread::FinalizeProgress()
{
ProgressCounter->ErrorCode=leidos;
ProgressCounter->EndProgress();
}

//---------------------------------------------------------------------------
