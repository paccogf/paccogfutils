#include "mysubexxx.h"


int SubExpression::_evaluate(const string & expression, const string::const_iterator & from, const string::const_iterator & to) const
{
int inpar=0;
string::const_iterator savepos, pos;
Operator * poperator=NULL;
Operator * saveoperator=NULL;

assert(from < to); // ESTO NO DEBERIA SUCEDER

// SI YA HAY OPERADOR UNARIO AL PPIO
saveoperator=_get_operator_from(*from, 1);
   
if(saveoperator)  // ALMACENAR SU POSICION
{
   savepos=from;
   pos=from+1;
}
else
   pos=from;

// BUSCAR EL OPERADOR BINARIO DE MENOR PRECEDENCIA A ESTE NIVEL
for(; pos!=to; pos++) // BUCLE PRINCIPAL
{
   if(*pos == '(') inpar++;  // PASAR DE TODO LO QUE VAYA ENTRE PARENTESIS
   if(*pos == ')') inpar--;
   if(inpar) continue;

   poperator=_get_operator_from(*pos, 2); // COMPROBAR SI CARACTER ES OPERADOR BINARIO
   if(poperator == NULL) continue; // EL CARACTER NO CORRESPONDIA A UN OPERADOR
  // if(i==savepos+1) continue;  // ESTO ES POR SI HAY ALGO ASI COMO 3+-4
   if(saveoperator == NULL) saveoperator=poperator;  // INICIALIZACION DE saveoperator
   if(poperator->precedence <= saveoperator->precedence)
   {
      saveoperator=poperator;
      savepos=pos;
   }
} // FOR


// EVALUAR OPERADOR ENCONTRADO, TIENE PREFERENCIA LOS BINARIOS PARA EL MISMO NIVEL
if(saveoperator!=NULL)
{
   if(saveoperator->noperands == 2)
      return saveoperator->theFunction(_evaluate(expression, from, savepos), _evaluate(expression, savepos+1, to));
   if(saveoperator->noperands == 1)
      return saveoperator->theFunction(_evaluate(expression, from+1, to));
   assert(0); // SI LLEGAMOS AQUI LA HEMOS CAGAO
}

// SI NO ENCONTRAMOS OPERADOR ALGUNO EN ESTE NIVEL
if((*from == '(') && (*(to-1) == ')') )  // PERO HAY ALGO ENTRE PARENTESIS
   return _evaluate(expression, from+1, to-1); // QUITARSELOS Y EVALUAR

// EN ESTE NIVEL SOLO QUEDA UN VALOR NUMERICO
return _evaluate_atom(expression, from, to);
}

// DEVUELVE EL OPERADOR ASOCIADO A UN CARACTER O SIMBOLO
SubExpression::Operator * SubExpression::_get_operator_from(char op, int n_operands) const
{

vector<Operator>::const_iterator itr;
for(itr=OpVector.begin(); itr!=OpVector.end(); itr++)
{ 
   if((itr->symbol==op) && (itr->noperands==n_operands))
   {
      return (Operator *)(&(*itr));
   }
}
return NULL;
}

// CONVIERTE LA CADENA APUNTADA A UN INT A TRAVES DE atomevaluator
int SubExpression::_evaluate_atom(const string & expression, const string::const_iterator & from, const string::const_iterator & to) const
{
if(theFunction==NULL)
{
   int i;
   istringstream(string(from, to)) >> i;  // MAS FASHION
   return i;
   //return atoi(string(from, to).c_str()); // MAS RAPIDO
}
else
{
   return theFunction(string(from, to));
}
}
