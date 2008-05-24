#include "stringfile.h"

bool StringFile::LoadFromFile()
{
if(filename.empty()) return false;
ifstream fin;
fin.open(filename.c_str());

if(!fin) return false;

string buffer;
clear();
while(std::getline(fin, buffer))
{
  data.push_back(buffer);
}

// INICIALIZAR PUNTERO AL FICHERO
fin.close();
return (seek(0)!=npos); // TRUE SI HAY DATOS
}


bool StringFile::SaveToFile()
{
if(filename.empty()) return false;
ofstream fout;

// COMPROBAR SI SE ABRIO BIEN EL FICHERO
fout.open(filename.c_str());
if(!fout) return false;

for(vector<string>::iterator itr=data.begin(); itr!=data.end(); itr++)
   fout << *itr << endl;
fout.close();

return true;
}

StringFile::size_type StringFile::getline(string & str)
{
size_type pos;
if(pos_index==npos) 
{
   return npos;
}

str=data[pos_index];
pos=pos_index;
// AVANZAMOS PUNTERO AL SGUTE ELEMENTO, SI NO HAY, LA PROXIMA VEZ tell() DARA npos
seek(pos_index+1); // ESTE METODO YA HACE LAS COMPROBACIONES NECESARIAS
return pos;
}

// ESCRIBIR LINEA EN LA POSICION pos_index
StringFile::size_type StringFile::putline(const string & str)
{
size_type pos;

// AÑADIR LINEA SI ESTAMOS AL FINAL (MAS ALLA) DEL FICHERO
if(pos_index==npos) 
{
   pos_index=data.size();
   data.push_back(str);
}
else data[pos_index]=str; // CAMBIAR LINEA SI NO

pos=pos_index;
// AVANZAMOS PUNTERO AL SGUTE ELEMENTO, SI NO HAY, LA PROXIMA VEZ tell() DARA npos
seek(pos_index+1); 
return pos; // RETORNAR Nº DE LINEA QUE HEMOS ESCRITO
}



StringFile & operator<<(StringFile & theFile, const string & str)
{
// IR A FINAL DE FICHERO E INSERTAR LINEA!!!
theFile.seek(theFile.nlines());
putline(theFile, str);
return theFile;   
}


StringFile & operator>>(StringFile & theFile, string & str)
{
theFile.getline(str);
return theFile;   
}
