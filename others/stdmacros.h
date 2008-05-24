#ifndef _STDMACROS_H_
#define _STDMACROS_H_

/******************************************************************************/
/* MACROS DE CARACTER GENERAL, NO DEBERIA NECESITARSE NINGUN INCLUDE          */
/* DEBEN SERVIR PARA CUALQUIER PLATAFORMA/SUBSISTEMA                          */
/******************************************************************************/


// MANEJO Y CONVERSION DE PUNTEROS / TIPOS DE DATO
// ESTAS MACROS FUNCIONAN TANTO DE LVALUE COMO RVALUE
#define PTRTO(T, p)	((T *)(p)) // CONVERTIR PTR DE UN TIPO A OTRO
#define CAST(T, v)	(*PTRTO(T, &(v))) // CONVERTIR VBLE DE UN TIPO A OTRO

// ESTAS PUEDEN DAR CONFLICTOS CON WINDOWS
/*
#define INT(v)		CAST(int, v)
#define CHAR(v)		CAST(char, v)
#define DOUBLE(v)	CAST(double, v)
#define FLOAT(v)	CAST(float, v)
*/

// MANEJO DE ESTRUCTURAS - INICIALIZACION - BLOQUES - BUFFERS - ETC
#define BZERO(pointer, value)	memset(pointer, 0, value)



// MACROS MATEMATICAS
#define MAX(a, b)	(((a)>(b))?(a):(b))
#define MIN(a, b)	(((a)<(b))?(a):(b))

// log2 PARA WINDOWS, EN LINUX YA ES UNA FUNCION DE libc
#ifndef log2
#define log2(x) (log10(x)/log10(2))
#endif


// RESTO DE MACROS




#endif
