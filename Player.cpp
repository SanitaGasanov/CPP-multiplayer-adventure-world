#pragma once
#include <vector>
#include "Point.h"  
#include "screen.h"

class Obstacle
{
private:
    std::vector<Point> body; // All points that make up the obstacle
    Screen& my_screen;

    void findAllPoints(const Point& p, int room, bool visited[MAX_Y][MAX_X]);
    void eraseFromScreen(int room);
    void drawToScreen(int room);

public:
    Obstacle(Screen& screen, const Point& p, int room, bool visited[MAX_Y][MAX_X]);

    bool push(Direction dir, int force, int room);

    bool isMovementPossible(Direction dir, int room, int dx, int dy);

    int getSize() const
    {
        return (int)body.size();
    }

    bool isPartOfObstacle(const Point& p) const;

};