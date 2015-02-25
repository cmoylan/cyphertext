#include "Level.h"

Level::Level()
{

}


Level::~Level()
{

}


bool
Level::loadFromJson(const std::string filename)
{
    std::string jsonString = Util::loadStringFromFile(filename);

    //const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    rapidjson::Document d;
    d.Parse(jsonString.c_str());

    // 2. Modify it by DOM.
    //rapidjson::Value& s = d["stars"];
    //s.SetInt(s.GetInt() + 1);

    // 3. Stringify the DOM
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    std::cout << buffer.GetString() << std::endl;

    return true;
}
