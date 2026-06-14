#include "Obstacle.h"

Obstacle::Obstacle(Screen& screen, const Point& startPos, int room, bool visited[MAX_Y][MAX_X])
    : my_screen(screen)
{
    findAllPoints(startPos, room, visited);
}
///////////////////////////////////////////////////////////
// Find all connected points to form the obstacle 

void Obstacle::findAllPoints(const Point& p, int room, bool visited[MAX_Y][MAX_X])
{
    std::vector<Point> openList; // List of points to explore
    openList.push_back(p);

    while (!openList.empty())
    {
        Point p = openList.back();
        openList.pop_back();

        int x = p.getX();
        int y = p.getY();

        if (x < 0 || x >= MAX_X || y < 0 || y >= MAX_Y) // check bounds
            continue;

        if (visited[y][x]) // check if already visited
            continue;

        char ch = my_screen.getCharAt(p, room);
        if (ch != OBSTACLE_CHAR)
            continue;

        // We found a valid part of the obstacle
        visited[y][x] = true;

        p.setChar(OBSTACLE_CHAR);
        body.push_back(p);

        //add neighboring points to explore
        openList.push_back(Point(x + 1, y, 0, 0, ' ')); // Right
        openList.push_back(Point(x - 1, y, 0, 0, ' ')); // Left
        openList.push_back(Point(x, y + 1, 0, 0, ' ')); // Down
        openList.push_back(Point(x, y - 1, 0, 0, ' ')); // Up
    }
}

/////////////////////////////////////
//Check if a point is part of the obstacle

bool Obstacle::isPartOfObstacle(const Point& targetP) const
{
    for (const Point& p : body)
    {
        if (p.getX() == targetP.getX() && p.getY() == targetP.getY())
        {
            return true;
        }
    }
    return false;
}

/////////////////////////////////////////////////////////////////
// Erase the obstacle from the screen

void Obstacle::eraseFromScreen(int room)
{
    for (const Point& p : body)
    {
        Point toDelete = p;
        toDelete.setChar(' ');
        my_screen.setCharAt(toDelete, room);
        toDelete.draw(' ');
    }
}

///////////////////////////////////////////////////////////////
// Draw the obstacle to the screen

void Obstacle::drawToScreen(int room)
{
    for (const Point& p : body)
    {
        my_screen.setCharAt(p, room);
        p.draw();
    }
}

///////////////////////////////////////////////////////////////
// Push the obstacle in a given direction with a certain force

bool Obstacle::push(Direction dir, int force, int room)
{
    if (force < getSize()) // Not enough force to move the obstacle
        return false;

    int dx = 0, dy = 0;

    switch (dir) // Moving direction
    {
    case Direction::UP:
        dy = -1;
        break;
    case Direction::DOWN:
        dy = 1;
        break;
    case Direction::LEFT:
        dx = -1;
        break;
    case Direction::RIGHT:
        dx = 1;
        break;
    default:
        return false;
    }

    if (isMovementPossible(dir, room, dx, dy) == false) // Check if movement is possible
    {
        return false;
    }

    // Move is valid, update positions

    eraseFromScreen(room); // Remove obstacle from current position

    for (Point& p : body) // Update each point's position
    {
        p.setX(p.getX() + dx);
        p.setY(p.getY() + dy);
    }

    drawToScreen(room); // Draw obstacle at new position

    return true;
}

///////////////////////////////////////////////////////////////
// Check if movement is possible in the given direction
bool Obstacle::isMovementPossible(Direction dir, int room, int dx, int dy)
{
    for (const Point& p : body) // Check if movement is possible
    {

        Point nextPos(p.getX() + dx, p.getY() + dy, 0, 0, ' '); // Next position after move

        if (nextPos.getX() < 0 || nextPos.getX() >= MAX_X || nextPos.getY() < 0 || nextPos.getY() >= MAX_Y) // Out of bounds
            return false;

        char charAtNewPos = my_screen.getCharAt(nextPos, room);

        if (charAtNewPos != ' ' && !isPartOfObstacle(nextPos)) // if the next position is not empty and not part of the obstacle cannot move
        {
            return false;
        }
    }
    return true;
}