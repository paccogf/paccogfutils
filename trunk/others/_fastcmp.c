int __fastcall _fastcmp(BYTE * buf1, BYTE * buf2, DWORD size)
{
DWORD *pdw1 = (DWORD *) buf1;
DWORD *pdw2 = (DWORD *) buf2;
DWORD quotient = size / sizeof(DWORD);  // NUMERO DE DWORDS A COMPARAR
DWORD i;

    // COMPARAR DWORD A DWORD, ANTE LA PRIMERA DIFERENCIA -> DEVOLVER FALSE
    for(i=0; i < quotient; i++)
    {
        if(pdw1[i] != pdw2[i])
        {
            return -1;
        }
    }
    
    // COMPARAR ULTIMOS BYTES (n-remainder < 4)
    for(i=i*4; i < size; i++)
    {
        if(buf1[i] != buf2[i]) 
        {
            return -1;
        }
    }

return 0;
}