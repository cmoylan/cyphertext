#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Constants.h"
#include "Util.h"

//using namespace rapidjson;
/**
 * TMX Subset:
 * Layer 0: Platforms - surfaces the player can walk on
 */

class Level {

public:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    Vector2D camera;

    std::vector<int> platforms;


    Level();
    ~Level();

    bool loadFromJson(const std::string filename);

    void print();

};
