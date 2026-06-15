#define _CRT_SECURE_NO_WARNINGS
#include "Screen.h"
#include "Player.h"


///////////////////////////////////////////
// also handle dark rooms 
void Screen::draw(int room, bool hasTorch) const
{
	if (g_is_silent_mode) return;
	if (room < 0 || room >= current_screens.size()) return;

	cls();
	gotoxy(0, 0);

	bool is_room_dark = dark_rooms[room].isDark;
	bool shouldHideDetails = is_room_dark && !hasTorch; //Check if we should hide details

	if (!shouldHideDetails) //if The room is not dark or the player has a torch - draw normally
	{
		for (size_t i = 0; i < current_screens[room].size() - 1; ++i)
		{
			if (g_use_colors) // If colors are ON, print char-char to color them
			{
				for (char c : current_screens[room][i])
				{
					setCharColor(c);
					cout << c;
				}
				setTextColor(Color::LIGHTGREY);
				cout << endl;
			}
			else
			{
				cout << current_screens[room][i] << endl;
			}
		}

		// Handle last line
		if (g_use_colors)
		{
			for (char c : current_screens[room].back())
			{
				setCharColor(c);
				cout << c;
			}
			setTextColor(Color::LIGHTGREY);
		}
		else
		{
			cout << current_screens[room].back();
		}
	}

	else //need to hide details
	{

		int x1 = dark_rooms[room].x1;
		int y1 = dark_rooms[room].y1;
		int x2 = dark_rooms[room].x2;
		int y2 = dark_rooms[room].y2;

		for (size_t y = 0; y < current_screens[room].size(); ++y)
		{
			for (size_t x = 0; x < current_screens[room][y].size(); ++x)
			{
				bool inside = (x >= x1 && x <= x2) && (y >= y1 && y <= y2);

				if (inside) {
					cout << ' ';
				}
				else
				{
					if (g_use_colors)
					{
						char c = current_screens[room][y][x];
						setCharColor(c);
						cout << c;
						setTextColor(Color::LIGHTGREY);
					}
					else
					{
						cout << current_screens[room][y][x];
					}
				}
			}
			cout << endl;
		}
	}
	cout.flush();
}
//////////////////////////////////////////////////////////////////////////////////////////

