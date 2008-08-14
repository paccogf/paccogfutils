#include "sqlstuff.h"




typedef struct 
{
  char *szSQLName;  // NOMBRE DE LA TABLA
  char *szSQLType; // TIPO 
  char *szFormat;   // COMO SE FORMATEA CON sprintf
  DWORD value;      // VALOR (SI ES DISTINTO DE ESTE, HACER CASTING

} SQLAssociation;



// CONSTANTES PARA AHORRAR TAMAÑO EN LA TABLA DE FICHEROS

char MAINTABLE[]="Files";
char SECTIONTABLE[] = "Sections";
char gszINTEGER[] =         "INTEGER";
char gszTEXT[] =            "TEXT";
char gszFORMAT_INTEGER[] =  "%d";
char gszFORMAT_STRING[] =   "'%s'";



/*********************************  FUNCIONES Y TIPOS DE DATOS PARA LA TABLA PRINCIPAL ***************************/


BYTE gszMD5[33]; // PARA EL CAMPO "MD5" DE LA ESTRUCTURA gFileTable
BYTE *gMap;  // BUFFER AL FICHERO ACTUAL
DWORD gMapSize; // TAMAÑO DEL BUFFER

// BUFFERS PARA CONSTRUIR QUERIES
char *gQueryBuffer = NULL;          // BUFER DONDE ALMACENAR LA QUERY PARA PASARLA A sqlite3_exec()
char *gColsForFileTable = NULL; // ALMACENA LAS COLUMNAS DE LA TABLA DE FICHEROS
char *gColsForSectionTable = NULL; // IDEM CON LAS DE LA TABLA DE SECCIONES
int gID = 0;


/*******************************************************/
//      CADA VEZ QUE SE AÑADA UN CAMPO NUEVO A CUALQUIERA DE LAS 2 TABLAS SIGUIENTES, HAY QUE INCLUIRLO
//      EN LA DEFINICION DE LA TABLA ASI COMO EN LA FUNCION QUE SIRVE PARA ASIGNAR VALORES A DICHA TABLA
//
/*******************************************************/


