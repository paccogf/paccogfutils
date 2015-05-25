// Win32Test.cpp : Defines the entry point for the application.
//

#include "stdafx.h"




typedef struct _IO_PAIR
{
    DWORD WinStdIoName;
    FILE *stdstream;
    char *mode;


} IO_PAIR;

IO_PAIR CONSOLE_INPUT   = {STD_INPUT_HANDLE, stdin, "r" };
IO_PAIR CONSOLE_OUTPUT  = {STD_OUTPUT_HANDLE, stdout, "w"};
IO_PAIR CONSOLE_ERROR   = {STD_ERROR_HANDLE, stderr, "w"};

// ASOCIAR STREAM E/S DE WINDOWS A E/S DE C
void Win32IOToC(IO_PAIR *consoleparams)
{
    int hConHandle = _open_osfhandle((long)GetStdHandle(consoleparams->WinStdIoName), _O_TEXT);
    FILE *fp = _fdopen(hConHandle, consoleparams->mode);
    memcpy(consoleparams->stdstream, fp, sizeof(FILE));
    setvbuf(consoleparams->stdstream, NULL, _IONBF, 0);
    // setvbuf(consoleparams->stdstream, NULL, _IOFBF, 1024);
}

void RedirectIoToConsole()
{
    static const WORD MAX_CONSOLE_LINES=500;
    CONSOLE_SCREEN_BUFFER_INFO coninfo;

    // set the screen buffer to be big enough to let us scroll text
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &coninfo);
    coninfo.dwSize.Y=MAX_CONSOLE_LINES;
    SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), coninfo.dwSize);

    // redirect unbuffered STDIN to the console
    Win32IOToC(&CONSOLE_INPUT);

    // redirect unbuffered STDOUT to the console
    Win32IOToC(&CONSOLE_OUTPUT);

    // redirect unbuffered STDERR to the console
    Win32IOToC(&CONSOLE_ERROR);

    // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
    // point to console as well
    /*
    #ifdef __cplusplus
    #include <iostream>

    std::iostream::sync_with_stdio();
    #endif
    */
}

// EJEMPLO DE USO

/*
void ShowConsole()
{
AllocConsole()
void RedirectIOToConsole();
}
*/


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                        _In_opt_ HINSTANCE hPrevInstance,
                        _In_ LPTSTR    lpCmdLine,
                        _In_ int       nCmdShow)
{
    char c=0;

    AllocConsole();
    RedirectIoToConsole();
    while(c=getchar(), c != '.')
    {
        printf("%c", c);

    }
    
    FreeConsole();

    return 0;
}
