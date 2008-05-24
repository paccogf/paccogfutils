#include <myparser.h>


/////////////////////////////////////////////////////
// DEVUELVE EL INDICE DE UN PARAMETRO [1..NPARAMS]
// 0 SI NO SE ENCONTRO
int paramindex(char *s, int argc, char *argv[])
{
int i;
  for(i=1; i<argc; i++) // RECORRER LINEA DE COMANDO
    if(!strcmp(s, argv[i])) 
      return i;
    
return 0;     
}

int argindex(int index, int argc, char *argv[]) // DEVUELVE INDICE DENTRO DE ARGV DEL PARAMETRO index
{                                              // 0 SI NO LO ENCUENTRA
int i;
if(index<1) return 0;

for(i=0; i<argc; i++)
{
  if(argv[i][0]!='-') index--;
  if(index==0) return i;
}

return 0;
}
