#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "entropia.h"
#include "processdir.h"

//#define NOGLOB
/*
#ifdef NOGLOB
   int _CRT_glob = 0;
#else
   int _CRT_glob = 1;
#endif
*/

#define perror_exit(s)\
        { int e=errno;\
          perror(s);\
          exit(e); }

#define printf_exit(s...) 	{ printf(s); exit(-1); }
#define PACKED_LIMIT 7.0   // CON 7.5 PUEDE QUE ESTE MEJOR...
#define SIZE_LIMIT 0.3

#define MAX(x, y)  (x>y?x:y)  // POR SI ACASO

/////////////////////////////////////////////////////////////////////
//
int fsize(FILE *f)
{
int oldpos;
int size;

oldpos=ftell(f);
fseek(f, 0, SEEK_END);
size=ftell(f);
fseek(f, oldpos, SEEK_SET);

return size;
}

/////////////////////////////////////////////////////////////////////
//
int ReadPEHeader(FILE *f, IMAGE_NT_HEADERS *PEheader)
{
int offset;
fseek(f, 0x3C, SEEK_SET);  // PONERNOS SOBRE EL CAMPO IMAGE_DOS_HEADER.e_lfanew
fread(&offset, sizeof(int), 1, f);

fseek(f, offset, SEEK_SET);                  // LEER CABECERA PE
fread(PEheader, sizeof(IMAGE_NT_HEADERS), 1, f);
if(PEheader->Signature != IMAGE_NT_SIGNATURE) return 0;

return offset;
}

/////////////////////////////////////////////////////////////////////
//
IMAGE_SECTION_HEADER *ReadSectionTable(FILE *f, int *nsections) //IMAGE_NT_HEADERS *PEheader)
{
IMAGE_SECTION_HEADER *sectable;
IMAGE_NT_HEADERS  PEheader;
int temp;

temp=ReadPEHeader(f, &PEheader);   // SACAR CABECERA PE
if(temp == 0) return NULL;

*nsections=PEheader.FileHeader.NumberOfSections;  // SACAR Nº SECCIONES Y RESERVAR ESPACIO
sectable=(IMAGE_SECTION_HEADER *) malloc(*nsections * sizeof(IMAGE_SECTION_HEADER));

// LEER SECCIONES
fseek(f, temp+sizeof(DWORD)+sizeof(IMAGE_FILE_HEADER)+PEheader.FileHeader.SizeOfOptionalHeader, SEEK_SET);
temp=fread(sectable, sizeof(IMAGE_SECTION_HEADER), *nsections, f);
if(temp < *nsections) return NULL;

return sectable;
}

/////////////////////////////////////////////////////////////////////
//
BYTE *ReadSection(FILE *f, IMAGE_SECTION_HEADER *section, int *nRead)
//BYTE *ReadSection(FILE *f, DWORD offset, DWORD size)
{
BYTE *sectbuf;

sectbuf=malloc(section->SizeOfRawData);
if(sectbuf == NULL) return NULL;  // CASO QUE NO SE RESERVE MEMORIA BIEN

fseek(f, section->PointerToRawData, SEEK_SET);
*nRead=fread(sectbuf, sizeof(BYTE), section->SizeOfRawData, f);

return sectbuf;
}

/////////////////////////////////////////////////////////////////////
//
DWORD print_help(void)
{
printf("Sintaxis: ent [-nopause] <fichero ejecutable>\n");
//printf("Si se especifica -nopause no se hace pausa al terminar\n");
return 0;
}

