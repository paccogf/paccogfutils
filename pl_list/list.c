#define _LIST_C_
#include "LIST.h"


BOOL LIST_IsInitialized(LIST pList)
{
  return pList != NULL;
}

BOOL LIST_IsEmpty(LIST pList)
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

void LIST_Finalize(LIST List)
{
  LIST_Clear(List);
  _FREE(List);
}

void LIST_Clear(LIST List)
{
  while(!LIST_IsEmpty(List))
  {
    LIST_Pop(List);
  }
}

void LIST_Push(LIST pList, void *pElement)
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
	_FREE(pAux);

return retval;
}

void *LIST_Head(LIST List)
{
  return List->pFirst->pValue;
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