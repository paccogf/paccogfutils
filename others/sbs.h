#ifndef _SBS_H_
#define _SBS_H_

/********************************************************************************************/
/*              IMPLEMENTACION DE PILA BASADA EN LA PILA (STACK-BASED STACK)    :-P         */
/*              MINIMAL FINGERPRINT, HERMANO!                                               */
/********************************************************************************************/

#ifndef DIMOF
#define DIMOF(array)    (sizeof(array)/sizeof(array[0])
#endif


#define SBS_DECLARE(type, name, size)   struct\
                                        {\
                                            int topindex;\
                                            type data[size];\
                                        } name

#define SBS_EMPTY                       SBS_INIT
#define SBS_INIT(stack)                 (stack.topindex=-1)
#define SBS_SIZE(stack)                 (DIMOF(stack.data))
#define SBS_TOP_INDEX(stack)            (stack.topindex)
#define SBS_ISEMPTY(stack)              (SBS_TOP_INDEX(stack) == -1)
#define SBS_ISFULL(stack)               (SBS_TOP_INDEX(stack) == (SBS_SIZE(stack) - 1))
#define SBS_TOP(stack)                  (stack.data[SBS_TOP_INDEX(stack)])
#define SBS_PUSH(stack, element)        (stack.data[++SBS_TOP_INDEX(stack)] = element)
#define SBS_POP(stack)                  (stack.data[SBS_TOP_INDEX(stack)--])

	
/************************* EJEMPLO DE USO **************************************************/
/*
    STACK_DECLARE(int, stack1, 10);
    STACK_INIT(stack1); // (RE)INICIALIZAR PILA (VACIAR)
    int i=0;

    while(!STACK_ISFULL(stack1)) // APILAR NUMEROS 1..10
    {
        i++;
        STACK_PUSH(stack1, i);
    }

*/	


/************************* EJEMPLO DE USO (2) **************************************************/
/*
    STACK_DECLARE(int, stack1, 10);
    int i;

    for(STACK_INIT(stack1), i=1; 
        !STACK_ISFULL(stack1); 
        STACK_PUSH(stack1, i), i++); // APILAR NUMEROS 1..10

	
*/


#endif
