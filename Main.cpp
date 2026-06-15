#include "Game.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[])
{

    bool isSave = false;
    bool isLoad = false;
    bool isSilent = false;

    for (int i = 1; i < argc; ++i)  // read command-line args
    {
        std::string arg = argv[i];
        if (arg == "-save")
            isSave = true;
        else if (arg == "-load")
            isLoad = true;
        else if (arg == "-silent")
            isSilent = true;
    }

    Game* game = nullptr;

    try
    {

        if (isLoad || isSilent) // playback/test mode
        {
            game = new GameFile(isSilent);
        }
        else
        {
            game = new GameInteractive(isSave);
        }

        game->run();
    }
    catch (const std::exception& e) // catch standard exceptions
    {
        Game::printErrorMessage(e.what());
        if (game) delete game;
        return 1;
    }
    catch (...)
    {
        Game::printErrorMessage("An unknown error occurred.");
        if (game) delete game;
        return 1;
    }

    delete game;
    return 0;
}
