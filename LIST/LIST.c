#include <stdio.h>
#include <windows.h>
#define _LIST_C_
#include "LIST.h"

#define _NEW(T) (T *)malloc(sizeof(T))
#define _DELETE(p) free(p)
//#define  LIST_Push(pList, pElement) _LIST_Push(pList, (void *)pElement)



BOOL LIST_Empty(LIST pList)
{
	return pList->pFirst == NULL;
}

LIST LIST_Init()
{
	LIST retval = _NEW(struct _LIST);
	if(retval != NULL)
	{
		retval->pFirst = NULL;
	}
	return retval;
}


void LIST_Finalize(LIST pList)
{
	_DELETE(pList);
}


void _LIST_Push(LIST pList, void *pElement)
{
	LISTNODE *pAux = _NEW(LISTNODE);
	pAux->pNext = pList->pFirst;
	pAux->pValue = pElement;
	pList->pFirst = pAux;
}


void * LIST_Pop(LIST pList)
{
	LISTNODE *pAux = pList->pFirst;
	void *retval = pAux->pValue;
	pList->pFirst = pAux->pNext;
	_DELETE(pAux);

return retval;
}



LIST_ITERATOR LIST_ITERATOR_Begin(LIST pList)
{
	return pList->pFirst;
}


LIST_ITERATOR LIST_ITERATOR_Next(LIST_ITERATOR pItr)
{
	return pItr->pNext;
}


void *LIST_ITERATOR_GetValue(LIST_ITERATOR pItr)
{
	return pItr->pValue;
}

BOOL LIST_ITERATOR_End(LIST_ITERATOR pItr)
{
	return pItr == NULL;
}


/*

void ListTest()
{
	LIST List = LIST_Init();

	LIST_Push(List, 1);
	LIST_Push(List, 2);
	LIST_Push(List, 3);


	for(LIST_ITERATOR itr=LIST_ITERATOR_Begin(List); !LIST_ITERATOR_End(itr); itr=LIST_ITERATOR_Next(itr))
	{
		printf("%d,", LIST_ITERATOR_GetValue(itr));
	}
	
	while(!LIST_Empty(List))
	{   

		printf("\nElemento extraido: %d\n", (int)LIST_Pop(List));
		printf("Elementos restantes: ");
		for(LIST_ITERATOR itr=LIST_ITERATOR_Begin(List); !LIST_ITERATOR_End(itr); itr=LIST_ITERATOR_Next(itr))
		{
			printf("%d, ", LIST_ITERATOR_GetValue(itr));
		}
	}

	LIST_Finalize(List);

}
*/