/////////////////////////////////////////////////////////////////////
//
int main(int argc, char *argv[])
{
FILE *f;
int filesize, sectionsize, i, nsections, ovlsize, imagesize, aux;
IMAGE_SECTION_HEADER *section;
BYTE *sectionbuffer = 0;
double sectionentropy;
double maxsectionentropy;
double entropy;
char *szfile, **szpattern;

BOOL nopause=FALSE;
BOOL bigpackedsection;
BOOL bigpackedoverlay;
BOOL packedsection;


if(argc < 2) return print_help();  // COMPROBACION ARGUMENTOS
if(!strcmp(argv[1], "-nopause"))
{
   if(argc < 3) return print_help();
   szpattern=argv+1;
   nopause=TRUE;
}
else szpattern=argv;




// PROCESSDIR_BEGIN(szpattern, fdData) // BUCLE PRINCIPAL - PROCESO FICHROS
while((++szpattern) < argv+argc)
{
    szfile=*szpattern;
    bigpackedsection=FALSE;
    bigpackedoverlay=FALSE;
    packedsection=FALSE;
    
    f = fopen(szfile, "rb");  // ABRIR FICHERO 
    // if(f == NULL) perror_exit(szfile);
    if(f == NULL) 
    {
        perror(szfile);
        if(!nopause) system("pause");
        continue;  // PUEDE SER UN DIRECTORIO
    }
    filesize = fsize(f);
    
    printf("\n\n");
    printf("Fichero: %s\n", szfile);
    printf("==============================================================\n");
    printf("N  Seccion\tOffset\t\tTamaño\t\tEntropia\n");
    printf("==============================================================\n");
    imagesize=-1;
    // LEER TABLA DE SECCIONES
    section = ReadSectionTable(f, &nsections);
    if(section) // SI HAY TABLA DE SECCIONES Y ES UN PE
    {
        // CALCULO DE LA ENTROPIA POR SECCION

        aux=-1;
        for(i = 0; i < nsections; i++)
        {
           sectionbuffer = ReadSection(f, &section[i], &sectionsize);
           sectionentropy = entropia_fast(sectionbuffer, sectionsize);
           imagesize=MAX(imagesize, section[i].PointerToRawData + section[i].SizeOfRawData);
           printf("%02d %8s\t%08lX\t%08lX\t%lf", i, section[i].Name, section[i].PointerToRawData, section[i].SizeOfRawData, sectionentropy);
           if(sectionentropy >= PACKED_LIMIT) 
           {
               packedsection=TRUE;
               printf(" !");
               if((section[i].SizeOfRawData > section[aux].SizeOfRawData) || (aux==-1))
               {
                    aux=i;  // GUARDAMOS SECCIÓN CON DICHO MÁXIMO
               }               
           }
           free(sectionbuffer);
           printf("\n");
        }
        //printf("\nTamano imagen: %d\n", imagesize);

        // aux ES LA SECCION EMPAQUETADA MAS GRANDE
        if(packedsection)
        {
            ///printf("Tamano de seccion empaquetada más grande: %ld bytes (seccion %d)\n\n", section[aux].SizeOfRawData, aux);
            entropy=(double) section[aux].SizeOfRawData / (double) imagesize;
            if(entropy > SIZE_LIMIT)
            {
                bigpackedsection = TRUE;
            }
        }
      
        // CALCULO ENTROPIA OVERLAY        
        if(imagesize < filesize)
        {
            ovlsize=filesize-imagesize;
            fseek(f, imagesize, SEEK_SET);
            sectionbuffer=malloc(ovlsize*sizeof(BYTE));
            fread(sectionbuffer, sizeof(BYTE), ovlsize, f);
            entropy=entropia_fast(sectionbuffer, ovlsize);
            printf("\n*  %8s\t%08X\t%08X\t%lf", "overlay" , imagesize, ovlsize, entropy);
            if(entropy >= PACKED_LIMIT) 
            {
                printf(" !");
                entropy=(double) ovlsize / (double) filesize;
                if(entropy > SIZE_LIMIT)
                {
                   bigpackedoverlay=TRUE;
                   printf("!");
                }
            }
    
            printf("\n");
            free(sectionbuffer);
        }
      
        free(section);
    }
    else  // TABLA DE SECCIONES
    {
        printf("\nNo se pudo leer la tabla de secciones\n");
    }
    
    // CALCULO DE TODO EL FICHERO
    sectionbuffer=malloc(filesize);
    if(sectionbuffer)
    {
        nsections=0; // LO VAMOS A USAR PARA CALCULAR LA RELACION DE > 7.0
        fseek(f, 0, SEEK_SET);
        fread(sectionbuffer, sizeof(BYTE), filesize, f);
        entropy=entropia_fast(sectionbuffer, filesize);
        printf("\n*  %8s\t%08X\t%08X\t%lf", "Fichero" , 0, filesize, entropy);
        if(entropy >= PACKED_LIMIT) printf(" (¿comprimido?)");  
        printf("\n\n");
        free(sectionbuffer);
    }
    
    fclose(f);
    
    printf("Estimacion final: ");
    //if(packedcode || (packedsection && (entropy >= PACKED_LIMIT))) 
    if(bigpackedsection)
       printf("Empaquetado");
    else 
       printf("No empaquetado");
    if(bigpackedoverlay)
       printf(" - Instalador");
    
    printf("\n\n");
    
    if(!nopause) system("PAUSE");

}//PROCESSDIR_END

return 0;
}
