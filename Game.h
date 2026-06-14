#include "Game.h"
#include <Windows.h>
#include <cctype>    // tolower
#include <ctime>     // for time/srand

/////////////////////////////////////////////////////////////////////////////////////////////

const char player1_keys[NUM_KEYS + 1] = { 'w', 'd', 'x', 'a', 's', 'e' };
const char player2_keys[NUM_KEYS + 1] = { 'i', 'l', 'm', 'j', 'k', 'o' };

//////////////////////////////////////////////////////////////////////////////////////////////

Game::Game() :

    game_bomb(Point(), game_screen), spring1(game_screen), spring2(game_screen), game_riddle(game_screen, this), pDoor(game_screen, 0, false, false),

    player1(Point(0, 0, 0, 0, '$'), player1_keys, game_screen, '$', game_bomb, spring1, current_room_index, &pDoor, game_riddle),

    player2(Point(0, 0, 0, 0, '&'), player2_keys, game_screen, '&', game_bomb, spring2, current_room_index, &pDoor, game_riddle)

{

    loadAllLevels(); // loads all screen files from folder

    player1.setPartner(&player2);

    player2.setPartner(&player1);

}

////////////////////////////////////////////////////////////////////////////////////////////////
void Game::setupLevel(int current_room_index) // prepares current stage data
{
    player1.setDoor(&pDoor);
    player2.setDoor(&pDoor);

    if (current_room_index >= all_levels_data.size())

        throw std::out_of_range("Game Logic Error: Attempted to setup invalid level index.");

    const LevelData& currentLevel = all_levels_data[current_room_index];

    if (currentLevel.hasDoor) // configures door if exists
    {
        const auto& d = currentLevel.doorData;
        pDoor.configure(d.keysNumber, d.switchesNumber, d.switchCode, d.switch_x, d.switch_y, d.id);
    }

    player1.resetLevelpos(currentLevel.p1Start.getX(), currentLevel.p1Start.getY()); // reset p1 position
    player2.resetLevelpos(currentLevel.p2Start.getX(), currentLevel.p2Start.getY()); // reset p2 position

    player1.setHeldItem(ItemType::NONE);
    player2.setHeldItem(ItemType::NONE);

    initObstacles(current_room_index); // scans for walls

    if (currentLevel.isDark) // handles dark rooms
    {
        game_screen.setDarkness(current_room_index, currentLevel.darkX1, currentLevel.darkY1, currentLevel.darkX2, currentLevel.darkY2);
    }
    else
    {
        game_screen.setDarkness(current_room_index, 0, 0, 0, 0);
    }
    game_screen.setLevelAt(current_room_index);
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void Game::runGameLoop() // main loop for updates and drawing
{
    setupLevel(current_room_index);

    if (shouldDraw())
    {
        drawLevelState();
    }

    sendGameEvent(GameEventType::LEVEL_CHANGE); // log start level

    bool wasLightOn = isAnyPlayerHoldingTorch();
    int last_room_index = current_room_index;

    iteration_counter = 0;

    while (state == GameState::RUNNING)
    {
        iteration_counter++; // counts ticks for sync

        if (current_room_index != last_room_index) // handle level transition
        {
            last_room_index = current_room_index;
            setupLevel(current_room_index);
            if (shouldDraw())
            {
                drawLevelState();
            }
            wasLightOn = isAnyPlayerHoldingTorch();

            sendGameEvent(GameEventType::GAME_STARTED);
        }

        updateLighting(wasLightOn);

        int p1LifeBefore = player1.getLifeCount();
        int p2LifeBefore = player2.getLifeCount();

        int p1_level_before = player1.getNewLevel();
        int p2_level_before = player2.getNewLevel();


        int p1_riddle_before = player1.getRiddleCount();
        int p2_riddle_before = player2.getRiddleCount();

        char key = getGameInput(); // get key from user or file

        last_key_pressed = key;

        if (key != 0)
        {
            processGameInput(key, current_room_index);
        }

        if (state != GameState::RUNNING)
            break;

        if (current_room_index < game_screen.getNumLevels() - 1)
        {
            player1.move();

            if (shouldDraw() && player2.isAlive()) player2.draw();

            player2.move();

            if (shouldDraw() && player1.isAlive()) player1.draw();
        };

        game_bomb.update(player1, player2); // checks explosions

        spring1.update(player1);
        spring2.update(player2);

        handlePlayerHealthChange(player1, p1LifeBefore, GameEventType::PLAYER_1_LOST_HEALTH, GameEventType::PLAYER_1_DEAD, GameEventType::PLAYER_1_ADD_HEALTH);
        handlePlayerHealthChange(player2, p2LifeBefore, GameEventType::PLAYER_2_LOST_HEALTH, GameEventType::PLAYER_2_DEAD, GameEventType::PLAYER_2_ADD_HEALTH);

        if (player1.getNewLevel() > p1_level_before)
        {
            sendGameEvent(GameEventType::PLAYER_1_PASSED_LEVEL);
        }
        if (player2.getNewLevel() > p2_level_before)
        {
            sendGameEvent(GameEventType::PLAYER_2_PASSED_LEVEL);
        }
        if (player1.getRiddleCount() > p1_riddle_before)
        {
            sendGameEvent(GameEventType::RIDDLE_ANSWER, player1.getLastRiddleLog());
        }
        if (player2.getRiddleCount() > p2_riddle_before)
        {
            sendGameEvent(GameEventType::RIDDLE_ANSWER, player2.getLastRiddleLog());
        }
        if (shouldDraw())
        {
            game_screen.setHealthAt(player1, current_room_index);
            game_screen.setHealthAt(player2, current_room_index);

            game_screen.setScoreAt(player1, player2, current_room_index);
        }
        if (shouldSleep()) // controls speed
        {
            if (is_interactive)
            {
                Sleep(150);
            }
            else
            {
                Sleep(50);
            }
        }
    }
    sendGameEvent(GameEventType::GAME_ENDED);
}

////////////////////////////////////////////////////////////////////////////////////////

void Game::processGameInput(char key, int current_room_index) // handles pause or moves
{
    if (key == ESC)
    {
        if (!is_interactive)  // ignore esc in file mode
            return;

        game_screen.delMessageAt(current_room_index);

        const char* new_message = "The_game_is_paused.";

        game_screen.setMessageAt(current_room_index, new_message);

        char ch = _getch();

        while ((ch != ESC)) // pause loop
        {
            if (ch == H || ch == h)
            {
                state = GameState::MENU;
                game_screen.delMessageAt(current_room_index);
                return;
            }
            ch = _getch();
        }
        game_screen.delMessageAt(current_room_index);
        return;
    }

    player1.handleKeyPressed(key);
    player2.handleKeyPressed(key);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::run() { // manages game state machine

    hideCursor();

    while (state != GameState::EXIT)
    {
        switch (state) {

        case GameState::MENU:

            game_menu.draw(0);

            handleMenuChoice((char)_getch());

            break;

        case GameState::RUNNING:
            runGameLoop();
            break;

        case GameState::EXIT:
            break;

        }
    }
    cls();
    std::cout << "Goodbye!" << std::endl;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handleMenuChoice(char choice)
{
    switch (choice)
    {
    case ONE: // start new game
        current_room_index = 0;
        game_screen.restart();

        player1.ResetHealth();
        player2.ResetHealth();

        player1.setScore(0);
        player2.setScore(0);

        player1.setHeldItem(ItemType::NONE);
        player2.setHeldItem(ItemType::NONE);

        spring1.resetPower(player1);
        spring2.resetPower(player2);

        player1.setNewLevel(0);
        player2.setNewLevel(0);

        pDoor.ResetDoor();

        state = GameState::RUNNING;
        break;

    case TWO:
        g_use_colors = !g_use_colors;
        break;


    case NINE:

        state = GameState::EXIT;
        break;



    case EIGHT: // instructions
    {
        game_menu.draw(1);
        [[maybe_unused]] char ignore = _getch();
        break;
    }

    default:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::initObstacles(int room_index) // scans screen for walls
{
    levels_obstacles.clear();
    bool visited[MAX_Y][MAX_X] = { false };
    for (int y = 0; y < MAX_Y; y++)
    {
        for (int x = 0; x < MAX_X; x++)
        {
            Point p(x, y, 0, 0, ' ');
            char ch = game_screen.getCharAt(p, room_index);
            if (ch == OBSTACLE_CHAR && !visited[y][x])
            {
                levels_obstacles.emplace_back(game_screen, p, room_index, visited);
            }
        }
    }
    player1.setObstacles(&levels_obstacles);
    player2.setObstacles(&levels_obstacles);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::drawLevelState()
{
    bool isLightOn = isAnyPlayerHoldingTorch();

    game_screen.draw(current_room_index, isLightOn);
    game_screen.setHealthAt(player1, current_room_index);
    game_screen.setHealthAt(player2, current_room_index);
    game_screen.setScoreAt(player1, player2, current_room_index);

    if (player1.isAlive())
        player1.draw();

    if (player2.isAlive())
        player2.draw();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::updateLighting(bool& wasLightOn) // redraws if torch state changed
{
    bool isLightOn = isAnyPlayerHoldingTorch();

    if (isLightOn != wasLightOn)
    {
        if (shouldDraw())
        {
            game_screen.updateLight(current_room_index, isLightOn);

            if (player1.isAlive()) player1.draw();
            if (player2.isAlive()) player2.draw();
        }
        wasLightOn = isLightOn;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::loadAllLevels() // reads .screen files
{
    all_levels_data.clear();
    vector<string> levelFiles;

    try {

        for (const auto& entry : fs::directory_iterator("."))
        {
            string filename = entry.path().filename().string();

            if (entry.is_regular_file() && entry.path().extension() == ".screen" && filename.find("adv-world") == 0)
            {
                levelFiles.push_back(entry.path().string());
            }
        }
    }
    catch (const fs::filesystem_error& e) {
        throw std::runtime_error("Error accessing directory: " + string(e.what()));
    }

    std::sort(levelFiles.begin(), levelFiles.end()); // sort levels by name

    for (const auto& filePath : levelFiles)
    {
        LevelData data = Files::loadLevelFromFile(filePath);

        if (data.isValid)
        {
            game_screen.addScreenFromFile(data.currentroom, data.legendLoc);

            all_levels_data.push_back(data);
        }
    }
    if (all_levels_data.empty())
    {
        throw std::runtime_error("CRITICAL ERROR: No .screen files found in directory.");
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::printErrorMessage(const std::string& msg)
{
    system("cls");

    std::cout << "\n============================================" << std::endl;
    std::cout << "               GAME CRASHED                 " << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "Error: " << msg << std::endl;
    std::cout << "============================================" << std::endl;
    std::cout << "\nPress any key to exit..." << std::endl;
    _getch();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::sendGameEvent(GameEventType type, const string& my_msg) // logs events to string
{
    string msg;

    switch (type)
    {
    case GameEventType::PLAYER_1_DEAD:

        msg = "Player 1 is dead";
        break;

    case GameEventType::PLAYER_2_DEAD:
        msg = "Player 2 is dead";
        break;

    case GameEventType::PLAYER_1_LOST_HEALTH:
        msg = "Player 1 lost health";
        break;

    case GameEventType::PLAYER_2_LOST_HEALTH:
        msg = "Player 2 lost health";
        break;

    case GameEventType::PLAYER_1_ADD_HEALTH:
        msg = "Player 1 added health";
        break;

    case GameEventType::PLAYER_2_ADD_HEALTH:
        msg = "Player 2 added health";
        break;

    case GameEventType::LEVEL_CHANGE:
        msg = "Level Changed to: " + std::to_string(current_room_index + 1);
        break;

    case GameEventType::GAME_ENDED:
        msg = "Game Ended. Score P1: " + std::to_string(player1.getScore()) +
            " P2: " + std::to_string(player2.getScore());
        break;

    case GameEventType::PLAYER_1_PASSED_LEVEL:
        msg = "Player 1 moved to next level " + std::to_string(player1.getNewLevel() + 1);
        break;

    case GameEventType::PLAYER_2_PASSED_LEVEL:
        msg = "Player 2 moved to next level " + std::to_string(player2.getNewLevel() + 1);
        break;

    case GameEventType::RIDDLE_ANSWER:

        if (!my_msg.empty())
        {
            msg = my_msg;
        }
        else
        {
            msg = "Riddle event occurred";   //Backup if we have mistake in riddles writing
        }

        ///////////////////////////////////////////////////////////

    default:
        break;
    }

    /////////////////////////////////////////////////////////////
    if (!msg.empty())  //add to file report
    {
        reportEvent(msg);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//xonstructor for Interactive Mode
GameInteractive::GameInteractive(bool saveMode) : is_save_mode(saveMode)
{
    if (is_save_mode)
    {
        steps_file.open("adv-world.steps");
        result_file.open("adv-world.result");

        auto seed = (unsigned int)time(nullptr); //saves the current random seed in order to be able to reproduce the same sequence later
        srand(seed);
        steps_file << seed << std::endl;
    }
    else
    {
        srand((unsigned int)time(nullptr));
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Destructor for Interactive Mode
GameInteractive::~GameInteractive()
{
    if (steps_file.is_open())
        steps_file.close();

    if (result_file.is_open())
        result_file.close();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Getting input from keyboard in Interactive Mode
char GameInteractive::getGameInput()
{
    if (_kbhit())
    {
        char key = _getch();

        if (is_save_mode)
        {
            steps_file << iteration_counter << " " << (int)key << std::endl;
        }
        return key;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Reporting events in Interactive Mode
void GameInteractive::reportEvent(const std::string& msg)
{
    if (is_save_mode) //Need to change to Enums + in the run
    {
        result_file << iteration_counter << ": " << msg << std::endl;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Constructor for File Mode
GameFile::GameFile(bool silentMode) : is_silent(silentMode), test_failed(false)
{
    is_interactive = false;

    if (is_silent)
    {
        g_is_silent_mode = true; //Set global silent mode for utils
    }

    steps_file.open("adv-world.steps");
    result_file.open("adv-world.result");

    if (!steps_file.is_open())  //checking if file opened successfull
    {
        throw std::runtime_error("Could not open adv-world.steps for loading");
    }
    unsigned int seed;
    steps_file >> seed;
    srand(seed);

    readNextStep();
    state = GameState::RUNNING; //No need to show menu in file mode
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Destructor for File Mode
GameFile::~GameFile()
{
    if (steps_file.is_open())
        steps_file.close();

    if (result_file.is_open())
        result_file.close();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GameFile::run()
{
    hideCursor();
    runGameLoop();

    if (is_silent)
    {
        if (!test_failed)
        {
            string extraLine;
            bool hasMoreEvents = false;

            while (std::getline(result_file, extraLine))
            {
                if (!extraLine.empty())
                {
                    hasMoreEvents = true;
                    break;
                }
            }
            cls();

            if (hasMoreEvents)
            {
                std::cout << "TEST FAILED!" << std::endl;

                std::cout << "Game ended earlier than expected." << std::endl;

                std::cout << "Extra event found in result file: " << extraLine << std::endl;
            }
            else
            {
                std::cout << "Test Completed Successfully." << std::endl;

                std::cout << "All results matched the expected output." << std::endl;
            }
        }
    }
    else
    {
        cls();
        std::cout << "Replay Finished." << std::endl;
    }
    _getch();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameFile::readNextStep() // reads next input from steps file
{
    int keyVal;
    if (steps_file >> next_input_iteration >> keyVal)
    {
        next_input_key = (char)keyVal;
    }
    else // reached end of file
    {
        next_input_iteration = -1;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char GameFile::getGameInput() // injects key at correct iteration
{
    if (iteration_counter == next_input_iteration)
    {
        char key = next_input_key;
        readNextStep();
        return key;
    }
    if (next_input_iteration == -1)
    {
        state = GameState::EXIT;
        iteration_counter--; //To avoid counting extra iteration after end of file
        return 0;
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GameFile::reportEvent(const std::string& msg) // compares result with expected file
{
    if (is_silent)
    {
        if (!result_file.is_open())  //checking if file opened successfully
            return;

        string expectedLine;
        string currentEvent = std::to_string(iteration_counter) + ": " + msg;

        while (std::getline(result_file, expectedLine))
        {
            if (!expectedLine.empty())
                break;
        }
        if (expectedLine != currentEvent)
        {
            cls();
            std::cout << "TEST FAILED!" << std::endl;

            std::cout << "Mismatch at iteration " << iteration_counter << std::endl;

            std::cout << "Expected: " << expectedLine << std::endl;

            std::cout << "Got:      " << currentEvent << std::endl;

            state = GameState::EXIT;

            test_failed = true; //FOR SILENT MODE

            return;
        }
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Game::handlePlayerHealthChange(Player& p, int oldLife, GameEventType lostHealthEv, GameEventType deadEv, GameEventType addHealthEv)
{
    if (p.getLifeCount() < oldLife)
    {
        if (p.isAlive())
        {
            sendGameEvent(lostHealthEv);
        }
        else
        {
            sendGameEvent(deadEv);
        }
    }
    else if (p.getLifeCount() > oldLife)
    {
        sendGameEvent(addHealthEv);
    }
}