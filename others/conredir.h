#ifndef _CONREDIR_H_
#define _CONREDIR_H_

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>


#define CON_INIT() { \
                     AllocConsole();\
                     *stdout=*_fdopen(_open_osfhandle((long) GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT), "w");\
                     setvbuf(stdout, NULL, _IONBF, 0 );\
                   }
                     
#define CON_FREE() FreeConsole();


// redirect unbuffered WIN32 CONSOLE HANDLE IO to C/C++ stdin/stdout/stderr
//	USO ESTANDAR 
//	WIN32_IO_TO_C(STD_INPUT_HANDLE, stdin) --> REDIRIGIR ENTRADA DE CONSOLA
//	WIN32_IO_TO_C(STD_OUTPUT_HANDLE, stdout) --> REDIRIGIR SALIDA CONSOLA
//	WIN32_IO_TO_C(STD_ERROR_HANDLE, stderr)  --> REDIRIGIR SALIDA DE ERROR A CONSOLA

#define WIN32_IO_TO_C(STD_IO_HANDLE, stdio_handle) setvbuf(*std_handle = *_fdopen(_open_osfhandle( (long)GetStdHandle(STD_IO_HANDLE), _O_TEXT), "r"), NULL, _IONBF, 0)
// EQUIVALENTE AL SIGUIENTE CODIGO DE EJEMPLO:
/*
	// redirect unbuffered STDIN to the console
	hConHandle = _open_osfhandle( (long)GetStdHandle(STD_INPUT_HANDLE), _O_TEXT);
	fp = _fdopen(hConHandle, "r");
	*stdin = *fp;
	setvbuf(stdin, NULL, _IONBF, 0);
*/


void RedirectIOToConsole()
{
static const WORD MAX_CONSOLE_LINES = 500;
CONSOLE_SCREEN_BUFFER_INFO coninfo;


	// set the screen buffer to be big enough to let us scroll text
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
	coninfo.dwSize.Y = MAX_CONSOLE_LINES;
	SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

	// redirect unbuffered STDIN to the console
	WIN32_IO_TO_C(STD_INPUT_HANDLE, stdin);
	
	// redirect unbuffered STDOUT to the console
	WIN32_IO_TO_C(STD_OUTPUT_HANDLE, stdout);

	// redirect unbuffered STDERR to the console
	WIN32_IO_TO_C(STD_ERROR_HANDLE, stderr);

	// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
	// point to console as well

#ifdef __cplusplus
	iostream::sync_with_stdio();
#endif
}

// EJEMPLO DE USO
/*

void ShowConsole()
{
	AllocConsole()
	void RedirectIOToConsole();
}












*/



#endif
