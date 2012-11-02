#ifndef _BITVECTOR_H_
#define _BITVECTOR_H_

#include <stdio.h>

#ifndef BYTE
#define BYTE unsigned char
#endif

typedef struct _BITVECTOR {
	size_t size;
	BYTE *data;
} BITVECTOR, *PBITVECTOR;


#define _BitVector_GetByte(bv, index)	((bv).data[(index) / 8])
#define SetBitMask(index)		(1 << ((index) % 8))
#define ResetBitMask(index)		(~SetBitMask(index))

#define BitVector_Make(bv, _data, _size) (bv).size=(size_t)(_size); (bv).data=(BYTE *)(_data);

#define BitVector_Get(bv, index)		((_BitVector_GetByte(bv, index) >> ((index) % 8)) & 1) // OBTENER BIT POS index
#define BitVector_Set(bv, index)		(_BitVector_GetByte(bv, index) |= SetBitMask(index))	// PONER A 1 BIT POS index
#define BitVector_Reset(bv, index)		(_BitVector_GetByte(bv, index) &= ResetBitMask(index))	// PONER A 0 BIT POS INDEX



#endif