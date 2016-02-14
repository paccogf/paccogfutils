#ifndef _LIST_H_
#define _LIST_H_

#include <windows.h>

extern "C"
{

typedef void *LIST;


LIST LIST_Init();
void LIST_Finalize(LIST List);
BOOL LIST_Empty(LIST List);
#define  LIST_Push(List, Element) _LIST_Push(List, (void *)Element)
void _LIST_Push(LIST List, void *pElement);
void * LIST_Pop(LIST List);

typedef void *LIST_ITERATOR;
LIST_ITERATOR LIST_ITERATOR_Begin(LIST List);
LIST_ITERATOR LIST_ITERATOR_Next(LIST_ITERATOR Itr);
void *LIST_ITERATOR_GetValue(LIST_ITERATOR Itr);
BOOL LIST_ITERATOR_End(LIST_ITERATOR Itr);
}

#endif