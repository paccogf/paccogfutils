#include <windows.h>
#include "XInf.h"
#include "dirrun.h"




// SEMI PLUG-INS
#include "sqlstuff.h"
//#include "csvstuff.h"

#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)

ProgArgs gProgArgs = { FALSE, FALSE, NULL, NULL };


void show_help()
{

    printf("Mi Primer Atlas (MPA) v 1e(-inf)\n");
    printf("Sintaxis: ShowPE [opciones] <database> <[unidad:][\\][ruta\\]ficheros (admite comodines)>\n\n");
    printf("Opciones:\n");
    printf("          -r, --recursive   Recorrido recursivo de directorios\n");
    printf("          -n, --nomd5       No calcular MD5 (mas rapido)\n");
    printf("          -h, --help        ¡Socorro!\n");
    SQLiteHelp();
 
return;
}



void GetProgArgs(int argc, char **argv)
{
int i;
char *opt;

    for(i=1; i<argc; i++)
    {

        if(argv[i][0] == '-') // PARAMETROS CON '-'
        {
            opt=argv[i]+1;
            
            if(!strcmp(opt, "r"))
            {
                gProgArgs.bRecursive = TRUE; // "-r" 

            } 
            else if(!strcmp(opt, "-nomd5") || !strcmp(opt, "n"))    // -csv
            {

                gProgArgs.bNoMD5 = TRUE;
            }   
            else if(!strcmp(opt, "-help") || !strcmp(opt, "h"))
            {
                show_help();
                exit(0);
            }
            else
            {
                printf("\nOpcion no reconocida: %s\n\n", argv[i]);
                show_help();
                exit(1);
            }
        }
        else  // NO EMPIEZA CON '-'
        {
            if(gProgArgs.szDBFile == 0)  // ASIGNAR NOMBRES DE FICHERO EN ORDEN
            {
                gProgArgs.szDBFile = argv[i];
            }
            else if(gProgArgs.szPathSpec == 0)
            {
                gProgArgs.szPathSpec = argv[i];
            }
        }

    }


return;
}


char * GetMD5(BYTE *buffer, int size, char *literal)
{
md5_state_t state;
md5_byte_t digest[16];
int di;

      // CALCULO Y ASIGNACION DEL MD5
	md5_init(&state);
	md5_append(&state, (const md5_byte_t *)buffer, size);
	md5_finish(&state, digest);
	for (di = 0; di < 16; ++di)
	    sprintf((char *)(literal) + di * 2, "%02X", digest[di]);

  
return literal;
}


//
// PROGRAMA PRINCIPAL
int main(int argc, char *argv[])
{

GetProgArgs(argc, argv);
if(argc < 3)
{
    show_help();
    return 0;
}


GetProgArgs(argc, argv);

SQLiteProcess();


return 0;
}