// TABLA DE FICHEROS CON REPRESENTACION PARA CADA CAMPO DE: NOMBRE, TIPO, CASTING CON sprintf Y VALOR
SQLAssociation gFileTable[] = {

    { "Id",                     "INTEGER PRIMARY KEY AUTOINCREMENT", "NULL", 0 }, // SIEMPRE VA A EXISTIR
    { "Name",                   gszTEXT,    gszFORMAT_STRING,  0 },       
    { "ShortName",              gszTEXT,    gszFORMAT_STRING,  0 }, 
    { "Size",                   gszINTEGER, gszFORMAT_INTEGER, 0 },    //PEInfo->Size
    { "Machine",                gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->FileHeader.Machine
    { "NumberOfSections",       gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->FileHeader.NumberOfSections
    { "TimeDateStamp",          gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->FileHeader.TimeDateStamp
    { "Characteristics",        gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->FileHeader.Characteristics
    { "AddressOfEntryPoint",    gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.AddressOfEntryPoint
    { "ImageBase",              gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.ImageBase
    { "SectionAlignment",       gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.SectionAlignment
    { "FileAlignment",          gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.FileAlignment
    { "SizeOfImage",            gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.SizeOfImage
    { "SizeOfHeaders",          gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.SizeOfHeaders
    { "CheckSum",               gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.CheckSum
    { "Subsystem",              gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.Subsystem
    { "DllCharacteristics",     gszINTEGER, gszFORMAT_INTEGER, 0 }, // PEInfo->PE->OptionalHeader.DllCharacteristics
    
    // CAMPOS CALCULDADOS
    { "nStartPESection",        gszINTEGER, gszFORMAT_INTEGER,  0 }, // dfa->nStartPEsection
    { "StartPE",                gszINTEGER, gszFORMAT_INTEGER,  0 }, // dfa->StartPE
    { "OVLSize",                gszINTEGER, gszFORMAT_INTEGER,  0 }, // dfa->StartPE
    { "MD5",                    gszTEXT,    gszFORMAT_STRING,    (DWORD) gszMD5},

};

// PSEUDOVBLE CONSTANTE CON EL TAMAÑO DE LA TABLA DE ARRIBA
#define gFileTableSize (sizeof(gFileTable) / sizeof(SQLAssociation))

//
// ASIGNACION DE LOS CAMPOS DE UN struct EXEINFO A UNA FILA DE UNA TABLA (gFileTable)
SQLAssociation * XInfo2FileTable(EXEINFO *PEInfo, char *szFileName)
{
int i=1;
int nsections = PEInfo->PE->FileHeader.NumberOfSections;


    // EL ORDEN DE ASIGNACION DEBE COINCIDIR CON EL DE LA TABLA DE ARRIBA!!!
    // EL CAMPO Id NOS LO SALTAMOS, LO AUTOASIGNA SQLITE
    gFileTable[i++].value = (DWORD) szFileName;
    gFileTable[i++].value = (DWORD) (strrchr(szFileName, '\\')+1);
    gFileTable[i++].value = (DWORD) PEInfo->size;
    gFileTable[i++].value = (DWORD) PEInfo->PE->FileHeader.Machine;
    gFileTable[i++].value = (DWORD) nsections;
    gFileTable[i++].value = (DWORD) PEInfo->PE->FileHeader.TimeDateStamp;
    gFileTable[i++].value = (DWORD) PEInfo->PE->FileHeader.Characteristics;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.AddressOfEntryPoint;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.ImageBase;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.SectionAlignment;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.FileAlignment;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.SizeOfImage;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.SizeOfHeaders;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.CheckSum;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.Subsystem;
    gFileTable[i++].value = (DWORD) PEInfo->PE->OptionalHeader.DllCharacteristics;

    // CAMPOS CALCULADOS
    gFileTable[i++].value = (DWORD) RVAToSection(PEInfo, PEInfo->PE->OptionalHeader.AddressOfEntryPoint);
    gFileTable[i++].value = (DWORD) RVAToRaw(PEInfo, PEInfo->PE->OptionalHeader.AddressOfEntryPoint);
    gFileTable[i++].value = (DWORD) (PEInfo->size - PEInfo->section[nsections-1].PointerToRawData - PEInfo->section[nsections-1].SizeOfRawData);
	
    // CALCULO Y ASIGNACION DEL MD5
	if(!gProgArgs.bNoMD5) 
    {
        GetMD5(gMap, gMapSize, (char *) gFileTable[i++].value);
    }
    else // SI HEMOS ESPECIFICADO "-noMD5" DEVOLVEMOS CADENA VACIA ""
    {
        ((char *)(gFileTable[i++].value))[0]=0;
    }
  



return gFileTable;
}

/********************************************* TABLA DE SECCIONES ****************************************************/

// REPRESENTACION DE LA TABLA DE SECCIONES DE UN EJECUTABLE
SQLAssociation gSectionTable[] = {
    
    { "Id",                     gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "Pos",                    gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "Name",                   gszTEXT,    "'%.8s'",           0 },
    { "VirtualSize",            gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "VirtualAddress",         gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "SizeOfRawData",          gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "PointerToRawData",       gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "Characteristics",        gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "PointerToRelocations",   gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "NumberOfRelocations",    gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "PointerToLinenumbers",   gszINTEGER, gszFORMAT_INTEGER,  0 },
    { "NumberOfLinenumbers",    gszINTEGER, gszFORMAT_INTEGER,  0 },
    

};


#define gSectionTableSize (sizeof(gSectionTable) / sizeof(SQLAssociation))


SQLAssociation * XInfo2SectionTable(IMAGE_SECTION_HEADER *section, int index, int id) // FUNCION PARA RELLENAR LA TABLA
{
int i=0;

    // EL ORDEN DE ASIGNACION DEBE COINCIDIR CON EL DE LA TABLA DE ARRIBA!!!
    gSectionTable[i++].value = (DWORD) id;
    gSectionTable[i++].value = (DWORD) index;
    gSectionTable[i++].value = (DWORD) section->Name;
    gSectionTable[i++].value = (DWORD) section->Misc.VirtualSize;
    gSectionTable[i++].value = (DWORD) section->VirtualAddress;
    gSectionTable[i++].value = (DWORD) section->SizeOfRawData;
    gSectionTable[i++].value = (DWORD) section->PointerToRawData;
    gSectionTable[i++].value = (DWORD) section->Characteristics;
    gSectionTable[i++].value = (DWORD) section->PointerToRelocations;
    gSectionTable[i++].value = (DWORD) section->NumberOfRelocations;
    gSectionTable[i++].value = (DWORD) section->PointerToLinenumbers;
    gSectionTable[i++].value = (DWORD) section->NumberOfLinenumbers;
    
    

return gSectionTable;
}



/***********************************************************************************************************************/




//
// RELLENAR ColsForTable CON LOS NOMBRES DE LAS COLUMNAS DE table
int InitializeColumn(SQLAssociation * table, int tablesize, char *ColsForTable)
{
int i;
    for(i=0; i < (tablesize-1); i++)  // CONCATENAR NOMBRES DE COLUMNAS
    {
       strcat(ColsForTable, table[i].szSQLName);
       strcat(ColsForTable, ", ");
    }
    // MENOS LA ULTIMA QUE NO LLEVA COMA
    strcat(ColsForTable, table[i].szSQLName);
return 0;
}


//
// AÑADE LOS DATOS DE UNA ESTRUCTURA PEINFO A LA BASE DE DATOS COMO UNA FILA
// VALE PARA CUALQUIER TABLA
// EL PATRON ES DEL TIPO: "CREATE TABLE %s (%s);", 
char * BuildQueryToCreateTable(char *pattern, char *tablename, SQLAssociation * table, int tablesize)
{
    char temp[5000] = { 0 };
int i;


    for(i=0, temp[0]=0; i < (tablesize-1); i++)  // CONCATENAR NOMBRES Y TIPOS DE COLUMNAS
    {
       strcat(temp, table[i].szSQLName); strcat(temp, " "); strcat(temp, table[i].szSQLType); strcat(temp, ", ");
    }
    // MENOS LA ULTIMA QUE NO LLEVA COMA
    strcat(temp, table[i].szSQLName); strcat(temp, " "); strcat(temp, table[i].szSQLType);

    sprintf(gQueryBuffer, pattern, tablename, temp);

return gQueryBuffer;
}


//
// CONSTRUIR QUERY PARA LA TABLA ESPECIFICADA
//
__inline 
char * BuildQueryToInsert(char *pattern, char *tablename, SQLAssociation * table, int tablesize, char *ColsForTable)
{
int i;
char values[2000]={ 0 };
char temp [2000] = { 0 };

    // VEEEEERY COSTOSO!!!!!
    for(i=0; i < (tablesize-1); i++)  // CONCATENAR NOMBRES DE FORMATEADORES
    {
       sprintf(temp, table[i].szFormat, table[i].value);
       strcat(values, temp);
       strcat(values, ", ");
    }
    // MENOS LA ULTIMA QUE NO LLEVA COMA
    sprintf(temp, table[i].szFormat, table[i].value);
    strcat(values, temp);

    sprintf(gQueryBuffer, pattern, tablename, ColsForTable, values);


return gQueryBuffer;
}





static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
int i;
    for(i=0; i<argc; i++)
    {
        if(strcmp(azColName[i], "last_insert_rowid()") == 0)
        {
            gID = atoi(argv[i]);
            break;
        }
    }
return 0;
}


//
//  EJECUTAR QUERY PARA AÑADIR LOS DATOS DE LA ESTRUCTURA PEINFO A LA BASE DE DATOS
//
int __fastcall AddDataToDB(sqlite3 *db, char *szFilename, EXEINFO *PEInfo)
{
char *zErrMsg = 0;
int rc;
int i;
    // AÑADIR DATOS DEL FICHERO (SIN LAS SECCIONES)
    XInfo2FileTable(PEInfo, szFilename);
    


    rc = sqlite3_exec(db, 
                      BuildQueryToInsert("INSERT INTO %s (%s) values (%s);", MAINTABLE, gFileTable, gFileTableSize, gColsForFileTable), 
                      NULL, 
                      0, 
                      &zErrMsg);

    if(rc == SQLITE_OK)
    {
        rc = sqlite3_exec(db, 
                      "SELECT last_insert_rowid()",
                      callback, 
                      0, 
                      &zErrMsg);

        if(rc == SQLITE_OK)
        {

            // AÑADIR DATOS DE LAS SECCIONES
            for(i=0; i < PEInfo->PE->FileHeader.NumberOfSections; i++)
            {
                
                XInfo2SectionTable(&(PEInfo->section[i]), i, gID);

                rc = sqlite3_exec(db, 
                              BuildQueryToInsert("INSERT INTO %s (%s) values (%s);", SECTIONTABLE, gSectionTable, gSectionTableSize, gColsForSectionTable), 
                              NULL, 
                              0, 
                              &zErrMsg);

                if(rc != SQLITE_OK)
                {
                    fprintf(stderr, "Cannot insert row in SectionTable. SQL error: %s\n", zErrMsg);
                    sqlite3_free(zErrMsg);
                    break;
                }

            }
        }
        else
        {
            fprintf(stderr, "Cannot get last_insert_rowid() in FileTable. SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        
    }
    else
    {
        fprintf(stderr, "Cannot insert row in FileTable. SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    

return 0;
}


//
// PROCESAR FICHERO (LLAMADO DESDE DirRun) 
// 3 FASES
// 1.- MAPEAR FICHERO EN MEMORIA
// 2.- SACAR DATOS RELEVANTES (CABECERA PE, SECCIONES...)
// 3.- AÑADIRLOS A LA BD EN LAS TABLAS QUE SE TERCIE
//

int ProcessFileAsDB(char *szFilename, void *database)
{
EXEINFO PEInfo;
MAPHANDLE *hMap;

int exitcode;


    // SACAR DATOS DEL PE
    hMap=MapFile(szFilename, &gMap, &gMapSize);

    if(hMap != NULL)
    {
        if((exitcode=GetExeInfo(gMap, gMapSize, &PEInfo)) == EXEINFO_SUCCESS) 
        {
             printf("\r%-79s", szFilename);
             AddDataToDB((sqlite3 *)database, szFilename, &PEInfo);
        }
        else
        {
            printf("\n%s: No es un MZ-PE válido\n", szFilename);
        }
        UnMapFile(hMap);
    }

return 0;
}


//
// FUNCION 'PRINCIPAL' DE INTRODUCCION DE DATOS SQL. RECORRE LA LISTA DE FICHEROS 
// Y METE SUS DATOS EN LA BASE DE DATOS
//
int SQLiteProcess()
{
sqlite3 *db;
char *zErrMsg = 0;
int rc;

    gQueryBuffer = (char *) malloc(5000);
    *gQueryBuffer = 0;

    gColsForFileTable = (char *) malloc(2000);
    *gColsForFileTable =  0;

    gColsForSectionTable = (char *) malloc(2000);
    *gColsForSectionTable = 0;

    rc = sqlite3_open(gProgArgs.szDBFile, &db); 

    if( rc == SQLITE_OK )
    {
        rc = sqlite3_exec(db, 
                          BuildQueryToCreateTable("CREATE TABLE IF NOT EXISTS %s (%s);",
                                                  MAINTABLE, 
                                                  gFileTable, 
                                                  gFileTableSize),
                          NULL,
                          0,
                          &zErrMsg);

        if(rc == SQLITE_OK)
        {
            rc = sqlite3_exec(db, 
                              BuildQueryToCreateTable("CREATE TABLE IF NOT EXISTS %s (%s);",
                                                      SECTIONTABLE, 
                                                      gSectionTable,
                                                      gSectionTableSize),
                              NULL,
                              0,
                              &zErrMsg); 

            //
            // TODO OK
            if(rc == SQLITE_OK)
            {

                ////////////////////////////////////////////////////////////////////////////
                InitializeColumn(gFileTable, gFileTableSize, gColsForFileTable);
                InitializeColumn(gSectionTable, gSectionTableSize, gColsForSectionTable);

                DirRun(gProgArgs.szPathSpec, gProgArgs.bRecursive, ProcessFileAsDB, db);
                ///////////////////////////////////////////////////////////////////////////////

            }
            else  // NO SE PUDO CREAR LA TABLA DE DATOS DE LAS SECCIONES
            {
                fprintf(stderr, "Cannot create Section Data Table. SQL error: %s\n", zErrMsg);
                sqlite3_free(zErrMsg);
            } // if(rc == SQLITE_OK) 

        } // if(rc == SQLITE_OK)
        else  // NO SE PUDO CREAR LA TABLA DE DATOS DE LOS FICHEROS
        {
            fprintf(stderr, "Cannot create File Data Table. SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
    
        } // if(rc == SQLITE_OK)

        sqlite3_close(db); 
    } // if( rc )
    else
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    }

 
    // SE PODRÍAN QUITAR, PUES EL PROGRAMA TERMINA DESPUES DE LLAMAR A ESTA FUNCION ¿NO?
    free(gQueryBuffer);
    free(gColsForFileTable);
    free(gColsForSectionTable);

    
return 0;
}

////////////////////////////////////////////////////////////////////////////
int PrintTableScheme(char *tablename, SQLAssociation *table, int tablesize)
{
int i;
    printf("| Tabla: %s |\n", tablename);
    printf("%-30s%s\n", "Campo", "Tipo");
    printf("-------------------------------------------\n");
    for(i=0; i < tablesize; i++)
    {
        printf("%-30s%s\n", table[i].szSQLName, table[i].szSQLType);
    }
return 0;
}

//
// IMPRIMIR ESQUEMA DE TABLAS
//
void SQLiteHelp()
{
   
    printf("\nTablas y campos definidos:\n\n");
    
    printf("\n");
    PrintTableScheme(MAINTABLE, gFileTable, gFileTableSize);

    printf("\n");
    PrintTableScheme(SECTIONTABLE, gSectionTable, gSectionTableSize);
    
    printf("\n");

return;
}



/*

// NOMBRE DE LA COLUMNA
#define SQL_XINFO_COLUMNS "Id,"     "Name,"      "ImageBase,"                           "CheckSum,"                     "SubSystem,"                       "Size,"   "NumberOfSections,"                 "TimeDateStamp,"                 "AddressOfEntryPoint"
// FORMATO
#define SQL_XINFO_FORMAT  "NULL,"   "'%s',"      "%d,"                                  "%d,"                           "%d,"                              "%d,"     "%d,"                               "%d,"                            "%d"
// DATOS
#define SQL_XINFO_VALUES(p)         szFilename,  PEInfo->PE->OptionalHeader.ImageBase,  p->PE->OptionalHeader.CheckSum, p->PE->OptionalHeader.Subsystem,   p->size,  p->PE->FileHeader.NumberOfSections, p->PE->FileHeader.TimeDateStamp, p->PE->OptionalHeader.AddressOfEntryPoint
        


//#define SQL_INSERT_FILE "insert into "MAINTABLE" ("SQL_XINFO_COLUMNS") values ("SQL_XINFO_FORMAT");"

// CONSULTA PARA AÑADIR DATOS DE FICHERO A LA TABLA DE FICHEROS
// PATRON DEL TIPO: "INSERT INTO %s (%s") values (%s);"





gszFileName=szName;
printf("\n\n");
printf("===============================================================================\n");
printf("Fichero: %s\n", szName);
printf("===============================================================================\n");

printf("\nCabecera MZ\n-----------\n");
CSV_WORD(PEInfo->MZ->e_magic);                     // Magic number
CSV_WORD(PEInfo->MZ->e_cblp);                      
CSV_WORD(PEInfo->MZ->e_cp);                        // Pages in file
CSV_WORD(PEInfo->MZ->e_crlc);                      // Relocations
CSV_WORD(PEInfo->MZ->e_cparhdr);                   // Size of header in paragraphs
CSV_WORD(PEInfo->MZ->e_minalloc);                  // Minimum extra paragraphs needed
CSV_WORD(PEInfo->MZ->e_maxalloc);                  // Maximum extra paragraphs needed
CSV_WORD(PEInfo->MZ->e_ss);                        // Initial (PEInfo->MZ->relative) SS value
CSV_WORD(PEInfo->MZ->e_sp);                        // Initial SP value
CSV_WORD(PEInfo->MZ->e_csum);                      // Checksum
CSV_WORD(PEInfo->MZ->e_ip);                        // Initial IP value
CSV_WORD(PEInfo->MZ->e_cs);                        // Initial (PEInfo->MZ->relative) CS value
CSV_WORD(PEInfo->MZ->e_lfarlc);                    // File address of relocation table
CSV_WORD(PEInfo->MZ->e_ovno);                      // Overlay number
//CSV_WORD(PEInfo->MZ->e_res[4]);                    // Reserved words
CSV_WORD(PEInfo->MZ->e_oemid);                     // OEM identifier (PEInfo->MZ->for e_oeminfo)
CSV_WORD(PEInfo->MZ->e_oeminfo);                   // OEM information); e_oemid specific
//CSV_WORD(PEInfo->MZ->e_res2[10]);                  // Reserved words
CSV_DWORD(PEInfo->MZ->e_lfanew);                    // File address of new exe header


printf("\nCabecera PE\n-----------\n");
CSV_DWORD(PEInfo->PE->Signature);
CSV_WORD(PEInfo->PE->FileHeader.Machine);
CSV_WORD(PEInfo->PE->FileHeader.NumberOfSections);
CSV_DWORD(PEInfo->PE->FileHeader.TimeDateStamp);
CSV_DWORD(PEInfo->PE->FileHeader.PointerToSymbolTable);
CSV_DWORD(PEInfo->PE->FileHeader.NumberOfSymbols);
CSV_WORD(PEInfo->PE->FileHeader.SizeOfOptionalHeader);
CSV_WORD(PEInfo->PE->FileHeader.Characteristics);


printf("\nCabecera PE Opcional\n--------------------\n");
CSV_DWORD(PEInfo->PE->OptionalHeader.AddressOfEntryPoint);
CSV_DWORD(PEInfo->PE->OptionalHeader.ImageBase);
CSV_DWORD(PEInfo->PE->OptionalHeader.SectionAlignment);
CSV_DWORD(PEInfo->PE->OptionalHeader.FileAlignment);
CSV_WORD(PEInfo->PE->OptionalHeader.MajorOperatingSystemVersion);
CSV_WORD(PEInfo->PE->OptionalHeader.MinorOperatingSystemVersion);
CSV_WORD(PEInfo->PE->OptionalHeader.MajorImageVersion);
CSV_WORD(PEInfo->PE->OptionalHeader.MinorImageVersion);
CSV_WORD(PEInfo->PE->OptionalHeader.MajorSubsystemVersion);
CSV_WORD(PEInfo->PE->OptionalHeader.MinorSubsystemVersion);
CSV_DWORD(PEInfo->PE->OptionalHeader.Win32VersionValue);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfImage);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfHeaders);
CSV_DWORD(PEInfo->PE->OptionalHeader.CheckSum);
CSV_WORD(PEInfo->PE->OptionalHeader.Subsystem);
CSV_WORD(PEInfo->PE->OptionalHeader.DllCharacteristics);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfStackReserve);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfStackCommit);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfHeapReserve);
CSV_DWORD(PEInfo->PE->OptionalHeader.SizeOfHeapCommit);
CSV_DWORD(PEInfo->PE->OptionalHeader.LoaderFlags);
CSV_DWORD(PEInfo->PE->OptionalHeader.NumberOfRvaAndSizes);
   // IMAGE_DATA_DIRECTORY DataDirectory[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];

*/



/*


static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName)
{
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}
*/