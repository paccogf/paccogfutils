#ifndef _LIST_H_
#define _LIST_H_

#include <windows.h>
#include <plgtopmalloc.h>

//////////////////////////////////// CAMBIAR DEPENDIENDO DEL ENTORNO /////////////
#define _MALLOC(x)  
#define _FREE(x)       
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
// TAMBIEN PODRÍA SER NECESARIO CAMBIAR/REDEFINIR EL TIPO BOOL
//////////////////////////////////////////////////////////////////////////////////



#ifndef _LIST_C_
typedef void *LIST;
typedef void *LIST_ITERATOR;
#else
typedef struct _LISTNODE LISTNODE;

typedef struct _LISTNODE
{
	void *pValue;
	LISTNODE *pNext;

} LISTNODE, *PLISTNODE;

typedef struct _LIST
{
	LISTNODE *pFirst;

} *LIST;

typedef LISTNODE *LIST_ITERATOR;
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define _NEW(type)    (type *)_MALLOC(sizeof(type))
#define _DELETE(p)    _FREE(p)
LIST LIST_Init();
BOOL LIST_IsInitialized(LIST pList);
void LIST_Finalize(LIST List);
void LIST_Clear(LIST List);

BOOL LIST_IsEmpty(LIST List);
void LIST_Push(LIST pList, void *pElement);
void * LIST_Pop(LIST List);
void *LIST_Head(LIST List);


LIST_ITERATOR LIST_ITERATOR_Begin(LIST List);
LIST_ITERATOR LIST_ITERATOR_Next(LIST_ITERATOR Itr);
void *LIST_ITERATOR_GetValue(LIST_ITERATOR Itr);
BOOL LIST_ITERATOR_End(LIST_ITERATOR Itr);


#ifdef __cplusplus
}
#endif


#endif