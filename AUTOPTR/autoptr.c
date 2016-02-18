#include "autoptr.h"


void *autoptr_add(LIST autolist, void *ptr)
{
	LIST_Push(autolist, ptr);
	return ptr;
}



void autoptr_end(LIST autoptrlist)
{

	while(!LIST_Empty(autoptrlist))
	{   
		void *todelete = LIST_Pop(autoptrlist);
		free(todelete);
	}

	LIST_Finalize(autoptrlist);
}

/*
void AUTOPTR_Test()
{
AUTOPTR_BEGIN();

char *hola = (char *)AUTOMALLOC(10);
strcpy(hola, "hola");
char *adios = (char *)AUTOMALLOC(10);
strcpy(adios, "adios");

AUTOPTR_END();
}

int AUTOPTR_Test2()
{
AUTOPTR_BEGIN();

char *hola = (char *)AUTOMALLOC(10);
strcpy(hola, "hola");
char *adios = (char *)AUTOMALLOC(10);
strcpy(adios, "adios");

RETURN(2);
}

*/