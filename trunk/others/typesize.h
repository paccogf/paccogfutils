#ifndef _TYPESIZE_H_
#define _TYPESIZE_H_

#if defined(WIN32) || defined(LINUX)
// ARQUITECTURA DE 32 BITS
  #define WORD unsigned short int
  #define DWORD unsigned int
#else
// ARQUITECTURA DE 16 BITS
  #define WORD unsigned int
  #define DWORD unsigned long int
#endif

#define BYTE unsigned char


typedef DWORD dword;
typedef WORD word;
typedef BYTE byte;


#endif
