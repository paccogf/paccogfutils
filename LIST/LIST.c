#include <stdio.h>
#include <windows.h>


#define Pfree free
#define Pmalloc(a, b) malloc(a)


#define _NEW(T) (T *)Pmalloc(sizeof(T), 0)
#define _DELETE(p) Pfree(p)
#define NEWNODE() NEW(LISTNODE)


typedef struct _LISTNODE LISTNODE;

typedef struct _LISTNODE
{
	void *pValue;
	LISTNODE *pNext;

} LISTNODE, *PLISTNODE;

typedef struct _LIST
{
	LISTNODE *pFirst;

} LIST, *PLIST;


BOOL LIST_Empty(LIST *pList)
{
	return pList->pFirst == NULL;
}

LIST * LIST_Init()
{
	LIST * retval = _NEW(LIST);
	if(retval != NULL)
	{
		retval->pFirst = NULL;
	}
	return retval;
}


void LIST_Finalize(LIST *pList)
{
	_DELETE(pList);
}

#define  LIST_Push(pList, pElement) _LIST_Push(pList, (void *)pElement)
void _LIST_Push(LIST *pList, void *pElement)
{
	LISTNODE *pAux = _NEW(LISTNODE);
	pAux->pNext = pList->pFirst;
	pAux->pValue = pElement;
	pList->pFirst = pAux;
}


void * LIST_Pop(LIST *pList)
{
	LISTNODE *pAux = pList->pFirst;
	void *retval = pAux->pValue;
	pList->pFirst = pAux->pNext;
	_DELETE(pAux);

return retval;
}

typedef LISTNODE *LIST_ITERATOR;

LIST_ITERATOR LIST_ITERATOR_Begin(LIST *pList)
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


void SimpleTest()
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