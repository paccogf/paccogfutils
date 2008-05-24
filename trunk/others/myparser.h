#ifndef _MYPARSER_H_
#define _MYPARSER_H_
#include <stdio.h>


// PARA paramindex()
#define PARAMINDEX(s) paramindex(s, argc, argv)

#define ARGNUM argnum(argc, argv)
#define FIRSTARGINDEX argindex(0, argc, argv)
#define NEXTARGINDEX(i) argindex(i, argc, argv)


struct parameters; // DECLARACION FORWARD (DECLARACION DE INTENCIONES)


// FUNCION AUN MAS SIMPLE
int paramindex(char *s, int argc, char *argv[]);
int argnum(int argc, char *argv[]);              // Nº DE ARGUMENTOS SIN '-'
int argindex(int start, int argc, char *argv[]); // INDICE DEL SGUTE ARGUMENTO SIN '-'



#endif
