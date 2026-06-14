#pragma once

////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
/////////////////////////////////////////////////

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::vector;
/////////////////////////////////////////////////
enum class GameEventType 
{
	LEVEL_CHANGE,

	PLAYER_1_DEAD,
	PLAYER_2_DEAD,

	PLAYER_1_LOST_HEALTH,
	PLAYER_2_LOST_HEALTH,

	PLAYER_1_ADD_HEALTH,
	PLAYER_2_ADD_HEALTH,

	PLAYER_1_PASSED_LEVEL,
	PLAYER_2_PASSED_LEVEL,

	GAME_ENDED,
	GAME_STARTED,

	RIDDLE_ANSWER
};

//////////////////////////////////////////////////

enum ScreensXY
{
	INVENTORY_X_P1 = 52,   //P1= $

	INVENTORY_X_P2 = 65, //P2= &

	INVENTORY_Y = 2,

	HEALTH_Y = 2,

	HEALTH_X_P1 = 11,   //P1= $

	HEALTH_X_P2 = 17, //P2= &

	SCORE_Y = 3,

	SCORE_X = 68,   //P1= $ and  P2= &

	MESSAGE_Y = 1,

	MESSAGE_X = 10,   //P1= $ and  P2= &

	HINT_Y = 3,

	HINT_X = 7,   //P1= $ and  P2= &

	L_LIMIT = 19,  //max legal line ( X ) for L in screen

	L_SIZE = 5,  //L total numbers os lines

	LEVEL_Y = 2,   //sssd

	LEVEL_X = 30,   //P1= $ and  P2= & //sssd

	MAX_LEVEL_DIGITS = 6,

	MAX_SCORE_DIGITS = 5

};
//////////////////////////////////////////////////





enum class Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT,
	STAY

}; //up=0 right=1..... stay=4

//////////////////////////////////////////////////////////
enum { MAX_X = 80, MAX_Y = 25 };

///////////////////////////////////////////////////////////

enum special_nums_for_menu { NINE = 57, ONE = 49, TWO = 50, EIGHT = 56 };  /// the value in ascii   //colors

///////////////////////////////////////////////////////////
enum special_nums_for_riddle { Three = 51, FOUR = 52 };  /// the value in ascii   
///////////////////////////////////////////////////////////
enum special_nums_for_game { ESC = 27, H = 72, h = 104 };  /// the value in ascii  

////////////////////////////////////////////////////////////

enum class ItemType
{
	NONE,
	KEY,
	BOMB,
	RIDDLE,
	SWITCH_ON,
	SWITCH_OFF,
	TORCH,
	SPRING
};

///////////////////////////////////////////////////////
// Define characters used in the game


inline const char RIDDLE_CHAR = '?';
inline const char BOMB_CHAR = '@';
inline const char KEY_CHAR = 'K';
inline const char SWITCH_ON_CHAR = '/';
inline const char SWITCH_OFF_CHAR = '\\';
inline const char IMMUTABLE_WALL_CHAR = 'i';
inline const char WALL_CHAR = 'w';
inline const char OBSTACLE_CHAR = '*';
inline const char TORCH_CHAR = '!';
inline const char SPRING_CHAR = '#';
inline const char PLAYER1_CHAR = '$';
inline const char PLAYER2_CHAR = '&';
inline const char LIFE_GIFT_CHAR = '+';

///////////////////////////////////////////////////////
const int NUM_KEYS = 6;  //keys for up, right, down, left, stay, dispose
const int MOVE_KEYS_COUNT = 5; //number of movement keys (excluding dispose key)

////////////////////////////////////////////////////////

constexpr int DOORS = 3;

////////////////////////////////////////////////////////

enum BombSettings
{
	EXPLOSION_RADIUS = 3,
	EXPLODE_TIMER = 5
};

/////////////////////////////////////////////////////

enum class Color
{
	BLACK = 0,
	BLUE = 1,
	GREEN = 2,
	CYAN = 3,
	RED = 4,
	MAGENTA = 5,
	BROWN = 6,
	LIGHTGREY = 7,
	DARKGREY = 8,
	LIGHTBLUE = 9,
	LIGHTGREEN = 10,
	LIGHTCYAN = 11,
	LIGHTRED = 12,
	LIGHTMAGENTA = 13,
	YELLOW = 14,
	WHITE = 15
};