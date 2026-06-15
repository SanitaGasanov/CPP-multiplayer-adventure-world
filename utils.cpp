#include "utils.h"
//////////////////////////////////////////////////
bool g_use_colors = false;
bool g_is_silent_mode = false;

void gotoxy(int x, int y) {
    if (g_is_silent_mode) return;
    cout.flush();
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
//////////////////////////////////////////////////////

void hideCursor()
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(hStdOut, &curInfo);
    curInfo.bVisible = FALSE; // Set to TRUE to make it visible
    SetConsoleCursorInfo(hStdOut, &curInfo);
}
/////////////////////////////////////////////////////////////////
void cls() {
    if (g_is_silent_mode) return;
    system("cls");
}
//////////////////////////////////////////////////////
void setTextColor(Color color)
{
    if (g_is_silent_mode) return;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)color);
}

void setCharColor(char c)
{
    if (!g_use_colors || g_is_silent_mode) return;

    Color color = Color::WHITE;

    switch (c)
    {
    case PLAYER1_CHAR:
        color = Color::LIGHTCYAN;
        break;
    case PLAYER2_CHAR:
        color = Color::DARKGREY;
        break;
    case BOMB_CHAR:
        color = Color::LIGHTRED;
        break;
    case KEY_CHAR:
    case TORCH_CHAR:
        color = Color::YELLOW;
        break;
    case RIDDLE_CHAR:
        color = Color::LIGHTMAGENTA;
        break;

    default:
        color = Color::LIGHTGREY;
        break;
    }

    setTextColor(color);
}
