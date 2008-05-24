#include <splitpath.h>


int splitpath(char *path, char *dir, char *file, char *ext)
{
char *aux, *slash, *dot;

if(!path || !*path) return 1;
slash=path-1;
dot=NULL;

// PRIMERA PASADA: SACAR ULTIMO (BACK)SLASH Y PUNTO
for(aux=path; *aux; aux++)
{
   if(*aux == _SLASH_CHARACTER_) slash=aux;
   else if(*aux == '.') dot=aux;
}

// SI NO HAY PUNTO, APUNTAR A CARACTER 0
if(!dot) dot=aux;

// SI NOS INTERESA SABER EL DIRECTORIO
if(dir)
   if(slash>=path)
   {
      strncpy(dir, path, (slash-path)+1);
      dir[slash-path+1]=0;
   }
   else *dir=0;


if(file)// SI NOS INTERESA SABER EL FICHERO
   if(slash>=path)
   {
      strncpy(file, slash+1, (dot-slash)-1);
      file[dot-slash-1]=0;
   }
   else 
   {
      strncpy(file, path, dot-path);
      
   }
//   *file=0;

if(ext)
   if(dot<aux) // APUNTARA A EL ULTIMO '.' O AL NULL DE path
   {
      strncpy(ext, dot+1, aux-dot);
      ext[aux-dot]=0;
   }
   else *ext=0;

return 0;   
}


/*

int main(int argc, char *argv[])
{
char path[100], name[100], ext[50];  

if(argc!=2) return 1;
  
splitpath(argv[1], 0, 0, ext);

printf("Nombre completo: %s\n", argv[1]);
//printf("Nombre directorio: %s\n", path);
//printf("Nombre fichero: %s\n", name);
printf("extension fichero: %s\n", ext);

system("PAUSE");
return 0;
}
*/
