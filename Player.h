#include "Player.h"
#include "Door.h"   //forward declaration
#include "riddle.h"
#include "Spring.h"

///////////////////////////////////////////////////////////////////
// Constructor
Player::Player(const Point& point, const char(&the_keys)[NUM_KEYS + 1], Screen& theScreen, char symbol, Bomb& bomb_ref, Spring& spring, int& current_room, Door* current_room_door, riddle& riddle_cla)
	: location(point), screen(theScreen), character(symbol), active_bomb(bomb_ref), my_spring(spring), pDoor(current_room_door), current_room_index(current_room), my_riddle_cla(riddle_cla)
{

	memcpy(keys, the_keys, NUM_KEYS * sizeof(keys[0]));
	location.setChar(symbol);
	setHeldItem(ItemType::NONE);
}
/////////////////////////////////////////////////////////////////
// Draw the player on the screen
void Player::draw()
{
	if (!is_finished_level) // draw only if the player has not finished the level
	{
		location.draw();
	}
}
/////////////////////////////////////////////////////////////////
////// Process keyboard input
void Player::handleKeyPressed(char key_pressed)
{
	size_t index = 0;
	for (char k : keys)
	{
		if (tolower(k) == tolower(key_pressed))
		{
			if (index < MOVE_KEYS_COUNT)
			{
				Direction new_dir = (Direction)index;

				if (my_spring.getIfStart()) //Pushed By spring
				{
					Direction springDir = my_spring.getSpringDirection();

					if (isOpposite(new_dir, springDir)) //Ignore Direction that should not happen
						return;

					if (new_dir == Direction::STAY)
						return;

					if (new_dir == springDir)
						return;

					location.setDirection(new_dir);
				}
				else
				{
					if (my_spring.getCounterChar() >= 1)
					{
						Direction current_dir = location.getDirectionFromDiff(location.getdiffX(), location.getdiffY());

						if (new_dir == current_dir)
						{
							return;
						}
					}

					location.setDirection(new_dir);

					if (my_spring.getCounterChar() >= 1)
					{
						my_spring.startSpringFunc(*this, current_room_index);
					}
				}
			}
			else
			{
				disposeItem();
			}
			return;
		}
		++index;
	}
}
/////////////////////////////////////////////////////////////////////
// Interaction logic
void Player::handleCollision(int component_char)
{
	Point next_pos = location.nextPos();
	switch (component_char) //based on the char on screen
	{

	case WALL_CHAR:
	case IMMUTABLE_WALL_CHAR:
		if (my_spring.getCounterChar() >= 1 && !my_spring.getIfStart())
		{
			my_spring.startSpringFunc(*this, current_room_index);
		}

		else if (my_spring.getIfStart())
		{
			my_spring.resetPower(*this);
			location.setDirection(Direction::STAY);
		}

		else
		{
			location.setDirection(Direction::STAY);
		}
		break;

	case SWITCH_ON_CHAR:
	case SWITCH_OFF_CHAR:
		screen.setSwitch(next_pos, current_room_index);
		break;

	case RIDDLE_CHAR: // (?)
	{
		location.setDirection(Direction::STAY);
		my_riddle_cla.run(next_pos, current_room_index);

		last_riddle_log = my_riddle_cla.getLastLog(); //this is the string report     
		riddle_counter++;   

		bool riddle_solved = my_riddle_cla.getIfSolved();

		if (riddle_solved)
		{
			increaseScore();
			screen.setScoreAt(*this, *partner, current_room_index);
		}

		break;
	}

	case BOMB_CHAR: // (@)
		if (active_bomb.isActive()) //// Cannot pick up a bomb if one is currently active
		{
			return;
		}

		if (getHeldItem() != ItemType::NONE) //// Swap items if already holding something
		{
			disposeItem();
		}

		setHeldItem(ItemType::BOMB);
		screen.setInventoryAt(*this, current_room_index);

		// Remove item from map
		next_pos.setChar(' ');
		screen.setCharAt(next_pos, current_room_index);
		break;

	case KEY_CHAR:  // (K)

		if (getHeldItem() != ItemType::NONE)
		{
			disposeItem();
		}

		setHeldItem(ItemType::KEY);
		pDoor->AddThisKey();
		screen.setInventoryAt(*this, current_room_index);

		// Remove item from map
		next_pos.setChar(' ');
		screen.setCharAt(next_pos, current_room_index);

		break;

	case OBSTACLE_CHAR:  // (*)
	{
		handleObstacle();
		break;
	}

	case TORCH_CHAR:  // (!)

		if (getHeldItem() != ItemType::NONE) //// Swap items if already holding something
		{
			disposeItem();
		}

		setHeldItem(ItemType::TORCH);
		screen.setInventoryAt(*this, current_room_index);

		// Remove item from map
		next_pos.setChar(' ');
		screen.setCharAt(next_pos, current_room_index);
		break;

	case PLAYER1_CHAR:
	case PLAYER2_CHAR:
		if (partner != nullptr && component_char == partner->getCharacter())
		{
			if (this->speed > 1)
			{
				Direction impactDir = location.getDirectionFromDiff(location.getdiffX(), location.getdiffY());

				partner->getSpring().triggerExternalForce(*partner, this->speed, impactDir);

				partner->setDirection(impactDir);

				return;
			}

		}
		break;

	case SPRING_CHAR:  // (#)                                                    

		if (my_spring.collectChars(*this, next_pos, current_room_index) == 0) //we get # point  
		{
			location.setDirection(Direction::STAY);
		}

		if (my_spring.getCounterChar() >= 1)
		{
			if (my_spring.getOpositeDirection() != location.getDirectionFromDiff(location.getdiffX(), location.getdiffY()))
			{
				my_spring.startSpringFunc(*this, current_room_index);
			}
		}
		break;

	case LIFE_GIFT_CHAR: // (+) //Life gift

		if (life_count < 3)
		{
			life_count++;
			screen.setHealthAt(*this, current_room_index);
		}

		next_pos.setChar(' ');
		screen.setCharAt(next_pos, current_room_index);
		break;


	default:
		// Pass through empty space or unhandled objects
		break;

	}
}
/////////////////////////////////////////////////////////////////////////
// Door interaction logic
void Player::handleDoor(char char_on_screen)
{
	if (pDoor == nullptr)
	{
		return;
	}

	if ((char_on_screen - '0') != (pDoor->getNumDoor()))   
	{
		return;
	}

	if (my_spring.getCounterChar() >= 1)  //if we have power of spring we nwwd to stop and reset     
	{ //power of spring stop when player pass to next room
		my_spring.resetPower(*this);
	}

	if (pDoor->isDoorOpen())  //// Case 1: Door is already open
	{
		location.draw(char_under_player);
		char_under_player = char_on_screen;
		location.move();
		location.draw();

		std::cout.flush();

		setNewLevel();

		pDoor->changeRoom(*this, current_room_index);
		char_under_player = ' ';
		return;
	}

	else if (getHeldItem() == ItemType::KEY && (pDoor->getItsAMatch()) == 1)   //// Case 2: Player has the correct key
	{
		pDoor->openDoor(*this, current_room_index);
		setHeldItem(ItemType::NONE);

		location.draw(char_under_player);
		char_under_player = char_on_screen;
		location.move();
		location.draw();

		cout.flush();
		Sleep(400); // Visual delay for transition

		setNewLevel();

		pDoor->changeRoom(*this, current_room_index);
		char_under_player = ' ';
		return;
	}
	//// Case 3: Door opened via remote switch
	else if ((pDoor->getifSwitch() == 1))
	{
		pDoor->itsASwitchMatch(current_room_index);
		if (pDoor->getItsAMatch() == 1)   //if door open with switch
		{
			pDoor->openDoor(current_room_index);
			location.draw(char_under_player);
			char_under_player = char_on_screen;
			location.move();
			location.draw();

			cout.flush();
			Sleep(400);

			setNewLevel();

			pDoor->changeRoom(*this, current_room_index);
			char_under_player = ' ';
			return;

		}
	}
	else ////Case 4: Door is locked

	{
		location.setDirection(Direction::STAY);
		return;
	}

}
//////////////////////////////////////////////////////////////////////
// Item management
void Player::setHeldItem(ItemType type)
{
	this->held_item = type;
}
/////////////////////////////////////////////////////////////////////////
// Dispose of the currently held item
void Player::disposeItem()
{
	if (getHeldItem() == ItemType::NONE)
	{
		return; // the player is not holding any item
	}

	Point drop_location = location.prevPos();
	char disposed_char = ' ';

	switch (held_item)
	{
	case ItemType::BOMB:
		active_bomb.activate(drop_location, current_room_index);
		disposed_char = BOMB_CHAR; // '@'
		break;

	case ItemType::KEY:
		disposed_char = KEY_CHAR; // 'K'

		//                              
		if (pDoor != nullptr) {
			pDoor->DeleteThisKey();
		}
		break;

	case ItemType::TORCH:
		disposed_char = TORCH_CHAR; // '!'
		break;

	default:
		break;
	}
	//// Place the item back on the map visually and logically
	if (disposed_char != ' ')
	{
		drop_location.setChar(disposed_char);
		screen.setCharAt(drop_location, current_room_index);
		drop_location.draw();
	}

	screen.delInventoryAt(*this, current_room_index);
	setHeldItem(ItemType::NONE);


}
/////////////////////////////////////////////////////////////////////////
void Player::move()
{
	if (is_finished_level)
	{
		location.setDirection(Direction::STAY);
		return;
	}

	if (my_spring.getIfStart()) //Player is effected by spring
	{
		char_under_player = screen.getCharAt(location, current_room_index);
		Direction userWantedDir = location.getDirectionFromDiff(location.getdiffX(), location.getdiffY());
		Direction springDir = my_spring.getSpringDirection();

		location.setDirection(springDir);

		for (int i = 0; i < speed; i++)
		{
			moveOneStep();

			if (!my_spring.getIfStart())
				break;
		}

		if (my_spring.getIfStart()) //Still effected by spring
		{
			location.setDirection(userWantedDir);

			bool isLateral = (userWantedDir != Direction::STAY) && (userWantedDir != springDir) && !isOpposite(userWantedDir, springDir);

			if (isLateral)
			{
				moveOneStep();
			}
		}
	}

	else
	{
		for (int i = 0; i < speed; i++)
		{
			moveOneStep();
			if (speed == 1 && i > 0)
				break;
		}
	}
}
///////////////////////////////////////////////////////////////////////////
void Player::moveOneStep()
{
	Point next_pos = location.nextPos();

	if (next_pos.getX() < 0 || next_pos.getX() >= MAX_X || next_pos.getY() < 0 || next_pos.getY() >= MAX_Y)
	{
		if (my_spring.getIfStart())
		{
			my_spring.resetPower(*this);
		}
		location.setDirection(Direction::STAY);
		return;
	}
	char char_on_screen = screen.getCharAt(next_pos, current_room_index);

	if (isdigit(char_on_screen)) // ãìú
	{
		handleDoor(char_on_screen);
		return;
	}
	//colision between players!
	if (partner != nullptr && partner->isAlive() &&
		partner->getCurrentRoomIndex() == current_room_index &&
		next_pos.getX() == partner->getLocation().getX() &&
		next_pos.getY() == partner->getLocation().getY())
	{
		char_on_screen = partner->getCharacter();
	}

	handleCollision(char_on_screen);

	char updated_char = screen.getCharAt(location.nextPos(), current_room_index);

	if (updated_char == OBSTACLE_CHAR)
		return;

	if (location.getdiffX() != 0 || location.getdiffY() != 0)
	{
		location.draw(char_under_player);

		if (updated_char == SWITCH_ON_CHAR || updated_char == SWITCH_OFF_CHAR || updated_char == SPRING_CHAR)
			char_under_player = updated_char;
		else
			char_under_player = ' ';

		location.move();
		location.draw();
	}
}

