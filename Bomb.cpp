#include "Bomb.h"
#include "Player.h"
/////////////////////////////////////////////////////////////
//activate the bomb at position (x,y)
void Bomb::activate(const Point& drop_location, int room)
{
    location.setX(drop_location.getX());
    location.setY(drop_location.getY());

    room_index = room;
    is_active = true;
    countdown = 5; // 5 game loops until explosion
    location.draw();
}
//////////////////////////////////////////////////////////////
void Bomb::update(Player& p1, Player& p2) {
    if (!is_active) //if the bomb is not active, do nothing
    {
        return;
    }

    if (countdown > 0) // every game loop, decrease countdown
    {
        countdown--;
    }

    if (countdown == 0) // Trigger explosion when countdown hits zero
    {
        explode(p1, p2); //boom!
        is_active = false; // the bomb is no longer active
    }
}

void Bomb::explode(Player& p1, Player& p2)
{
    location.draw(' '); // erase the bomb from the screen
    Point current_pos = location;
    current_pos.setChar(' ');
    s.setCharAt(current_pos, room_index);

    checkDamagePlayer(p1, room_index);
    checkDamagePlayer(p2, room_index);

    // Check a 7x7 area centered on the bomb's location
    for (int y = -EXPLOSION_RADIUS; y <= EXPLOSION_RADIUS; y++) {
        for (int x = -EXPLOSION_RADIUS; x <= EXPLOSION_RADIUS; x++) {

            Point target_pos = location;
            target_pos.setX(location.getX() + x);
            target_pos.setY(location.getY() + y);

            // Check bounds
            if (target_pos.getX() < 0 || target_pos.getX() >= MAX_X ||
                target_pos.getY() < 0 || target_pos.getY() >= MAX_Y)
            {
                continue;
            }

            char char_at_target = s.getCharAt(target_pos, room_index);

            //destroy:

            if (char_at_target == WALL_CHAR)
            {
                target_pos.setChar(' ');
                s.setCharAt(target_pos, room_index);
                target_pos.draw(' ');
            }


        }
    }

}

void Bomb::checkDamagePlayer(Player& p, int room)
{
    if (p.isAlive() && p.getCurrentRoomIndex() == room)
    {
        int dx = abs(p.getLocation().getX() - location.getX());
        int dy = abs(p.getLocation().getY() - location.getY());

        if (dx <= 3 && dy <= 3)
        {
            p.hitByBomb(); //Reduce player's life by one
            s.setHealthAt(p, room); // Update health display
        }
    }

}
