#ifndef _MYSUBEX_H_
#define _MYSUBEX_H_

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#ifdef __cplusplus
extern "C" {
typedef int (*t_f_operator)(...);
#else
typedef int (*t_f_operator)();
#endif


typedef int (*t_f_atom_evaluator)(const char *);

typedef struct 
{
char symbol;
char operands;
char precedence; // MAYOR VALOR NUMERICO -> SE EVALUA ANTES
t_f_operator func;
} st_operator;

int subex(char str[], st_operator opv[], t_f_atom_evaluator f);

#ifdef __cplusplus
}
#endif
                       
#endif

/* ESQUEMA DE DESCOMPOSICION:
   
//   3+4 = +        (3+4)= 3+4     -1+3 = +           3+-1 = 
//        / \                            / \
//       3   4                          -   3
//                                      |
//                                      1


// PROGRAMA EJEMPLO

#include <stdio.h>
#include <string.h>
#include "mysubex.h"

int suma(int x, int y)
{
return x+y;
}

int resta(int x, int y)
{
return x-y;
}

int multiplicacion(int x, int y)
{
return x*y;
}

int division(int x, int y)
{
return x/y;   
}

int menos(int x)
{
return -x;
}

st_operator opv[]={  {'+', 2, 1, suma},
                     {'-', 2, 1, resta},
                     {'*', 2, 2, multiplicacion},
                     {'/', 2, 2, division},
                     {'-', 1, 3, menos}, // OPERADORES UNARIOS
                     {0, 0, 0, NULL}
                  };


int main(int argc, char *argv[])
{
  char str[1024];
  gets(str);
  

  printf("%s=%d\n", str, subex(str, opv, atoi));
system("pause");  
  return 0;
}
*/
