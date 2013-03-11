#ifndef _CONDBGMACROS_H_
#define _CONDBGMACROS_H_
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/************************************ LOG *****************************************/
#ifdef _PACODEBUG

#define _DEFAULT_LOG_STREAM "CON"
#define SetLogFile(filename) memcpy(_logfilename, filename, min(sizeof(_logfilename)-1, strlen(filename));
#define Log(frmstr, ...) LogToFile(_logfilename, frmstr, __VA_ARGS__)

static char _logfilename[MAX_PATH] = _DEFAULT_LOG_STREAM;

void LogToFile(const char *filename, const char *frmstr, ...)
{
	FILE *fLog;
	va_list va;


	fLog=fopen(filename, "a");
	if(fLog != NULL)
	{	
		va_start(va, frmstr);
		vfprintf(fLog, frmstr, va);
		va_end(va);

		fclose(fLog); 
	}
}

#endif // _PACODEBUG
/**********************************************************************************/


/*************************** STACK TRACE ******************************************/
#ifdef _STACK_TRACE_H_
#define ST_SIZE (256)
struct
{
	int top;
	char *table[ST_SIZE];

} _STACKTRACE = { ST_SIZE, NULL };


#define ST_BEGIN		{ _STACKTRACE.table[--_STACKTRACE.top]=__FUNCTION__; }
#define ST_END			{ _STACKTRACE.top++; }
#define PRINT_STACK()	{ int i; for(i=_STACKTRACE.top; i<ST_SIZE; i++) printf("%s\n", _STACKTRACE.table[i]); }
#define ST_RETURN		ST_END 
#endif // _STACK_TRACE_H_
/**********************************************************************************/


////////////////////////////// SOLO PARA APLICACIONES EN MODO CONSOLA ///////////////////


// IMPRESION / DEPURACION DE MENSAJES DE ERROR
#define printf_exit(s...) 	{ printf(s); exit(-1); }
#define perror_exit(s) 	{ int e=errno; perror(s); exit(e); }


#define PRINTERROR   {\
                        printf("(\nIn file: %s - line: %d) ", __FILE__, __LINE__);\
                        fflush(NULL);\
                     }
                     
#define FDEBUG(stream, str...) fprintf(stream, str);
#define FLOG(stream, header, str...) {FDEBUG(stream, "%s: %s(): ", header, __FUNCTION__); FDEBUG(stream, str);}
#define LOG(str...) FLOG(stfout, "LOG: ", str)


#define printint(v)     printf("%s=%d", #v, (v))
#define printchar(v) 	printf("%s=%c", #v, (v))
#define printdouble(v) 	printf("%s=%lf", #v, (v))





// RESTO DE MACROS
#define println(...) 	{printf(__VA_ARGS__); putchar('\n'); }
// ESTO ESTA DEFINIDO COMO EXTENSION
// #define println(str...) 	{printf(str); putchar('\n'); }

/*
#define _MYDEBUG_ 0// PARA MENSAJES DE DEPURACION PUNTUALES


#ifndef _MYDEBUG_  // MODO NORMAL, SIN DEPURACION
  #define perror_exit(s) { perror(s); exit(-1); }
  #define printf_exit(s) { printf(s); exit(-1); }
  #define DEBUG(s)
#else // SI NK_DEBUG DEFINIDO (VALE 0 COMO POCO)
  #define perror_exit(s) { int e=errno;\
                           PRINTLF;\
                           errno=e;\
                           perror(s);\
                           exit(-1); }

  #define printf_exit(s) { PRINTLF\
                           printf(s);\
                           exit(-1); }

  #if(_MYDEBUG_>0)
    #define DEBUG(s) { s; fflush(NULL); }
  #else
    #define DEBUG(s)
  #endif

#endif*/


#endif
