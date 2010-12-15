#ifndef _HPRINTF_H_
#define _HPRINTF_H_

#include <stdio.h>
#include <stdarg.h>

static
int hprintf(char *format)
{
int i;
static char _default_indent_char_ = '\t';  // TIENE QUE SER UN CARACTER (DE MOMENTO)
static int _indentation_level_ = 0;
va_list lpvar; 
int indentation_delta = 0;



    //
    // MIRAR SI SE ESPECIFICO INDENTACION
    //
    if((format[0] == '[') && (format[2] == ']'))
    {
        if(format[1] == '+')    // "[+]..."
        {
            indentation_delta = 1;        
        }
        else if((format[1] == '-') && (_indentation_level_ > 0)) // "[-]..."
        {
            indentation_delta = -1;        
        }
    }

    
    if(indentation_delta < 0) _indentation_level_--;
    
    // ESCRIBIR CARACTER DE INDENTACION LAS VECES QUE HAGA FALTA
    for(i=0; i < _indentation_level_; i++)
    {
        putchar(_default_indent_char_);
    }

    if(indentation_delta > 0) _indentation_level_++; // SI SE INCREMENTA LA INDENTACION, HACERLO EFECTIVO TRAS IMPRIMIR INDENTACION

    va_start(lpvar, format);
    i=vprintf(format, lpvar);
    va_end(lpvar);



return _indentation_level_;
}

#endif
