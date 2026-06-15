#pragma once

#include "Enums.h"
#include <vector>
#include <string>
#include "Point.h" 
#include <fstream>

using std::vector;
using std::string;

struct DoorData  //door info
{
    int id;
    int keysRequired;
    int switchesRequired;
    int keysNumber;
    int switchesNumber;
    int switch_x;
    int switch_y;
    string switchCode;

};

struct RiddleData {
    int id;
    Point loc;
};

struct LevelData
{
    vector<string> currentroom;
    Point legendLoc = { 0,0,0,0,' ' };
    Point p1Start = { 1,1,0,0,' ' };
    Point p2Start = { 2,1,0,0,' ' };

    DoorData doorData;
    bool hasDoor = false;

    vector<RiddleData> riddles;

    bool isDark = false;
    int darkX1 = 0, darkY1 = 0, darkX2 = 0, darkY2 = 0;

    bool isValid = false;
};


class Files
{
public:

    static LevelData loadLevelFromFile(const std::string& filename); //loads level data from file
};
