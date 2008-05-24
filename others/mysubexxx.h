#ifndef _MYSUBEX_H_
#define _MYSUBEX_H_

#include <vector>
#include <string>
#include <sstream>

using namespace std;

class SubExpression
{
public:

   class Operator
   {
   friend class SubExpression;
   public:
// TO DO: QUE DEVUELVA <class T>
   typedef int (*Function)(...);
   Operator(char sym, char nops, Function f, char prec): symbol(sym),
                                                         noperands(nops),
                                                         precedence(prec),
                                                         theFunction(f) {}

   protected:
   char     symbol;
   int      noperands;
   char     precedence; // MAYOR VALOR NUMERICO -> SE EVALUA ANTES
   Function theFunction;
   };

   class Atom
   {
   protected:
   virtual int Evaluate(const string & atom) = 0;



   };





public:
// TIPO PARA LOS EVALUADORES DE ATOMO
// TO DO: QUE DEVUELVA <class T>
typedef int (*AtomEvaluator)  (const string & atom);
   
// CONSTRUCTORES
            SubExpression     (): theFunction(0) {} // NO HACE FALTA INICIALIZAR OpVector PUES TIENE CONSTRUCTOR POR DEFECTO
            SubExpression     (const Operator & op, const AtomEvaluator & f=NULL): theFunction(f)
               { OpVector.push_back(op); }

// OPERACIONES BASICAS
bool        AddOperator       (const Operator & op) { OpVector.push_back(op); return true; }
bool        SetEvaluator      (const AtomEvaluator & f) { theFunction=f; return true; }

// METODO PRINCIPAL
// TO DO: QUE DEVUELVA <class T>
int         Evaluate          (const string & expression) const { return (OpVector.size() > 0)?_evaluate(expression, expression.begin(), expression.end()):0; }


protected:
// TO DO: QUE DEVUELVA <class T>
int         _evaluate         (const string & expression, const string::const_iterator & from, const string::const_iterator & to) const;
int         _evaluate_atom    (const string & expression, const string::const_iterator & from, const string::const_iterator & to) const;
Operator * _get_operator_from (char op, int n_operands) const;

// DATOS
vector<Operator> OpVector;
AtomEvaluator    theFunction;
};






                       
#endif

/* ESQUEMA DE DESCOMPOSICION:
   
//   3+4 = +        (3+4)= 3+4     -1+3 = +           3+-1 = 
//        / \                            / \
//       3   4                          -   3
//                                      |
//                                      1

*/
