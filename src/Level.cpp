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

    SizeType i;
    Document document;

    std::string jsonString = Util::loadStringFromFile(filename);

    document.Parse(jsonString.c_str());

    mapWidth = document["width"].GetInt();
    mapHeight = document["height"].GetInt();
    tileWidth = document["tilewidth"].GetInt();
    tileHeight = document["tileheight"].GetInt();

    // Using a reference for consecutive access is handy and faster.
    const Value& layers = document["layers"];
    std::string layerName;
    for (SizeType i = 0; i < layers.Size(); i++) {
        //printf("layers[%d] = %s\n", i, layers[i]["name"].GetString());
        layerName = layers[i]["name"].GetString();
      if (layerName == "platforms") {
        setPlatforms();
      }
      else if (layerName) {
      }
    }

    // TODO: make sure this exists before trying to call it
    const Value& data = document["layers"][0]["data"];
    for (i = 0; i < data.Size(); i++) {
        platforms.push_back(data[i].GetInt());
    }
    //platforms(document["layers"][0]["data"]);
    printf("platforms is populated. size: %d\n", platforms.size());
    //printf("layer 0: %s\n", document["layers"][0]["name"].GetString());


    return true;
}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);
}
