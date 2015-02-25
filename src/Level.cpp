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
    using namespace rapidjson;

    std::string jsonString = Util::loadStringFromFile(filename);

    Document document;
    document.Parse(jsonString.c_str());

    //rapidjson::Value& s;
    mapWidth = document["width"].GetInt();
    mapHeight = document["height"].GetInt();
    tileWidth = document["tilewidth"].GetInt();
    tileHeight = document["tileheight"].GetInt();
    //s.SetInt(s.GetInt() + 1);

    // Stringify the DOM
    //rapidjson::StringBuffer buffer;
    //rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    //doc.Accept(writer);

    // Output {"project":"rapidjson","stars":11}
    //std::cout << buffer.GetString() << std::endl;

    // Using a reference for consecutive access is handy and faster.
    const Value& layers = document["layers"];
    for (SizeType i = 0; i < layers.Size(); i++) {
        printf("layers[%d] = %s\n", i, layers[i]["name"].GetString());
    }


    return true;
}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);
}
