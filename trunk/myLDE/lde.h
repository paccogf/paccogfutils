#ifndef _LDE_H_
#define _LDE_H_
#include <windows.h>



// disasm_flag values:
#define C_66           0x00000001       // 66-prefix
#define C_67           0x00000002       // 67-prefix
#define C_LOCK         0x00000004       // lock
#define C_REP          0x00000008       // repz/repnz
#define C_SEG          0x00000010       // seg-prefix
#define C_OPCODE2      0x00000020       // 2nd opcode present (1st==0F)
#define C_MODRM        0x00000040       // modrm present
#define C_SIB          0x00000080       // sib present
#define C_ANYPREFIX    (C_66|C_67|C_LOCK|C_REP|C_SEG)

extern DWORD disasm_len;                       // 0 if error
extern DWORD disasm_flag;                      // C_xxx
extern DWORD disasm_memsize;                   // value = disasm_mem
extern DWORD disasm_datasize;                  // value = disasm_data
extern DWORD disasm_defdata;                   // == C_66 ? 2 : 4
extern DWORD disasm_defmem;                    // == C_67 ? 2 : 4

extern BYTE  disasm_seg;                       // CS DS ES SS FS GS
extern BYTE  disasm_rep;                       // REPZ/REPNZ
extern BYTE  disasm_opcode;                    // opcode
extern BYTE  disasm_opcode2;                   // used when opcode==0F
extern BYTE  disasm_modrm;                     // modxxxrm
extern BYTE  disasm_sib;                       // scale-index-base
extern BYTE  disasm_mem[8];                    // mem addr value
extern BYTE  disasm_data[8];                   // data value


int disasm(BYTE* opcode0);

#endif
