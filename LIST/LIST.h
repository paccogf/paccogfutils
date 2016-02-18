#ifndef _LIST_H_
#define _LIST_H_

#include <windows.h>

#define LIST_Push(pList, pElement) _LIST_Push(pList, (void *)pElement)

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

LIST LIST_Init();
void LIST_Finalize(LIST List);
BOOL LIST_Empty(LIST List);

void _LIST_Push(LIST List, void *pElement);
void * LIST_Pop(LIST List);


LIST_ITERATOR LIST_ITERATOR_Begin(LIST List);
LIST_ITERATOR LIST_ITERATOR_Next(LIST_ITERATOR Itr);
void *LIST_ITERATOR_GetValue(LIST_ITERATOR Itr);
BOOL LIST_ITERATOR_End(LIST_ITERATOR Itr);


#ifdef __cplusplus
}
#endif






#endif