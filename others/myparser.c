#include <myparser.h>
////////////////////////////////////////////////////
// PARAMETRO: CON -
// ARGUMENTO: SIN -



/////////////////////////////////////////////////////
// DEVUELVE EL INDICE DE UN PARAMETRO [1..NPARAMS]
// 0 SI NO SE ENCONTRO
int paramindex(char *s, int argc, char *argv[])
{
int i;
for(i=1; i<argc; i++) // RECORRER LINEA DE COMANDO
  if(!strcmp(s, argv[i])) return i;
    
return 0;     
}

// RETORNA Nº DE ARGUMENTOS SIN '-'

int argnum(int argc, char *argv[])
{
int i, aux;
aux=0;
for(i=1; i<argc; i++)
   if(argv[i][0]!='-') aux++;
      
return aux;      
}


// DEVUELVE EL INDICE DENTRO DE argv[] DEL SIGUIENTE PARAMETRO ENCONTRADO A PARTIR DE start
// start ES OTRO INDICE DENTRO DE argv[]
// PARA ENCONTRAR EL PRIMER ARGUMENTO SERIA 
// index=argindex(0, argc, argv);
// PARA LOS SIGUIENTES
// index=argindex(index, argc, argv);

int argindex(int start, int argc, char *argv[])
{
int i;

if((start>=argc) || (start<0)) return -1; // ERROR
for(i=(start+1); i<argc; i++) if(argv[i][0]!='-') return i;
   
return 0;  // NO ENCONTRADO   
}

