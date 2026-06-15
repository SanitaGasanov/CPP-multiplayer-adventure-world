#include "Spring.h"
////////////////////////////////////////////////////
#include "Player.h"   //forward declaration

//////////////////////////////////////////////////////////////////////////////////////

Spring::Spring(Screen& screen) : my_screen(screen)
{
	counter_char_of_spring = 0;
	my_power = 0;
	start_spring = false;
	game_cycles = 0;

	my_oposite_direction = Direction::STAY;
	my_spring_direction = Direction::STAY;
}

//////////////////////////////////////////////////////////////////////////////////////

void Spring::resetPower(Player& my_player)
{
	my_power = 0;          //set in spring
	my_player.setSpeed(1); //set in player
	ChangeCycles();        //calling to func to update steps  0*0= 0
	counter_char_of_spring = 0;
	start_spring = 0;
}

//////////////////////////////////////////////////////////////////////////////////////

Direction Spring::updateSpringDirection() const
{
	if (my_oposite_direction == Direction::UP)
	{
		return Direction::DOWN;
	}

	else if (my_oposite_direction == Direction::DOWN)
	{
		return Direction::UP;
	}

	else if (my_oposite_direction == Direction::LEFT)
	{
		return Direction::RIGHT;
	}

	else if (my_oposite_direction == Direction::RIGHT)
	{
		return Direction::LEFT;
	}

	return Direction::STAY;
}
/////////////////////////////////////////////////////////////////////////////
bool Spring::ifItsLegalAccess(Point& my_next_point, int current_room_index)
{
	// äëéååï ùáå äùç÷ï îðñä ìäéëðñ ì÷ôéõ
	Direction my_direction = my_next_point.getDirectionFromDiff(
		my_next_point.getdiffX(),
		my_next_point.getdiffY()
	);

	Point check_pos = my_next_point;
	char char_at_pos = my_screen.getCharAt(check_pos, current_room_index);

	// ðîùéê ìáãå÷ ÷ãéîä ëì òåã àðçðå òì ÷ôéöéí
	while (char_at_pos == SPRING_CHAR) {
		check_pos.setDirection(my_direction);
		check_pos = check_pos.nextPos();
		char_at_pos = my_screen.getCharAt(check_pos, current_room_index);
	}

	// àí áñåó øöó ä÷ôéöéí äâòðå ì÷éø - äàéñåó çå÷é!
	if (char_at_pos == WALL_CHAR || char_at_pos == IMMUTABLE_WALL_CHAR) {
		if (my_direction != my_oposite_direction) {
			my_oposite_direction = my_direction;
			my_spring_direction = updateSpringDirection();
		}
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////

bool Spring::collectChars(Player& my_player, Point& my_next_point, int current_room)
{
	if (ifItsLegalAccess(my_next_point, current_room))
	{
		my_next_point.setChar(' ');
		counter_char_of_spring++;
		if (counter_char_of_spring == 1)
		{
			point_of_start = my_next_point.prevPos();
		}
		my_screen.setCharAt(my_next_point, current_room);
		return true;
	}
	return false;
}
//////////////////////////////////////////////////////////////////////////////////////

void Spring::update(Player& my_player)
{
	if ((start_spring != 0) && (game_cycles > 0))
	{
		game_cycles--;
		if (game_cycles <= 0)
		{
			resetPower(my_player);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////////
// 
// overloading functions!!!!!!!
void Spring::SetPower(Player& my_player)
{
	my_power = counter_char_of_spring;          //set in spring
	my_player.setSpeed(counter_char_of_spring);//set in player
	ChangeCycles();        //calling to func to update steps
}
/////////
void Spring::SetPower(Player& my_player, int power)
{
	my_power = power;          //set in spring
	my_player.setSpeed(power);//set in player
	ChangeCycles();        //calling to func to update steps
}

//////////////////////////////////////////////////////////////////////////////////////



void Spring::drawChars(Player& my_player, int room_index)
{
	Point temp_point = point_of_start;
	int dx = 0;
	int dy = 0;

	switch (my_oposite_direction)
	{
	case Direction::UP:    dy = -1; break;
	case Direction::DOWN:  dy = 1;  break;
	case Direction::LEFT:  dx = -1; break;
	case Direction::RIGHT: dx = 1;  break;
	default: break;
	}

	for (int i = 0; i < counter_char_of_spring; i++)
	{
		temp_point.setX(temp_point.getX() + dx);
		temp_point.setY(temp_point.getY() + dy);

		char char_at_dest = my_screen.getCharAt(temp_point, room_index);

		if (char_at_dest == ' ' || char_at_dest == SPRING_CHAR || char_at_dest == my_player.getCharacter())
		{
			temp_point.setChar(SPRING_CHAR);
			my_screen.setCharAt(temp_point, room_index);

			if (char_at_dest != my_player.getCharacter())
			{
				temp_point.draw();
			}
		}
	}
	std::cout.flush();
}
//////////////////////////////////////////////////////////////////////////////////////

void Spring::startSpringFunc(Player& my_player, int room_index)          //if we brake or move
{
	SetPower(my_player);  //update all parameters
	start_spring = 1;////////////
	my_player.setDirection(my_spring_direction);
	drawChars(my_player, room_index);

}

//////////////////////////////////////////////

void Spring::triggerExternalForce(Player& my_player, int speed, Direction dir)
{

	my_power = speed;
	my_player.setSpeed(speed);
	ChangeCycles();

	my_spring_direction = dir;

	start_spring = true;
}
