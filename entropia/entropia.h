#ifndef _ENTROPIA_H_


#include <math.h>

 
#ifndef log2

#define log2(x) (log10(x)/log10(2))

#endif


double entropia(unsigned char *buffer, unsigned int size);

#endif
