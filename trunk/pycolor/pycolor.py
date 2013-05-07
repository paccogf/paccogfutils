# -*- coding: latin-1 -*-
import _pycolor


# PRIMARY COLORS
FG_BLACK =      0x0000
FG_BLUE =		0x0001
FG_GREEN =		0x0002
FG_RED =		0x0004
FG_INTENSITY =	0x0008
BG_BLACK =      0x0000
BG_BLUE =		0x0010
BG_GREEN =		0x0020
BG_RED =		0x0040
BG_INTENSITY =	0x0080

# LIGHT COLORS
FG_LIGHT_BLUE =     FG_INTENSITY | FG_BLUE
FG_LIGHT_GREEN =    FG_INTENSITY | FG_GREEN
FG_LIGHT_RED =      FG_INTENSITY | FG_RED
BG_LIGHT_BLUE =     BG_INTENSITY | BG_BLUE
BG_LIGHT_GREEN =    BG_INTENSITY | BG_GREEN
BG_LIGHT_RED =      BG_INTENSITY | BG_RED

# CMY(-K) COLORS
FG_DARK_YELLOW =	FG_GREEN | FG_RED
FG_DARK_CYAN =		FG_GREEN | FG_BLUE
FG_DARK_MAGENTA =	FG_RED   | FG_BLUE
BG_DARK_YELLOW =	BG_GREEN | BG_RED
BG_DARK_CYAN =		BG_GREEN | BG_BLUE
BG_DARK_MAGENTA =	BG_RED	 | BG_BLUE

# GRAY
FG_GRAY =		    FG_GREEN | FG_RED | FG_BLUE
BG_GRAY =		    BG_GREEN | BG_RED | BG_BLUE

# CMY(-K) LIGHT COLORS
FG_YELLOW =		FG_INTENSITY | FG_DARK_YELLOW 
FG_CYAN =		FG_INTENSITY | FG_DARK_CYAN
FG_MAGENTA =	FG_INTENSITY | FG_DARK_MAGENTA
BG_YELLOW =		BG_INTENSITY | BG_DARK_YELLOW
BG_CYAN =		BG_INTENSITY | BG_DARK_CYAN
BG_MAGENTA =	BG_INTENSITY | BG_DARK_MAGENTA

# WHITE
FG_WHITE =		FG_INTENSITY | FG_GRAY
BG_WHITE =		BG_INTENSITY | BG_GRAY 








getcolor=_pycolor.getcolor
setcolor=_pycolor.setcolor

_colorstack = list()

def pushcolor():
    """
    Push color onto color stack for later retrieval
    """
    _colorstack.insert(0, _pycolor.getcolor())

def popcolor():
    """
    Pop color from top of color stack.
    """
    _pycolor.setcolor(_colorstack.pop())

def printcolor(color, text):
    """
    Print 'text' in 'color'
    """
    pushcolor()
    setcolor(color)
    print text
    popcolor()

__author__ = 'fgonzalez'
if __name__ == "__main__":
    print "This module cannot be executed as a script."