void Screen::setInventoryAt(const Player& my_player, int room)
{
	if (g_is_silent_mode) return;

	if (notValidRoom(room))
		return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + INVENTORY_Y;

	int x_offset = (my_player.getCharacter() == '$') ? INVENTORY_X_P1 : INVENTORY_X_P2;

	ItemType my_item_type = my_player.getHeldItem();
	const char* new_inventory = "";

	switch (my_item_type)
	{
	case ItemType::NONE:
		new_inventory = "NONE";
		break;
	case ItemType::KEY:
		new_inventory = "Key";
		break;
	case ItemType::BOMB:
		new_inventory = "Bomb";
		break;
	case ItemType::SWITCH_ON:
		new_inventory = "On";
		break;
	case ItemType::SWITCH_OFF:
		new_inventory = "Off";
		break;
	case ItemType::TORCH:
		new_inventory = "Torch";
		break;
	}

	int i = 0;
	char ch = new_inventory[i];


	while (ch != '\0')
	{
		if (baseX + x_offset + i < MAX_X)
		{
			current_screens[room][y][baseX + x_offset + i] = ch;
			gotoxy((int)(baseX + x_offset + i), y);
			cout << ch;
		}
		i++;
		ch = new_inventory[i];
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Screen::delInventoryAt(const Player& my_player, int room)
{
	if (g_is_silent_mode) return;
	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + INVENTORY_Y;

	int x_offset;
	char stop_char;

	if (my_player.getCharacter() == '$')
	{
		x_offset = INVENTORY_X_P1;
		stop_char = '&';
	}
	else
	{
		x_offset = INVENTORY_X_P2;
		stop_char = '\0';
	}

	if (baseX + x_offset >= MAX_X) return;

	int i = 0;
	char ch = current_screens[room][y][baseX + x_offset + i];

	while ((ch != ' ') && (ch != stop_char))
	{
		if (baseX + x_offset + i < MAX_X)
		{
			current_screens[room][y][baseX + x_offset + i] = ' ';
			gotoxy((int)(baseX + x_offset + i), y);
			cout << ' ';

			i++;

			if (baseX + x_offset + i < MAX_X)
				ch = current_screens[room][y][baseX + x_offset + i];
			else
				break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////

void Screen::setHealthAt(const Player& my_player, int room)
{
	if (g_is_silent_mode) return;
	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + HEALTH_Y;

	int x_offset = (my_player.getCharacter() == '$') ? HEALTH_X_P1 : HEALTH_X_P2;

	char new_life_val = (char)((my_player.getLifeCount()) + '0');

	if (baseX + x_offset < MAX_X)
	{
		current_screens[room][y][baseX + x_offset] = new_life_val;
		gotoxy((int)(baseX + x_offset), y);
		cout << new_life_val;
	}
	cout.flush();
}

//////////////////////////////////////////////////////////////////////////////////////////

void Screen::setScoreAt(const Player& my_player1, const Player& my_player2, int room)
{
	if (g_is_silent_mode) return;
	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + SCORE_Y;

	int scoreX = baseX + SCORE_X;

	int totalScore = my_player1.getScore() + my_player2.getScore();
	string s = std::to_string(totalScore);


	for (int i = 0; i < MAX_SCORE_DIGITS; i++)  //we dont have more then 5 nums for score
	{
		if (scoreX + i < MAX_X)
		{
			current_screens[room][y][scoreX + i] = ' ';
		}
	}

	gotoxy(scoreX, y);
	cout << "     ";

	gotoxy(scoreX, y);
	cout << s;

	for (size_t i = 0; i < s.length(); ++i)
	{
		if (scoreX + i < MAX_X)
		{
			current_screens[room][y][scoreX + i] = s[i];
		}
	}

	cout.flush();
}

///////////////////////////////////////////////////////////////////

void Screen::setLevelAt(int room)
{
	if (g_is_silent_mode) return;
	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + LEVEL_Y;

	int levelX = baseX + LEVEL_X;

	for (int i = 0; i < MAX_LEVEL_DIGITS; i++)  //we dont have more then 1,000,000 nums for  
	{
		if (levelX + i < MAX_X)
		{
			current_screens[room][y][levelX + i] = ' ';
		}
	}

	//gotoxy(levelX, y);
	//cout << "     ";

	//gotoxy(levelX, y);
	//cout << s;

	string str_level = std::to_string(room + 1);

	for (size_t i = 0; i < str_level.length(); ++i)
	{
		if (levelX + i < MAX_X)
		{
			current_screens[room][y][levelX + i] = str_level[i];
		}
	}

	cout.flush();
}



/////////////////////////////////////////////////////////////////////////////////////////
void Screen::setMessageAt(int room, const char* new_str)
{
	if (g_is_silent_mode) return;

	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + MESSAGE_Y;

	int i = 0;

	char ch = new_str[i];
	while (ch != '\0')
	{
		if (baseX + MESSAGE_X + i < MAX_X)
		{
			current_screens[room][y][baseX + MESSAGE_X + i] = ch;
			gotoxy(baseX + MESSAGE_X + i, y);
			cout << ch;
		}
		i++;
		ch = new_str[i];
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
void Screen::setHintAt(int room, const char* new_str)
{
	if (g_is_silent_mode) return;

	if (notValidRoom(room)) return;
	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + HINT_Y;

	int i = 0;
	char ch = new_str[i];
	while (ch != '\0')
	{
		if (baseX + HINT_X + i < MAX_X)
		{
			current_screens[room][y][baseX + HINT_X + i] = ch;
			gotoxy(baseX + HINT_X + i, y);
			cout << ch;
		}
		i++;
		ch = new_str[i];
	}
}
/////////////////////////////////////////////////////////////////////////////////////////
void Screen::delMessageAt(int room)
{
	if (g_is_silent_mode) return;

	if (notValidRoom(room)) return;

	size_t baseX = legend_locations[room].getX();
	size_t baseY = legend_locations[room].getY();
	int y = baseY + MESSAGE_Y;

	int i = 0;
	if (baseX + MESSAGE_X >= MAX_X) return;

	char ch = current_screens[room][y][baseX + MESSAGE_X + i];

	while ((ch != ' ') && (ch != '\0'))
	{
		if (baseX + MESSAGE_X + i < MAX_X) {
			current_screens[room][y][baseX + MESSAGE_X + i] = ' ';
			gotoxy(baseX + MESSAGE_X + i, y);
			cout << ' ';

			i++;
			if (baseX + MESSAGE_X + i < MAX_X)
				ch = current_screens[room][y][baseX + MESSAGE_X + i];
			else
				break;
		}
	}
}
////////////////////////////////////////////
void Screen::restart()
{
	current_screens = templates_screens;
}

/////////////////////////////////////////////////////////////////////////////////////////
void Screen::updateLight(int room, bool hasTorch) const
{
	if (g_is_silent_mode) return;

	if (notValidRoom(room)) return;
	if (!dark_rooms[room].isDark) return;//if the room isn't dark, no need to update light

	int x1 = dark_rooms[room].x1;
	int y1 = dark_rooms[room].y1;
	int x2 = dark_rooms[room].x2;
	int y2 = dark_rooms[room].y2;

	for (int y = y1; y <= y2; ++y)  //redraw only the dark area
	{
		for (int x = x1; x <= x2; ++x)
		{
			gotoxy(x, y);

			if (hasTorch)
			{
				cout << current_screens[room][y][x];
			}
			else
			{
				cout << ' ';
			}
		}
	}
	cout.flush();
}

/////////////////////////////////////////

void Screen::addScreenFromFile(const vector<string>& newLevel, const Point& legendLocation)
{
	vector<string> currentLevel = newLevel;
	Point finalLegendPos = legendLocation;

	if (legendLocation.getY() > L_LIMIT)
	{
		finalLegendPos.setY(0);
		finalLegendPos.setX(0);
	}

	while (currentLevel.size() < MAX_Y)
	{
		currentLevel.push_back(string(MAX_X, ' '));
	}

	for (auto& row : currentLevel)
	{
		row.resize(MAX_X, ' ');


		if (row[0] != 'i' && (row[0] < '1' || row[0] > '9') && row[0] != 'H' && row[0] != 'M')
		{
			row[0] = 'i';
		}

		if (row[MAX_X - 1] != 'i' && (row[MAX_X - 1] < '1' || row[MAX_X - 1] > '9'))
		{
			row[MAX_X - 1] = 'i';
		}
	}

	for (int i = 0; i < MAX_X; i++)
	{
		currentLevel[MAX_Y - 1][i] = 'i';
	}

	if (finalLegendPos.getY() > 0)
	{
		for (int j = 0; j < MAX_X; j++)
		{
			currentLevel[0][j] = 'i';
		}
	}

	LoadLegend(currentLevel, finalLegendPos);

	current_screens.push_back(currentLevel);
	templates_screens.push_back(currentLevel);
	legend_locations.push_back(finalLegendPos);
	dark_rooms.push_back({ false, 0,0,0,0 });
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Screen::LoadLegend(vector<string>& currentLevel, const Point& legendLocation)
{
	//int baseX = legendLocation.getX();
	size_t baseX = 0;
	size_t baseY = legendLocation.getY();

	string lines[] = {
		"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",
		"Messages:                                                                  i",
		"Health: $: 3  &: 3     Level: 1        Inventory: $:           &:          i",
		"Hints:                                                  Scores:            i",
		"iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"
	};

	for (int i = 0; i < L_SIZE; ++i)
	{
		int currentY = baseY + i;

		if (currentY >= 0 && currentY < MAX_Y)
		{

			for (size_t j = 0; j < lines[i].size() && (baseX + (int)j) < MAX_X; ++j)
			{
				currentLevel[currentY][baseX + j] = lines[i][j];
			}
		}
	}
}
