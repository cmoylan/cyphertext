#pragma once

#include <iostream>
#include <string>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "Util.h"

//using namespace rapidjson;

class Level {

public:
    Level();
    ~Level();
    
    bool loadFromJson(const std::string filename);
};