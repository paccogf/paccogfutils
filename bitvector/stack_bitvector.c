#define BITSPER(T) (sizeof(T)*8)

typedef struct _BitVector {
int size;
BYTE data[0];
} BitVector;

#define BitVector_Create(name, bsize) \
struct _BitVector_##name {\
int size; /* EN BITS */ \
BYTE data[1 + (bsize)/BITSPER(BYTE)]; \
} name = { bsize }
#define BitVector_Size(bitvector) ((bitvector).size)
#define _BitVector_GetElement(bitvector, index) ((bitvector).data[(index) / BITSPER(BYTE)]) // ME DA LA REFERENCIA!
#define _BitVector_Mask(index) (1 << ((index) % BITSPER(BYTE))) // MASCARA NECESARIA PARA ACCEDER AL BIT DENTRO DEL BYTE
#define BitVector_Test(bitvector, index) ((_BitVector_GetElement(bitvector, index) & _BitVector_Mask(index))?1:0)
#define BitVector_Set(bitvector, index) (_BitVector_GetElement(bitvector, index) |= _BitVector_Mask(index))
#define BitVector_Reset(bitvector, index) (_BitVector_GetElement(bitvector, index) &= ~_BitVector_Mask(index))
#define BitVector_Box(pBitVector) ((BitVector *) (pBitVector))
void bitvector_test()
{
int i;
int b;
BitVector_Create(pepe, 10);
for(i=0; i < BitVector_Size(pepe); i++) // PONER A 1
b=BitVector_Set(pepe, i);
for(i=0; i < BitVector_Size(pepe); i++)  // PONER A 0
b=BitVector_Reset(pepe, i);
for(i=0; i < BitVector_Size(pepe); i++)  // COMPROBAR
b=BitVector_Test(pepe, i);

 

}