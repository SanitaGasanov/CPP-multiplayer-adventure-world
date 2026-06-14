#include "Menu.h"

///////////////////////////////////////////////////////////////////////////////
// Draw the menu screen 
void Menu::draw(int room) const  
{
	if (g_is_silent_mode) return;
	cls();
	gotoxy(0, 0);

	for (size_t i = 0; i < MAX_Y - 1; ++i)
	{
		cout << menu_screens[room][i] << endl;
	}
	cout << menu_screens[room][MAX_Y - 1];
	cout.flush();
}

//////////////////////////////////////////////////////////////////////////////
// Get the user's menu choice
int Menu::user_choice() const
{
	while (true)
	{
		char key = (char)_getch();  ////we wating to char ( we dont need  _kbhit() because now screen is ststic)

		//we dont need here get char if no onw of keys. the loop just start from begining
		if (key == special_nums_for_menu::EIGHT)
		{
			draw(1);
			key = (char)_getch();  //push- and every char go to menu
			draw(0);
		}
		else if (key == special_nums_for_menu::ONE)
		{
			return 1;
		}
		else if (key == special_nums_for_menu::NINE)

		{
			return 9;  //dont need break!!!!
		}
		else if (key == special_nums_for_menu::TWO)
		{
			return 2;
		}

	}
}