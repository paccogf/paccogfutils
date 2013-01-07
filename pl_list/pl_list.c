#include <pl_list.h>


/*********************** IMPLEMENTEISHON *************************/



PL_LIST PL_LIST_Add(PL_LIST *List, void *Value)
{
PL_LIST_NODE *pNewNode;

	// if((pNewNode = Pmalloc(sizeof(PL_LIST_NODE), 0)) != NULL)
	if((pNewNode = (PL_LIST_NODE *) malloc(sizeof(PL_LIST_NODE))) != NULL)
	{
		pNewNode->Value = Value;
		pNewNode->Next	= *List;
		*List			= pNewNode;
	}

return pNewNode;
}



// LA LISTA NO DEBE ESTAR VACIA!!!
void * PL_LIST_Remove(PL_LIST *List)
{
void *retval;
PL_LIST_NODE *pAuxNode;
	
	pAuxNode = *List;
	*List = pAuxNode->Next;
	retval = pAuxNode->Value;
	
	free(pAuxNode); // SE BORRA EL NODO, NO SU "VALOR"		


return retval;
}


// VACIAR TODA LA LISTA
void PL_LIST_Clear(PL_LIST *List)
{
	
	while(!PL_LIST_Empty(List))
	{
		PL_LIST_Remove(List);
	}


}


/*
void nodelist_test()
{
PL_LIST List;
PL_LIST lAux;
void *vAux;

	PL_LIST_Init(&List);

	PL_LIST_Add(&List, (void *) 1);
	PL_LIST_Add(&List, (void *) 2);
	PL_LIST_Add(&List, (void *) 3);

	for(lAux=PL_LIST_First(&List); !PL_LIST_Empty(&lAux); lAux=PL_LIST_Next(&lAux))
	{

		printf("Nodelist value: %x\n", PL_LIST_Value(&lAux));

	}

	vAux = PL_LIST_Remove(&List);
	vAux = PL_LIST_Remove(&List);
	vAux = PL_LIST_Remove(&List);
}
*/
