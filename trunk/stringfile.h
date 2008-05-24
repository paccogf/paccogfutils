#ifndef _STRING_FILE_H_
#define _STRING_FILE_H_
#include <fstream>
#include <vector>
#include <assert.h>


using namespace std;

// VECTOR DE STRINGS CON CAPACIDADES DE E/S
class StringFile
{
public:
typedef size_t size_type;
static const size_type npos=size_t(-1);

// FUNCIONES AMIGAS PARA E/S A FICHERO
friend size_type getline(StringFile & theFile, string & str) { return theFile.getline(str); }
friend size_type putline(StringFile & theFile, const string & str) { return theFile.putline(str); }
// LEER UNA LINEA DEL FICHERO
friend StringFile & operator<<(StringFile &, const string &);
friend StringFile & operator>>(StringFile &, string &);

// CONSTRUCTOR
StringFile(const string & deffile=""): data(0), pos_index(npos), filename(deffile) { LoadFromFile(); }

// OPERADOR INDEXACION
string & operator[](size_type string_index) { string_index=seek(string_index); assert(string_index!=StringFile::npos); return data[string_index]; }

// CARGA Y SALVADO
bool        LoadFromFile();
bool        LoadFromFile(const string & file_name) { filename=file_name; return LoadFromFile(); }
bool        SaveToFile  ();
bool        SaveToFile  (const string & file_name) { filename=file_name; return SaveToFile(); }

// INTERFAZ E/S DE FICHERO
bool        open        (const string & filename)  { return LoadFromFile(filename); }
void        close       ()                         { SaveToFile(); clear(); }
size_type   getline     (string &);
size_type   putline     (const string &);
// FUNCIONES DE ADMINISTRACION DE TAMAÑO
size_type    nlines()                    { return data.size(); } // TAMAÑO EN LINEAS
void         clear()                     { data.clear(); pos_index=npos; } // VACIADO
size_type    seek(const size_type & pos) { return ((pos>=0) && (pos<data.size()))?pos_index=pos: pos_index=npos; }
size_type    tell()                      { return pos_index; }
//size_type    reset()                     { return 0; }; // PONERNOS AL PPIO DEL FICHERO


protected:
vector <string> data;
size_type pos_index;
string filename;
};

#endif
