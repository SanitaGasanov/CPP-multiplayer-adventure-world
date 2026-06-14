#pragma once

////////////////////////////////////////////////

#include "Point.h"
#include "Enums.h"
#include "Bomb.h"
#include "Screen.h"
#include "Obstacle.h"


class Screen;
class Door;
class riddle;
class Spring;

////////////////////////////////////////////////

class Player {


    Point location;         // Current position on the screen
    char keys[NUM_KEYS];    // Movement and action keys
    Screen& screen;
    Bomb& active_bomb;
    Spring& my_spring;
    const char character;   // Visual representation ('$' or '&')
    ItemType held_item;     // Currently equipped item
    int& current_room_index; // Reference to the global room index
    int my_new_level = 0;
    bool is_finished_level = false; // indicates if the player has finished the level
    int life_count = 3; // number of lives the player has
    Door* pDoor;
    riddle& my_riddle_cla;
    int speed = 1; // Player's movement speed
    int Score = 0; // Player's score
    Player* partner = nullptr; // Reference to the other player
    std::vector<Obstacle>* my_obstacles = nullptr; // Pointer to the obstacles in the current level

    std::string last_riddle_log = ""; //for report 
    int riddle_counter = 0; 

private:
    char char_under_player = ' ';

    //////////////////////////////////////////////////////////////////

public:
    //////////////////////////////////////////////
    // Constructor
    Player(const Point& point, const char(&the_keys)[NUM_KEYS + 1], Screen& theScreen, char character, Bomb& bomb_ref, Spring& spring, int& current_room, Door* current_room_door, riddle& riddle_cla);
    ///////////////////////////////////////////////////////////
    // Draw the player on the screen
    void draw();
    ///////////////////////////////////////////////////////////
    // Core movement logic based on input
    void move();
    ///////////////////////////////////////////////////////////
    void moveOneStep();
    ///////////////////////////////////////////////////////////
    // Process keyboard input
    void handleKeyPressed(char key);
    ///////////////////////////////////////////////////////////
    // Interaction logic
    void handleDoor(char char_on_screen);
    ///////////////////////////////////////////////////////
    void disposeItem();
    ///////////////////////////////////////////////////////////
    // Item management
    void setHeldItem(ItemType type);
    ///////////////////////////////////////////////////////////
    ItemType getHeldItem() const
    {
        return held_item;
    }
    ///////////////////////////////////////////////////////////
    void handleCollision(int component_char);
    //////////////////////////////////////////////////////////
    // State updates and checks
    bool isFinished() const { return is_finished_level; }
    ///////////////////////////////////////////////////////////
    // Visual toggles for level transitions or death
    void disappear();
    void appear();
    //////////////////////////////////////////////////////////
    // Getters
    int getCurrentRoomIndex() const
    {
        return current_room_index;
    }
    ///////////////////////////////////////////////////////////
    char getCharacter() const
    {
        return character;
    }
    ////////////////////////////////////////////////////////////
    int getLifeCount() const
    {
        return life_count;
    }
    /////////////////////////////////////////////////////////////
    Point getLocation() const
    {
        return location;
    }
    /////////////////////////////////////////////////////////////
    // Handle being hit by a bomb
    void hitByBomb()
    {
        life_count--;
        if (life_count <= 0)
        {
            disappear();
        }
    }
    //////////////////////////////////////////////////////////////
    void ResetHealth()
    {
        life_count = 3;
        is_finished_level = false;
    }
    //////////////////////////////////////////////////////////////
    bool isAlive() const
    {
        return life_count > 0;
    }
    ///////////////////////////////////////////////////////////
    // Reset player for new level or restart
    void resetLevelpos(int x, int y);
    ///////////////////////////////////////////////////////////
    void setDoor(Door* new_door)
    {
        pDoor = new_door;
    }
    ///////////////////////////////////////////////////////////
    void increaseScore()
    {
        Score = Score + 1;
    }
    ///////////////////////////////////////////////////////////
    void setPartner(Player* other_player)
    {
        partner = other_player;
    }
    ///////////////////////////////////////////////////////////
    void setObstacles(std::vector<Obstacle>* obstacles)
    {
        my_obstacles = obstacles;
    }
    //////////////////////////////////////////////////////////
    void handleObstacle();
    /////////////////////////////////////////////////////////
    int manageForce();
    /////////////////////////////////////////////////////////
    int getSpeed() const
    {
        return speed;
    }
    ///////////////////////////////////////////////////////
    void setSpeed(int s)
    {
        speed = s;
    }
    ///////////////////////////////////////////////////////
    void setDirection(Direction dir)
    {
        location.setDirection(dir);
    }
    ///////////////////////////////////////////////////////
    void resetSpring(Player& my_player, Point& my_location);  
    ///////////////////////////////////////////////////////
    Spring& getSpring() const
    {
        return my_spring; 
    }
    //////////////////////////////////////////////////////////
    bool isOpposite(Direction dir1, Direction dir2) const;
    ///////////////////////////////////////////////////////////
    void setScore(int new_score)
    {
        Score = new_score;
    }
    ///////////////////////////////////////////////////////////
    int getScore() const
    {
        return Score;
    }
    ///////////////////////////////////////////////////////////

    void setNewLevel()
    {
        my_new_level++;
    }
    ///////////////////////////////////////////////////////////
    void setNewLevel(int new_n)
    {
        my_new_level = new_n;
    }
    ///////////////////////////////////////////////////////////
    int getNewLevel() const
    {
        return my_new_level;
    }
    ///////////////////////////////////////////////////////////
    const string& getLastRiddleLog() const
    {
        return last_riddle_log;
    }
    //////////////////////////////////////////////////////////
    int getRiddleCount() const
    {
        return riddle_counter;
    }
    //////////////////////////////////////////////////////////
};