////////////////////////////////////////////////////////////////////////////
// Visual toggles
void Player::disappear()
{
	char background_char = screen.getCharAt(location, current_room_index);
	location.draw(background_char);
	this->is_finished_level = true;

}
////////////////////////////////////////////////////////////////////////
void Player::appear()
{
	this->is_finished_level = false;
	location.draw();
}
/////////////////////////////////////////////////////////////////////
// Reset player position and state for level restart
void Player::resetLevelpos(int x, int y)
{
	location.setX(x);
	location.setY(y);
	location.setDirection(Direction::STAY);

	char_under_player = screen.getCharAt(location, current_room_index);
	appear();
}
//////////////////////////////////////////////////////////////////////
void Player::handleObstacle()
{
	if (my_obstacles == nullptr)
	{
		return; // No obstacles to interact with
	}
	Point next_pos = location.nextPos();
	int force = manageForce();

	for (Obstacle& obs : *my_obstacles)
	{
		if (obs.isPartOfObstacle(next_pos))
		{
			obs.push(location.getDirectionFromDiff(location.getdiffX(), location.getdiffY()), force, current_room_index);
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// Calculate total force considering partner's assistance
int Player::manageForce()
{
	int totalForce = speed; //add to force the speed of player

	if (partner == nullptr || !partner->isAlive()) //no partner or partner is dead
	{
		return totalForce;
	}

	Point partnerPos = partner->getLocation();

	bool sameDirection = (location.getDirectionFromDiff(location.getdiffX(), location.getdiffY()) == partnerPos.getDirectionFromDiff(partnerPos.getdiffX(), partnerPos.getdiffY()));
	if (sameDirection)
	{
		int distX = abs(location.getX() - partnerPos.getX());
		int distY = abs(location.getY() - partnerPos.getY());

		if (distX + distY == 0) //Same position
		{
			totalForce += partner->getSpeed();
		}

		else if (distX + distY == 1) //Adjacent positions
		{
			Point partnerNext = partnerPos.nextPos();
			char partnerHit = screen.getCharAt(partnerNext, current_room_index); //char at next pos of partner

			if (partnerHit == OBSTACLE_CHAR) //partner is pushing too
			{
				totalForce += partner->getSpeed();
			}
		}
	}

	return totalForce;

}


//////////////////////////////////////////////////////////////////////////////////////////// 
void Player::resetSpring(Player& my_player, Point& my_location)
//function for reset spring if we arriving to door ar wall
{

	if (my_spring.getIfStart() != 0)
	{
		my_spring.resetPower(my_player);
		location.setDirection(Direction::STAY);  //you need to stop player
	}

	else
	{
		my_spring.startSpringFunc(*this, current_room_index);
	}
}

//////////////////////////////////////////////////////////////////////////////////

bool Player::isOpposite(Direction dir1, Direction dir2) const
{
	if (dir1 == Direction::UP && dir2 == Direction::DOWN) return true;
	if (dir1 == Direction::DOWN && dir2 == Direction::UP) return true;
	if (dir1 == Direction::LEFT && dir2 == Direction::RIGHT) return true;
	if (dir1 == Direction::RIGHT && dir2 == Direction::LEFT) return true;
	return false;
}