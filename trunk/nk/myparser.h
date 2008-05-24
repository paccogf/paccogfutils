#ifndef _MYPARSER_H_
#define _MYPARSER_H_
#include <stdio.h>


// PARA paramindex()
#define PARAMINDEX(s) (paramindex(s, argc, argv))
#define ARGINDEX(i) (argindex(i, argc, argv))
#define PARAMINDEXREV(s) (argc-PARAMINDEX(s))
#define NPARAMS (argc-1)


struct parameters; // DECLARACION FORWARD (DECLARACION DE INTENCIONES)


// FUNCION AUN MAS SIMPLE
int paramindex(char *s, int argc, char *argv[]);
int argindex(int index, int argc, char *argv[]); // INDICE DEL PRIMER ARGUMENTO SIN '-'



#endif
