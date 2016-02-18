#ifndef _KMPSEARCH_H_
#define _KMPSEARCH_H_
#include <Windows.h>
#include <WinDef.h>

/*
#ifndef DIMOF
#define DIMOF(s) (sizeof(s)/sizeof((s)[0]))
#endif
*/
//#define s_strlen(s) (DIMOF(s) - 1)
#define s_strlen(s) (sizeof(s)-1)
#define s_wcslen(s) (sizeof(s)-2)
typedef struct _KMP_TERM
{ 
	int length; 
	BOOL initialized; 
	BYTE *search_term; 
	int *prefix_table;
} KMP_TERM, *PKMP_TERM;


// DEFINIR ESTRUCTURA E INICIALIZARLA CON EL ARRAY search_term_name
#define _KMP_MAKE_TERM(name, search_term_name, size, prefix_table_name)	KMP_TERM name = {size, FALSE, search_term_name, prefix_table_name}


#define KMP_MAKE_ARRAY(name, ...)	static BYTE search_term_##name[] = __VA_ARGS__; \
									static int prefix_table_##name[sizeof(search_term_##name)] = {0}; \
									static KMP_TERM name = {sizeof(search_term_##name), FALSE, search_term_##name, prefix_table_##name}

#define KMP_MAKE_ASCII(name, ...)	static BYTE search_term_##name[] = __VA_ARGS__; \
									static int prefix_table_##name[s_strlen(search_term_##name)] = {0}; \
									static KMP_TERM name = {s_strlen(search_term_##name), FALSE, search_term_##name, prefix_table_##name}

#define KMP_MAKE_UNICODE(name, ...)	static WCHAR search_term_##name[] = __VA_ARGS__; \
									static int prefix_table_##name[s_wcslen(search_term_##name)] = {0}; \
									static KMP_TERM name = {s_wcslen(search_term_##name), FALSE, (BYTE *) search_term_##name, prefix_table_##name}


/*
#define KMP_MAKE_ARRAY_SIZE(name, size, ...)	static BYTE search_term_##name[] = __VA_ARGS__; \
												static int prefix_table_##name[size] = {0}; \
												static _KMP_MAKE_TERM(name, search_term_##name, size, prefix_table_name)

#define KMP_MAKE_ARRAY(name, ...)	KMP_MAKE_ARRAY_SIZE(name, sizeof(name), __VA_ARGS__)
#define KMP_MAKE_ASCII(name, ...)	KMP_MAKE_ARRAY_SIZE(name, sizeof(name)-1, __VA_ARGS__)
*/



int KMP_Search(BYTE *target, int target_size, KMP_TERM *kmp_term);

#endif