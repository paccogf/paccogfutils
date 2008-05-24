#include "csvstuff.h"

void PrintFileData(FILE *f, char *szFileName, EXEINFO *PEInfo)
{

    fprintf(f, "\"%s\",", szFileName);
    fprintf(f, "\"%s\",", (strrchr(szFileName, '\\')+1));
    fprintf(f, "%d,", PEInfo->size);
    fprintf(f, "%d,", PEInfo->PE->FileHeader.Machine);
    fprintf(f, "%d,", PEInfo->PE->FileHeader.NumberOfSections);
    fprintf(f, "%d,", PEInfo->PE->FileHeader.TimeDateStamp);
    fprintf(f, "%d,", PEInfo->PE->FileHeader.Characteristics);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.AddressOfEntryPoint);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.ImageBase);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.SectionAlignment);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.FileAlignment);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.SizeOfImage);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.SizeOfHeaders);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.CheckSum);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.Subsystem);
    fprintf(f, "%d,", PEInfo->PE->OptionalHeader.DllCharacteristics);
    
    fprintf(f, "%d,", RVAToSection(PEInfo, PEInfo->PE->OptionalHeader.AddressOfEntryPoint));
    fprintf(f, "%d", RVAToRaw(PEInfo, PEInfo->PE->OptionalHeader.AddressOfEntryPoint));
    fprintf(f, "\n");

return;
}


int ProcessFileAsCSV(char *szFilename, void *f)
{

EXEINFO PEInfo;
MAPHANDLE *hMap;
BYTE *map;
DWORD mapsize;
int exitcode = 0;


    // SACAR DATOS DEL PE
    hMap=MapFile(szFilename, &map, &mapsize);

    if(hMap != NULL)
    {
        if((exitcode=GetExeInfo(map, mapsize, &PEInfo)) == EXEINFO_SUCCESS) 
        {
             printf("\r%-79s", szFilename);
             PrintFileData((FILE *) f, szFilename, &PEInfo);
              
        }
        else
        {
              printf("\n%s: No es un MZ-PE válido\n", szFilename);   
        }
        UnMapFile(hMap);
    }


return exitcode;
}


int CSVProcess()
{
// CABECERA
FILE *fCSV;

fCSV=fopen(gProgArgs.szDBFile, "w");

if(fCSV)
{
    fprintf(fCSV, "\"Name\",\"ShortName\",\"Size\",\"Machine\",\"NumberOfSections\",\"TimeDateStamp\",\"Characteristics\",\
\"AddressOfEntryPoint\",\"ImageBase\",\"SectionAlignment\",\"FileAlignment\",\"SizeOfImage\",\"SizeOfHeaders\",\"CheckSum\",\
\"Subsystem\",\"DllCharacteristics\",\"nStartPESection\",\"StartPE\"\n");

    DirRun(gProgArgs.szPathSpec, gProgArgs.bRecursive, ProcessFileAsCSV, fCSV);
}

fclose(fCSV);
return 0;
}






