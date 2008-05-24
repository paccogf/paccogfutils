#ifndef _NETCAT_H_
#define _NETCAT_H_

#define NK_DEBUG 0// PARA MENSAJES DE DEPURACION PUNTUALES

#define BYTE char
#define BOOL char
#define TRUE 1
#define FALSE 0

#define PRINTV(var) printf("Variable %s vale algo", #var);

#define PRINTLF { printf("\n(In file: %s - line: %d) ", __FILE__, __LINE__); fflush(NULL); }

#ifndef NK_DEBUG  // MODO NORMAL, SIN DEPURACION
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

  #if(NK_DEBUG>0)
    #define DEBUG(s) { s; fflush(NULL); }
  #else
    #define DEBUG(s)
  #endif

#endif


#define MAX(a, b) ((a>b)?a:b)
#define max MAX
#define FLUSH(s) { printf(s); fflush(NULL); }


#endif
