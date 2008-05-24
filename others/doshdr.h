#ifndef _DOSHDR_H_
#define _DOSHDR_H_
#include "typesize.h"

struct doshdr
{
  char c_magic[2];      // MAGIC NUMBER "MZ"
  WORD w_cblp;          // BYTES ON LAST PAGE OF FILE
  WORD w_cp;            // CODE PAGES IN FILE
  WORD w_crlc;          // RELOCATIONS
  WORD w_cparhdr;       // HEADER SIZE (PARAGRAPHS)
  WORD w_minalloc;      // MINIMUM EXTRA PARAGRAPHS NEEDED
  WORD w_maxalloc;      // MAXIMUM EXTRA PARAGRAPHS NEEDED
  WORD w_ss;            // INITIAL (RELATIVE) SS VALUE
  WORD w_sp;            // INITIAL SP VALUE
  WORD w_csum;          // CHECKSUM
  WORD w_ip;            // INITIAL IP VALUE
  WORD w_cs;            // INITIAL (RELATIVE) CS VALUE
  WORD w_lfarlc;        // FILE ADDRESS OF RELOCATION TABLE
  WORD w_ovno;          // OVERLAY NUMBER
  WORD w_res[4];        // RESERVED WORDS
  WORD w_oemid;         // OEM IDENTIFIER
  WORD w_oeminfo;       // OEM INFORMATION
  WORD w_res2[10];      // RESERVED WORDS
  DWORD dw_lfanew;      // FILE ADDRESS OF NEW EXE HEADER      
};

#endif
