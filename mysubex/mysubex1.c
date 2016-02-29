
#ifndef _MYSUBEX_H_
#define _MYSUBEX_H_
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;
template<class T>
class SubExpression
{
public:
    class Operator
    {
    friend class SubExpression;
    public:
    Operator() {}
    Operator(char sym, char nops, char prec): symbol(sym),
                                              noperands(nops),
                                              precedence(prec) {}
    virtual T Evaluate(const T &left, const T & right) {return left;}
    virtual T Evaluate(const T &left) { return left; }
    protected:
    char     symbol;
    int      noperands;
    char     precedence; // MAYOR VALOR NUMERICO -> SE EVALUA ANTES
    };
 
    class Operand
    {
    friend class SubExpression;
    public:
    virtual T Evaluate(const string & operand) { T i;
                                                istringstream(operand) >> i;  // MAS FASHION
                                                return i;
                                                /*return atoi(i.c_str());*/}
    };
public:
// CONSTANTE DE CLASE
static const Operand defaultOperand;
// CONSTRUCTORES
SubExpression(Operand * a=(Operand *)&defaultOperand): theOperand(a) {}
          
// OPERACIONES BASICAS
bool        AddOperator       (Operator * op) { OpVector.push_back(op); return true; }
bool        SetOperand        (Operand * a) { theOperand=a; return true;}
// METODO PRINCIPAL
// TO DO: QUE DEVUELVA <class T>
T           Evaluate          (const string & expression) const { return (OpVector.size() > 0)?_evaluate(expression, expression.begin(), expression.end()): T(0); }
protected:
// TO DO: QUE DEVUELVA <class T>
T          _evaluate         (const string & expression, const string::const_iterator & from, const string::const_iterator & to) const;
Operator * _get_operator_from (char op, int n_operands) const;
// DATOS
vector<Operator *>  OpVector;
Operand             *theOperand;
};
// CONSTANTE ESTATICA, TRATAMIENTO DE LOS OPERANDOS POR DEFECTO
template<class T>
const typename SubExpression<T>::Operand SubExpression<T>::defaultOperand;
 
// DEVUELVE EL OPERADOR ASOCIADO A UN CARACTER O SIMBOLO
template<class T>
typename SubExpression<T>::Operator * SubExpression<T>::_get_operator_from(char op, int n_operands) const
{
typedef typename vector<Operator *>::const_iterator iterator;
iterator itr;
for(itr=OpVector.begin(); itr!=OpVector.end(); itr++)
{
   if(((*itr)->symbol==op) && ((*itr)->noperands==n_operands))
   {
      return (*itr);
   }
}
return NULL;
}
 
template<class T>
T SubExpression<T>::_evaluate(const string & expression, const string::const_iterator & from, const string::const_iterator & to) const
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
   if(*pos == ‘(‘) inpar++;  // PASAR DE TODO LO QUE VAYA ENTRE PARENTESIS
   if(*pos == ‘)’) inpar–;
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
      return saveoperator->Evaluate(_evaluate(expression, from, savepos), _evaluate(expression, savepos+1, to));
   if(saveoperator->noperands == 1)
      return saveoperator->Evaluate(_evaluate(expression, from+1, to));
   assert(0); // SI LLEGAMOS AQUI LA HEMOS CAGAO
}
// SI NO ENCONTRAMOS OPERADOR ALGUNO EN ESTE NIVEL
if((*from == ‘(‘) && (*(to-1) == ‘)’) )  // PERO HAY ALGO ENTRE PARENTESIS
   return _evaluate(expression, from+1, to-1); // QUITARSELOS Y EVALUAR
// EN ESTE NIVEL SOLO QUEDA UN VALOR NUMERICO
return theOperand->Evaluate(string(from, to));
}
#endif
/*************** USO *****************************/
/*
template <class T>
class Suma: public SubExpression<T>::Operator
{
public:
   Suma(): SubExpression<T>::Operator::Operator(‘+’, 2, 1) { }
   virtual T Evaluate(const T &left, const T & right) { return left+right; }
   //virtual T Evaluate(const T &left) { assert(false); return left; }
};
template <class T>
class Multiplicacion: public SubExpression<T>::Operator
{
public:
   Multiplicacion(): SubExpression<T>::Operator(‘*’, 2, 2) { }
   virtual T Evaluate(const T &left, const T & right) { return left*right; }
   //virtual T Evaluate(const T &left) { assert(false); return left; }
};
int main(int argc, char *argv[])
{
SubExpression<float> sub1;
Suma<float> su;
Multiplicacion<float> mu;
sub1.AddOperator(&su);
sub1.AddOperator(&mu);
cout << sub1.Evaluate("2*2.4") << endl;  // SALE 4.8
system("pause");
return 0;
}
 
*/
/* ESQUEMA DE DESCOMPOSICION:
  
//   3+4 = +        (3+4)= 3+4     -1+3 = +           3+-1 =
//        / \                            / \
//       3   4                          –   3
//                                      |
//                                      1
*/
