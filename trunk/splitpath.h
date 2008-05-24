#ifndef _SPLITPATH_H_
#define _SPLITPATH_H_

#include <string.h>

#ifndef WIN32                 // COMPILAMOS EN LINUX
#define _SLASH_CHARACTER_ '/'
#else                         // COMPILAMOS BAJO WINDOWS
#define _SLASH_CHARACTER_ '\\'
#endif

// NECESITA: EL PATH COMPLETO QUE TRITURAR APUNTADO POR path
// DEVUELVE: EL PATH SEPARADO EN DIRECTORIO, FICHERO Y EXTENSION
// COMENTARIO: dir, file Y ext PUEDEN SER NULL SI NO QUEREMOS OBTENERLOS


int splitpath(char *path, char *dir, char *file, char *ext);


#endif
