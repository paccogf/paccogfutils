#ifndef _PL_LIST_H_

/***************************** INTERFACE ************************************/

typedef struct _PL_LIST_NODE PL_LIST_NODE;

typedef struct _PL_LIST_NODE {

	PL_LIST_NODE	*Next;
	void			*Value;

} PL_LIST_NODE, *PL_LIST;


#define PL_LIST_Value(List)	((*(List))->Value)
#define PL_LIST_First(List) (*(List))
#define PL_LIST_Next(List)	((*(List))->Next)
#define PL_LIST_Empty(List) ((*(List)) == NULL)
#define PL_LIST_Init(List)  ((*(List)) = NULL)


PL_LIST PL_LIST_Add(PL_LIST *List, void *Value);
void * PL_LIST_Remove(PL_LIST *List);


#endif