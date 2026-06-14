#include "Door.h"

////////////////////////////////////////////////////////////////


Door::Door(Screen& screen, int my_door_num, bool if_need_a_key, bool if_need_a_switch, int how_many_keys, int how_many_switches, const char* solu, Point p)
	: my_screen(screen), need_key(if_need_a_key), num_of_door(my_door_num), need_switch(if_need_a_switch), num_of_key(how_many_keys), num_of_switch(how_many_switches), solution(solu), first_point_of_switches(p)
{

}



//////////////////////////////////////////////////////////////////

void Door::openDoor(Player my_player, int room)
{
	door_is_open = 1;

	my_screen.delInventoryAt(my_player, room);
	my_screen.delMessageAt(room);
	const char* my_str = "Next_door_is_open,_and_you_can_pass_through.";   
	my_screen.setMessageAt(room, my_str);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void Door::openDoor(int room)
{
	door_is_open = 1;

	my_screen.delMessageAt(room);
	const char* my_str = "Next_door_is_open,_and_you_can_pass_through.";   
	my_screen.setMessageAt(room, my_str);
}

/////////////////////////////////////////////////////////////////////////////
void Door::itsASwitchMatch(int my_room) //////////////////////////////////////////////////////////////
{
	Point temp = first_point_of_switches;
	for (size_t i = 0; i < num_of_switch; i++)
	{
		char char_on_screen = my_screen.getCharAt(temp, my_room);
		if (char_on_screen != solution[i])
		{
			itsAMatch = 0;
			return;
		}
		temp.setX(temp.getX() + 2);
	}
	itsAMatch = 1;
}

//////////////////////////////////////////////////////////////////////////////

void Door::changeRoom(Player& my_character, int& room)   //calling only if door is open!!!!
{
	this->addOneToCounter();   //if player standing on the door 

	if ((this->showCounter()) % 2 == 0)  //if 2 players passed

	{
		my_screen.delMessageAt(room);  //when we pass a room message will delete

		if (room < (this->getNumDoor()))  //because if we pass to next room  door num is more then room index num
		{
			room++;
			my_character.disappear();
			//my_screen.draw(room);
			///players need show on

		}

	}
	else
	{
		my_character.disappear();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
void Door::ResetDoor()
{
	counter = 0;
	door_is_open = 0;
	collect_all_key = 0;
	counter_of_keys = 0;
	itsAMatch = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////

void Door::configure(int keysNumber, int switchesNumber, const string& code, int x_switch, int y_switch, int id)
{
	this->ResetDoor();  //counter of keys, counter, collect all keys

	this->num_of_key = keysNumber;
	if (keysNumber > 0)
	{
		this->need_key = 1;
	}
	else
	{
		this->need_key = 0;
	}


	this->num_of_switch = switchesNumber;
	if (switchesNumber > 0)
	{
		this->need_switch = 1;

		const char* my_code = code.c_str();
		this->solution = my_code;



		Point my_switch_location(x_switch, y_switch, 0, 0, '\\'); //every switch start from off.
		this->first_point_of_switches = my_switch_location;
	}
	else
	{
		this->need_switch = 1;
		this->solution = "";
	}

	this->num_of_door = id;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////