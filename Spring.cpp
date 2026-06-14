#pragma once
///////////////////////////////////////////////
#include "Point.h" 
#include "Enums.h"
#include "utils.h"

class Player;

#include <cstring>  

////////////////////////////////////////////////////

class Screen
{
	vector<vector<string>> current_screens;
	vector<vector<string>> templates_screens;

	struct DarkRoom  //to know which room is dark
	{
		bool isDark = false;
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0; //to know the bounds of the dark room (x1,y1,x2,y2)
	};

	vector<DarkRoom> dark_rooms;
	vector<Point> legend_locations;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
public:


	Screen() = default;  /// default constructor

	bool notValidRoom(int room) const
	{
		if (room < 0 || room >= current_screens.size())
			return true;
		return false;
	}

	////////////////////////////////////////////
	void addScreenFromFile(const vector<string>& newLevel, const Point& legendLoc); //Add Screen from file

	////////////////////////////////////////////
	void draw(int room, bool hasTorch = false) const; 

	////////////////////////////////////////////

	char getCharAt(const Point& p, int room) const  //const in right side tell that we dont change the  classs members in this file class.
	{
		if (notValidRoom(room)) return ' ';
		if (p.getY() < 0 || p.getY() >= MAX_Y || p.getX() < 0 || p.getX() >= MAX_X) return ' ';
		return current_screens[room][p.getY()][p.getX()];
	}
	////////////////////////////////////////////

	void setCharAt(const Point& p, int room)  //function to change the char in the current screen
	{
		if (notValidRoom(room)) return;
		if (p.getY() < 0 || p.getY() >= MAX_Y || p.getX() < 0 || p.getX() >= MAX_X) return;
		current_screens[room][p.getY()][p.getX()] = p.getChar();
	}
	//////////////////////////////////////////////

	void setHealthAt(const Player& my_player, int room);

	/////////////////////////////////////////////////

	void setInventoryAt(const Player& my_player, int room);

	//////////////////////////////////////////////////

	void delInventoryAt(const Player& my_player, int room);

	/////////////////////////////////////////////////

	void setMessageAt(int room, const char* new_str); 
	//////////////////////////////////////////////////
	void delMessageAt(int room);

	///////////////////////////////////////////////////

	void setHintAt(int room, const char* new_str); 

	//////////////////////////////////////////////////

	void setSwitch(Point& place, int index_room)
	{
		char current_char_on_screen = getCharAt(place, index_room);

		if (current_char_on_screen == SWITCH_ON_CHAR)
		{
			place.setChar(SWITCH_OFF_CHAR);
		}
		else
		{
			place.setChar(SWITCH_ON_CHAR);
		}
		setCharAt(place, index_room);
	}

	///////////////////////////////////////////////////

	void restart();

	////////////////////////////////////////////////////
	//function to set darkness in room
	void setDarkness(int room, int x1, int y1, int x2, int y2)
	{
		dark_rooms[room].isDark = true;
		dark_rooms[room].x1 = x1;
		dark_rooms[room].y1 = y1;
		dark_rooms[room].x2 = x2;
		dark_rooms[room].y2 = y2;
	}
	////////////////////////////////////////////////////////////////////////////
	void updateLight(int room, bool hasTorch) const;

	int getNumLevels() const
	{
		return (int)current_screens.size();
	}

	void LoadLegend(vector<string>& currentLevel, const Point& legendLocation);

	///////////////////////////////////////////////////////////////////////////////////

	void setScoreAt(const Player& my_player1, const Player& my_player2, int room);
	
	void setLevelAt(int room);
};