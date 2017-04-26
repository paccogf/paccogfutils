

#define VA_NUM_ARGS_IMPL( _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
						 _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
						 _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
						 _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
						 _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
						 _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
						 _61,_62,_63,N,...) N

#define VA_NUM_ARGS_IMPL_(tuple) VA_NUM_ARGS_IMPL tuple

#define __VA_NARGS__(...) VA_NUM_ARGS_IMPL_((__VA_ARGS__,	63,62,61,60,\
								 59,58,57,56,55,54,53,52,51,50, \
								49,48,47,46,45,44,43,42,41,40, \
								 39,38,37,36,35,34,33,32,31,30, \
								 29,28,27,26,25,24,23,22,21,20, \
								 19,18,17,16,15,14,13,12,11,10, \
								 9,8,7,6,5,4,3,2,1))


#define __TOKEN_PASTE(a, b) a##b
#define _TOKEN_PASTE(a, b) __TOKEN_PASTE(a, b)
#define TOKEN_PASTE(a, b) _TOKEN_PASTE(a, b)

#define VFUNC(name, ...) TOKEN_PASTE(name, __VA_NARGS__(__VA_ARGS__))(__VA_ARGS__)



/***************** EJEMPLO DE USO **********************
// (1) GENERAR UN #define COMO EL SIGUIENTE PARA SOBRECARGAR LA FUNCION/MACRO. EN ESTE CASO, LA LLAMAMOS "test":
#define test(...) VFUNC(test, __VA_ARGS__)

// (2) A PARTIR DE AHORA CUALQUIER LLAMADA A test() SE SUSTITUIRA POR LA LLAMADA A test<numero de argumentos>(argumentos). 
//		test("hola")			--> DIGIEVOLUCIONA EN --> test1("hola");
//		test("hola", "adios")--> DIGIEVOLUCIONA EN --> test2("hola", "adios");

// (3) IMPLEMENTAR LA SOBRECARGA DE DICHAS FUNCIONES/MACROS

void test1(char *a)
{
	printf("%s", a);
}

void test2(char *a, char *b)
{
	printf("%s, %s", a, b);

}


#define test3(a, b, c)  test4(a, b, c, "\n") // HACER QUE test4() TENGA UN ARGUMENTO POR DEFECTO


void test4(char a, char b, char c, char d)
{
	printf("%s, %s %s, %s", a, b, c, d);
}

******************************************************************************/