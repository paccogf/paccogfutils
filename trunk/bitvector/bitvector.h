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




// DEFINICION ALTERNATIVA, NO SE CON CUAL QUEDARME

#ifdef __BITVECTOR2_H__

#define BITSPER(T) (sizeof(T)*8)

typedef struct _BITVECTOR {
	int size;
	BYTE data[1];
} BITVECTOR, *PBITVECTOR;

#define BitVector_Create(name, bsize)	struct _BITVECTOR_##name {\
											int size; /* EN BITS */\
											BYTE data[1 + (bsize)/BITSPER(BYTE)];\
										} name = { bsize }


#define BitVector_Size(bitvector)				((bitvector).size)
#define _BitVector_GetElement(bitvector, index) ((bitvector).data[(index) / BITSPER(BYTE)]) // ME DA LA REFERENCIA!
#define _BitVector_Mask(index)					(1 << ((index) % BITSPER(BYTE))) // MASCARA NECESARIA PARA ACCEDER AL BIT DENTRO DEL BYTE

#define BitVector_Test(bitvector, index)		((_BitVector_GetElement(bitvector, index) >> (index % BITSPER(BYTE))) & 1)
#define BitVector_Set(bitvector, index)			(_BitVector_GetElement(bitvector, index) |= _BitVector_Mask(index))
#define BitVector_Reset(bitvector, index)		(_BitVector_GetElement(bitvector, index) &= ~_BitVector_Mask(index))

#define BitVector_Box(pBitVector) ((BitVector *) (pBitVector))


#endif