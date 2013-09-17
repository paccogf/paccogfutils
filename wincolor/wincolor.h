#include <Windows.h>


/////////////////////
// COLOR CONSTANTS
/////////////////////

#define FG_BLUE 		(0x0001)
#define FG_GREEN 		(0x0002)
#define FG_RED 			(0x0004)
#define FG_INTENSITY 	(0x0008)

#define BG_BLUE 		(0x0010)
#define BG_GREEN 		(0x0020)
#define BG_RED 			(0x0040)
#define BG_INTENSITY 	(0x0080)

#define FG_YELLOW 		(FG_GREEN | FG_RED)
#define FG_CYAN 		(FG_GREEN | FG_BLUE)
#define FG_MAGENTA 		(FG_RED | FG_BLUE)

#define BG_YELLOW 		(BG_GREEN | BG_RED)
#define BG_CYAN 		(BG_GREEN | BG_BLUE)
#define BG_MAGENTA 		(BG_RED	 | BG_BLUE)

#define FG_GRAY 		(FG_GREEN | FG_MAGENTA)
#define FG_WHITE 		(FG_GRAY | FG_INTENSITY)

#define BG_GRAY 		(BG_GREEN | BG_MAGENTA)
#define BG_WHITE 		(BG_GRAY | BG_INTENSITY)




////////////////////////////
// CONSOLE COLOR FUNCTIONS
////////////////////////////
HANDLE StdOutHandle()
{
static HANDLE hStdOut = INVALID_HANDLE_VALUE;
	
	if(hStdOut == INVALID_HANDLE_VALUE)
	{
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

return hStdOut;
}


DWORD gettextcolor()
{
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
WORD retval;
	
	if(GetConsoleScreenBufferInfo(StdOutHandle(), &csbiInfo)) 
    {
		retval = csbiInfo.wAttributes; 
	}
	else
	{
		retval = -1;
	}

return (DWORD) retval;
}


void settextcolor(WORD color) 
{
	SetConsoleTextAttribute(StdOutHandle(), color);
}
