#include "Point.h"

//Everything here token from Tirgul
////////////////////////////////////////////////////////////////

void Point::move()
{
	x = x + diff_x;
	y = y + diff_y;
}

//////////////////////////////////////////////////////////////

void Point::setDirection(Direction dir) {
	switch (dir) {
	case Direction::UP:
		diff_x = 0;
		diff_y = -1;
		break;
	case Direction::RIGHT:
		diff_x = 1;
		diff_y = 0;
		break;
	case Direction::DOWN:
		diff_x = 0;
		diff_y = 1;
		break;
	case Direction::LEFT:
		diff_x = -1;
		diff_y = 0;
		break;
	case Direction::STAY:
		diff_x = 0;
		diff_y = 0;
		break;
	}
}
/////////////////////////////////////////////////////////////////////

void Point::setChar(char c)
{
	this->ch = c;
}
////////////////////////////////////////////////////////////////////

Point Point::nextPos() const {

	Point next = *this;

	next.x = next.x + next.diff_x;
	next.y = next.y + next.diff_y;

	return next;
}

//////////////////////////////////////////////////////////UPDATES FROM HERE

Point Point::prevPos() const
{
	Point prev = *this; //                             
	prev.x = prev.x - prev.diff_x;
	prev.y = prev.y - prev.diff_y;
	return prev;
}

///////////////////////////////////////////////////////////

Direction Point::getDirectionFromDiff(int diff_x, int diff_y) const
{
	if (diff_x == 0 && diff_y == -1)
		return Direction::UP;
	else if (diff_x == 1 && diff_y == 0)
		return Direction::RIGHT;
	else if (diff_x == 0 && diff_y == 1)
		return Direction::DOWN;
	else if (diff_x == -1 && diff_y == 0)
		return Direction::LEFT;
	else
		return Direction::STAY;
}
/////////////////////////////////////////////////////////////

void Point::draw(char c) const
{
	if (g_is_silent_mode) return;
	gotoxy(x, y);

	if (g_use_colors)
	{
		setCharColor(c);
		cout << c;
		setTextColor(Color::LIGHTGREY);
	}
	else
	{
		cout << c;
	}

}
