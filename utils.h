#pragma once
/////////////////////////////////////////////////////////
#include <windows.h> // Required for Windows API console functions
#include <cstdlib> // For system("cls")
#include "Enums.h"
///////////////////////////////////////////////////////////////


void gotoxy(int x, int y);
void hideCursor();
void cls();
extern bool g_is_silent_mode; //global variable to know if we are in silent mode

//colors
void setTextColor(Color color);
extern bool g_use_colors;
void setCharColor(char c);
