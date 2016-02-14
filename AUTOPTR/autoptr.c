

#define AUTOPTR_BEGIN() LIST AUTOPTRNAME = LIST_Init()
#define AUTOMALLOC(size) autoptr_add(AUTOPTRNAME, Pmalloc(size, 0))
#define AUTOPTR(ptr)	autoptr_add(AUTOPTRNAME, ptr)
#define AUTOPTR_END()	autoptr_end(AUTOPTRNAME)
#define RETURN(x)		AUTOPTR_END(); return x



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
		Pfree(todelete);
	}

	LIST_Finalize(autoptrlist);
}




void Test()
{

AUTOPTR_BEGIN();

char *hola = (char *)AUTOMALLOC(10, 0);
strcpy(hola, "hola");
char *adios = (char *)AUTOMALLOC(10, 0);
strcpy(adios, "adios");

RETURN(0);



}

