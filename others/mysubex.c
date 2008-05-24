#include "mysubex.h"

#ifdef __cplusplus
#error mysubex.c must be compiled in C!
#endif

// VBLES GENERALES
st_operator * opvector=NULL;
t_f_atom_evaluator atomevaluator=NULL;


// DEVUELVE EL OPERADOR ASOCIADO A UN CARACTER O SIMBOLO
st_operator * _get_operator_from(char op, int n_operands)
{
st_operator *p;
for(p=&opvector[0]; p->symbol!=0; p++)
{ 
  if((p->symbol==op) && (p->operands==n_operands)) return p;
}
return NULL;
}


// CONVIERTE LA CADENA APUNTADA A UN INT A TRAVES DE atomevaluator
int _subex_evaluate_atom(char str[], int posinit, int posfin)
{
int ret;
char save;
//printf("evaluando %s, posinit=%d, posfin=%d\n", str, posinit, posfin);
save=str[posfin+1];
str[posfin+1]=0;
ret=atomevaluator(str+posinit);
str[posfin+1]=save;

return ret;   
}


// FUNCION RECURSIVA ORIGINAL
int _subex(char str[], int posinit, int posfin)
{
int inpar=0;
int i;
int savepos=-1;
st_operator * poperator=NULL;
st_operator * saveoperator=NULL;

assert(posinit<=posfin);

// SI YA HAY OPERADOR UNARIO AL PPIO
saveoperator=_get_operator_from(str[posinit], 1);
if(saveoperator)  // ALMACENAR SU POSICION
{
   savepos=posinit;
   i=posinit+1;
}
else
   i=posinit;

// BUSCAR EL OPERADOR BINARIO DE MENOR PRECEDENCIA A ESTE NIVEL
for(i; i<=posfin; i++) // BUCLE PRINCIPAL
{
   if(str[i]=='(') inpar++;  // PASAR DE TODO LO QUE VAYA ENTRE PARENTESIS
   if(str[i]==')') inpar--;
   if(inpar) continue;
   
   poperator=_get_operator_from(str[i], 2); // COMPROBAR SI CARACTER ES OPERADOR BINARIO
   if(poperator==NULL) continue; // EL CARACTER NO CORRESPONDIA A UN OPERADOR
  // if(i==savepos+1) continue;  // ESTO ES POR SI HAY ALGO ASI COMO 3+-4
   if(saveoperator==NULL) saveoperator=poperator;  // INICIALIZACION DE saveoperator
   if(poperator->precedence <= saveoperator->precedence) 
   {
      saveoperator=poperator;
      savepos=i;
   }
} // FOR

// EVALUAR OPERADOR ENCONTRADO, TIENE PREFERENCIA LOS BINARIOS PARA EL MISMO NIVEL
if(saveoperator!=NULL)
{
   if(saveoperator->operands == 2)
      return saveoperator->func(_subex(str, posinit, savepos-1), _subex(str, savepos+1, posfin));  
   if(saveoperator->operands == 1)
      return saveoperator->func(_subex(str, posinit+1, posfin));  
   assert(0);   
}

// SI NO ENCONTRAMOS OPERADOR ALGUNO EN ESTE NIVEL
if((str[posinit]=='(') && (str[posfin]==')'))  // PERO HAY ALGO ENTRE PARENTESIS
   return _subex(str, posinit+1, posfin-1); // QUITARSELOS Y EVALUAR

// EN ESTE NIVEL SOLO QUEDA UN VALOR NUMERICO
return _subex_evaluate_atom(str, posinit, posfin);
}



int subex(char str[], st_operator opv[], t_f_atom_evaluator f)
{

opvector=opv;
atomevaluator=f;
if(atomevaluator==NULL) atomevaluator=atoi;
return _subex(str, 0, strlen(str)-1);
}

