#ifndef _STRMACROS_H_
#define _STRMACROS_H_

/////////// MACROS DE MANEJO DE CADENAS, DEBERIAN VALER PARA TODA VERSION /////


// NOT INTENDED TO BE DIRECTLY USED
#define __STRTOCASE(str, n, case) {for(int i=0; i<n; i++) str[i]=case(str[i]); }
#define _STRTOLOWER(str, n) __STRTOCASE(str, n, tolower)
#define _STRTOUPPER(str, n) __STRTOCASE(str, n, toupper)


// MACROS PARA USAR EN MODULOS
#ifdef __cplusplus
   #define STRTOUPPER(str) _STRTOUPPER(str, str.length())
   #define STRTOLOWER(str) _STRTOLOWER(str, str.length())
#else
   #define STRTOUPPER(str) _STRTOUPPER(str, strlen(str))
   #define STRTOLOWER(str) _STRTOLOWER(str, strlen(str))
#endif


#endif
