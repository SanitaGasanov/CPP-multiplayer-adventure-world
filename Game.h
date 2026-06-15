#pragma once

#include "Enums.h"
#include "Screen.h"
#include "Player.h"
#include "Bomb.h"
#include "Door.h"
#include "Menu.h"
#include "riddle.h" 
#include "utils.h" 
#include "Obstacle.h"
#include "Spring.h"
#include "Files.h"
#include <conio.h> 
#include <fstream> 
#include <iostream>
#include <string>
#include <filesystem>
#include <algorithm> //for sort

namespace fs = std::filesystem;

class Game {
public:
    char last_key_pressed = 0; // fix for riddle input bug
    enum class GameState {
        MENU,
        RUNNING,
        EXIT
    };

protected:

    Screen game_screen; // handles console output
    Menu game_menu;

    Bomb game_bomb;
    Spring spring1;
    Spring spring2;

    riddle game_riddle;

    Door pDoor;

    int current_room_index = 0; // tracks current level
    bool is_interactive = true; // flag for play vs test mode

    Player player1;
    Player player2;

    GameState state = GameState::MENU;

    std::vector<Obstacle> levels_obstacles; // stores obstacles for level
    std::vector<LevelData> all_levels_data;


    long iteration_counter = 0; // syncs game loops


    void initObstacles(int room_index); // scans map for obstacles
    void setupLevel(int current_room_index); // inits level data
    void runGameLoop(); // main game logic
    void processGameInput(char key, int current_room_index);
    void handleMenuChoice(char choice);
    virtual void sendGameEvent(GameEventType type, const string& my_msg = ""); // logs important events
    void handlePlayerHealthChange(Player& p, int oldLife, GameEventType lostHealthEv, GameEventType deadEv, GameEventType addHealthEv);

    bool isAnyPlayerHoldingTorch() const // lighting logic helper
    {
        return (player1.getHeldItem() == ItemType::TORCH) || (player2.getHeldItem() == ItemType::TORCH);
    }
    void drawLevelState(); // renders everything
    void updateLighting(bool& wasLightOn);
    void loadAllLevels(); // reads .screen files


    virtual void reportEvent(const std::string& msg) {} // virtual for polymorphism
    virtual bool shouldDraw() { return true; } // false in silent mode

public:
    Game();

    virtual ~Game() {}
    virtual void run(); // entry point

    static void printErrorMessage(const std::string& msg);
    virtual char getGameInput() = 0;    // pure virtual input method
    virtual bool shouldSleep() { return true; } // controls game speed
    char getLastKey() const { return last_key_pressed; }
};


class GameInteractive : public Game // handles real-time play
{
    bool is_save_mode; // records gameplay if true
    std::ofstream steps_file; // saves keystrokes
    std::ofstream result_file; // saves expected results

public:
    GameInteractive(bool saveMode);
    ~GameInteractive();

    virtual char getGameInput() override; // reads from keyboard
    virtual void reportEvent(const std::string& msg) override; // writes to result file
};

////////////////new

class GameFile : public Game // handles playback/testing
{
    bool is_silent; // disables graphics if true
    ifstream steps_file; // inputs for playback
    ifstream result_file; // expected results for validation
    bool test_failed = false; // flag for mismatch
    long next_input_iteration = -1;
    char next_input_key = 0;

    void readNextStep(); // helper for parsing steps

public:
    GameFile(bool silentMode);
    ~GameFile();

    virtual char getGameInput() override; // reads from file
    virtual void reportEvent(const std::string& msg) override; // compares actual vs expected
    virtual bool shouldSleep() override { return !is_silent; } // fast forward if silent
    virtual void run() override;
    virtual bool shouldDraw() override { return !is_silent; } // hide output if silent
};
