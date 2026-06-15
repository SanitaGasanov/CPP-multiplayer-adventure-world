#pragma once

#include "Point.h"
#include "Screen.h"
class Player;


class Bomb {
private:
    Point location;
    int countdown = EXPLODE_TIMER;     // Timer in game loops (ticks)
    bool is_active = false;// Flag to indicate if the bomb is active
    int room_index = 0;
    Screen& s;


public:
    /////////////////////////////////////////////////////////
    //ctor
    Bomb(const Point& p, Screen& screen) :location(p), s(screen), countdown(5), is_active(false), room_index(0)
    {
        location.setChar(BOMB_CHAR); // '@'
    }
    ////////////////////////////////////////////////////////
    //draw the bomb on the screen
    void draw() const
    {
        location.draw();
    }
    ///////////////////////////////////////////////////////
    //activate the bomb at position (x,y)
    void activate(const Point& drop_location, int room); //activate the bomb at position (x,y)
    ////////////////////////////////////////////////////////
    //update the bomb status
    void update(Player& p1, Player& p2);
    ///////////////////////////////////////////////////////
    // check if the bomb is active
    bool isActive() const { return is_active; }
    /////////////////////////////////////////////////////////
    // get the bomb location
    Point getLocation() const { return location; }

private:
    /////////////////////////////////////////////////////////
    // explode the bomb
    void explode(Player& p1, Player& p2);
    void checkDamagePlayer(Player& p, int room);
};
