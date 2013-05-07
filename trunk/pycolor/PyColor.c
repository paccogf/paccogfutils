#include <Windows.h>
#include <Python.h>


//
// MIXED NATIVE/PYTHON MODULE, DEMONSTRATING SIMPLE IMPLEMENTATION OF TEXT COLOR CHANGE
// ONLY FOR WINDOWS, SORRY...
//

/*
# SIMPLE USAGE (REAL WORLD EXAMPLE)

 print "Disinfecting ...",
        try:
            element.DisinfectVirus()

            pushcolor(); setcolor(FOREGROUND_GREEN | FOREGROUND_INTENSITY)
            print "virus disinfected." # PRINT IN BRIGHT GREEN, RESTORE OLD COLOR
            popcolor()

        except Error as e:
            pushcolor(); setcolor(FOREGROUND_RED | FOREGROUND_INTENSITY)
            print "could not disinfect" # PRINT IN BRIGHT RED, RESTORE OLD COLOR
            popcolor()
            print e
*/


//////////////////////////////////////////////////////////////////////////////////////
// OS DEPENDANT FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////
static HANDLE StdOutHandle()
{
static HANDLE hStdOut = INVALID_HANDLE_VALUE;
	
	if(hStdOut == INVALID_HANDLE_VALUE)
	{
		hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

return hStdOut;
}


static DWORD gettextcolor()
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


static void settextcolor(WORD color) 
{
	SetConsoleTextAttribute(StdOutHandle(), color);
}

//////////////////////////////////////////////////////////////////////////////////////
// PYTHON ENVIRONMENT FUNCTIONS
//////////////////////////////////////////////////////////////////////////////////////
/*
static void PyColor_DefineColorCtes()
{
	PyRun_SimpleString("pycolor.FOREGROUND_BLUE =		0x0001");
	PyRun_SimpleString("pycolor.FOREGROUND_GREEN =		0x0002");
	PyRun_SimpleString("pycolor.FOREGROUND_RED =		0x0004");
	PyRun_SimpleString("pycolor.FOREGROUND_INTENSITY =	0x0008");
	PyRun_SimpleString("pycolor.BACKGROUND_BLUE =		0x0010");
	PyRun_SimpleString("pycolor.BACKGROUND_GREEN =		0x0020");
	PyRun_SimpleString("pycolor.BACKGROUND_RED =		0x0040");
	PyRun_SimpleString("pycolor.BACKGROUND_INTENSITY =	0x0080");

	// CYAN, MAGENTA, YELLOW
	PyRun_SimpleString("pycolor.FOREGROUND_YELLOW =		pycolor.FOREGROUND_GREEN | pycolor.FOREGROUND_RED");
	PyRun_SimpleString("pycolor.FOREGROUND_CYAN =		pycolor.FOREGROUND_GREEN | pycolor.FOREGROUND_BLUE");
	PyRun_SimpleString("pycolor.FOREGROUND_MAGENTA =	pycolor.FOREGROUND_RED   | pycolor.FOREGROUND_BLUE");
	PyRun_SimpleString("pycolor.BACKGROUND_YELLOW =		pycolor.BACKGROUND_GREEN | pycolor.BACKGROUND_RED");
	PyRun_SimpleString("pycolor.BACKGROUND_CYAN =		pycolor.BACKGROUND_GREEN | pycolor.BACKGROUND_BLUE");
	PyRun_SimpleString("pycolor.BACKGROUND_MAGENTA =	pycolor.BACKGROUND_RED	 | pycolor.BACKGROUND_BLUE");

	PyRun_SimpleString("pycolor.FOREGROUND_GRAY =		pycolor.FOREGROUND_GREEN | pycolor.FOREGROUND_MAGENTA");
	PyRun_SimpleString("pycolor.FOREGROUND_WHITE =		pycolor.FOREGROUND_GRAY | pycolor.FOREGROUND_INTENSITY");

	PyRun_SimpleString("pycolor.BACKGROUND_GRAY =		pycolor.BACKGROUND_GREEN | pycolor.BACKGROUND_MAGENTA");
	PyRun_SimpleString("pycolor.BACKGROUND_WHITE =		pycolor.BACKGROUND_GRAY | pycolor.BACKGROUND_INTENSITY");
}
*/

static PyObject* PyColor_getcolor(PyObject *self, PyObject *args)
{
DWORD color;

    color=gettextcolor();

return Py_BuildValue("i", color);
}


static PyObject* PyColor_setcolor(PyObject *self, PyObject *args)
{
DWORD color;

    if (!PyArg_ParseTuple(args, "i", &color))
        return NULL;

	settextcolor((WORD) color);

return Py_BuildValue("");
}

/*
static void PyColor_InitColorStackStuff()
{
	PyRun_SimpleString("pycolor.colorstack = list()");
	PyRun_SimpleString("pycolor.pushcolor = lambda: pycolor.colorstack.insert(0, pycolor.getcolor())");
	PyRun_SimpleString("pycolor.popcolor = lambda: pycolor.setcolor(pycolor.colorstack.pop())");
}
*/


static PyMethodDef PyColorMethods[] = 
{
	{"getcolor", PyColor_getcolor, METH_NOARGS, "Get foreground/background color."},
	{"setcolor", PyColor_setcolor, METH_VARARGS, "Set foreground/background color."},
	//{"pushcolor", PyColor_pushcolor, METH_VARARGS, "push/save current color onto stack."},
	//{"popcolor", PyColor_popcolor, METH_NOARGS, "pop/restore current color from top of stack."},
	{NULL, NULL, 0, NULL}        // Sentinel 
};


//////////////////////////////////////////////////////////////////////////////////////
// MODULE INITIALIZATION
//////////////////////////////////////////////////////////////////////////////////////
PyMODINIT_FUNC init_pycolor(void)
{
    Py_InitModule("_pycolor", PyColorMethods);
}

