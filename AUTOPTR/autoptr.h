#ifndef _AUTOPTR_H_
#define _AUTOPTR_H_
#include "LIST.h"

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)
#define AUTOPTRNAME2 TOKENPASTE2(list, __FUNCTION__)
#define AUTOPTRNAME _AUTOPTRLIST_

#define AUTOPTR_BEGIN() LIST AUTOPTRNAME = LIST_Init()
#define AUTOMALLOC(size) autoptr_add(AUTOPTRNAME, malloc(size))
#define AUTOPTR(ptr)	autoptr_add(AUTOPTRNAME, ptr)
#define AUTOPTR_END()	autoptr_end(AUTOPTRNAME)
#define RETURN(x)		AUTOPTR_END(); return x

#ifdef __cplusplus
extern "C"
{
#endif

void *autoptr_add(LIST autolist, void *ptr);
void autoptr_end(LIST autoptrlist);


#ifdef __cplusplus
}
#endif



#endif