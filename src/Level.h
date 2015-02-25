#pragma once

#include <iostream>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Util.h"

//using namespace rapidjson;
/**
 * TMX Subset:
 * Layer 0: Level - surfaces the player can walk on
 */

class Level {

public:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;

    Level();
    ~Level();

    bool loadFromJson(const std::string filename);

    void print();

};