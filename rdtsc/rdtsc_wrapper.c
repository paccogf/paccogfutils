inline void timeGetTime(long long unsigned int *resul)
{
unsigned int lo, hi;
asm ("rdtsc" :"=a" (lo), "=d" (hi)); // LOS VALORES DE SALIDA ESTAN EN EAX Y EDX, ASIGNAR A lo Y hi
*resul=hi;                            // CONVERTIR A long long unsigned (__int64)
*resul=(*resul<<32) + lo;
}