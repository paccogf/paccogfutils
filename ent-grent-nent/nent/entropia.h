/******************************************************************************/
/************************* FICHERO ENTROPIA.H *********************************/
/******************************************************************************/
#ifndef _ENTROPIA_H_

#include <math.h>


#ifndef log2
#define log2(x) (log10(x)/log10(2))
#endif


double entropia_old(unsigned char *buffer, unsigned int size);
double entropia_sumatorio(int absfreqs[256], unsigned int size, double *sum);
double entropia(unsigned char *buffer, unsigned int size);
double entropia_fast(unsigned char *buffer, unsigned int size);
double * entropia_intervalo(unsigned char * buffer, int leidos, int ENTORNO);
#endif


/************************* FIN DE ENTROPIA.H *********************************/
