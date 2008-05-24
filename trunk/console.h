#ifndef _STDCONSOLE_H_
#define _STDCONSOLE_H_

#include <windows.h>
#include <iostream>


namespace std
{
namespace console
{

enum color {/*FORE*/ color_default=7,
                     fg_black=0,
                     fg_blue, 
                     fg_green, 
                     fg_cyan, 
                     fg_red, 
                     fg_magenta,
                     fg_yellow,
                     fg_gray,
                     fg_light,
            /*BACK*/ bg_black=0,
                     bg_blue=16, 
                     bg_green, 
                     bg_cyan, 
                     bg_red, 
                     bg_magenta, 
                     bg_yellow, 
                     bg_gray, 
                     bg_light};  
   
inline color operator|(const color & left, const color & right)
{
return (color)((BYTE)left | (BYTE)right);  
}


inline ostream & operator<<(ostream & out, color c)
{
SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
return out;   
}

}







  
   
}
#endif
