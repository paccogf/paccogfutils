// APLICA CADA A FICHERO DE pattern LA EXPRESION do_expr
#define PROCESSDIR(pattern, do_expr)\
{\
    WIN32_FIND_DATA _PROCESSDIR_DATA_;\
    HANDLE _PROCESSDIR_FIND_;\
    INT _PROCESSDIR_ITR_;\
\
    _PROCESSDIR_FIND_ = FindFirstFile(pattern, &_PROCESSDIR_DATA_);\
    if(_PROCESSDIR_FIND_ == INVALID_HANDLE_VALUE) _PROCESSDIR_ITR_ = 0;\
    else _PROCESSDIR_ITR_ = 1;\
\
    while(_PROCESSDIR_ITR_ > 0)\
    {\
        if(_PROCESSDIR_ITR_ > 1)/* YA NO ESTAMOS EN LA PRIMERA ITERACION*/\
        {\
            if(FindNextFile(_PROCESSDIR_FIND_, &_PROCESSDIR_DATA_) == FALSE) break;\
        }\
        _PROCESSDIR_ITR_++;\
        do_expr;\
\
    }\
}
 
// HACER SANDWICH CON EL CÓDIGO A EJECUTAR POR FICHERO
#define PROCESSDIR_BEGIN(pattern, _PROCESSDIR_DATA_)\
{\
    WIN32_FIND_DATA _PROCESSDIR_DATA_;\
    HANDLE _PROCESSDIR_FIND_;\
    INT _PROCESSDIR_ITR_;\
\
    _PROCESSDIR_FIND_ = FindFirstFile(pattern, &_PROCESSDIR_DATA_);\
    if(_PROCESSDIR_FIND_ == INVALID_HANDLE_VALUE) _PROCESSDIR_ITR_ = 0;\
    else _PROCESSDIR_ITR_ = 1;\
\
    while(_PROCESSDIR_ITR_ > 0)\
    {\
        if(_PROCESSDIR_ITR_ > 1)/* YA NO ESTAMOS EN LA PRIMERA ITERACION*/\
        {\
            if(FindNextFile(_PROCESSDIR_FIND_, &_PROCESSDIR_DATA_) == FALSE) break;\
        }\
        _PROCESSDIR_ITR_++;
 
// CODIGO DE USUARIO AQUI…
 
#define PROCESSDIR_END\
    }\
}
 