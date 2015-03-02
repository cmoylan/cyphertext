#include "Level.h"

Level::Level()
{
    Shader::getInstance();
}


Level::~Level()
{

}


void
Level::initGL()
{

}


bool
Level::loadFromJson(const std::string filename)
{
    using namespace rapidjson;

    SizeType i;
    Document document;
    std::string layerName;

    std::string jsonString = Util::loadStringFromFile(filename);

    document.Parse(jsonString.c_str());

    mapWidth = document["width"].GetInt();
    mapHeight = document["height"].GetInt();
    tileWidth = document["tilewidth"].GetInt();
    tileHeight = document["tileheight"].GetInt();

    // Using a reference for consecutive access is handy and faster.
    const Value& layers = document["layers"];

    for (SizeType i = 0; i < layers.Size(); i++) {
        layerName = layers[i]["name"].GetString();

        if (layerName == "platforms") {
            if (!setPlatforms(layers[i]["data"])) {
                return false;
            }
        }
        else if (layerName == "metadata") {
            if (!setMetadata(layers[i]["data"])) {
                return false;
            }
        }
    }

    //printf("platforms is populated. size: %d\n", platforms.size());
    render();
    return true;
}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);
}


void
Level::render()
{
    // only create one shader, we will reuse it
    // textures, ideally will be atlased
    // for now we can just upload the individual textures
    std::vector<int>::iterator p;
    int i, row;

    row = 0;

    printf("\n[%d]:  ", row);

    for (p = platforms.begin(), i = 1; p != platforms.end(); ++p, ++i) {


        printf("%d | ", *p);

        if ((i % mapWidth == 0) && (row < (mapHeight - 1))) {
            ++row;
            printf("\n[%d]:  ", row);
        }
    }

    printf("\n");

}


bool
Level::setMetadata(const rapidjson::Value& data)
{
    // TODO: get camera and character starting positions
    return true;
}


bool
Level::setPlatforms(const rapidjson::Value& data)
{
    platforms.clear();

    for (int i = 0; i < data.Size(); i++) {
        platforms.push_back(data[i].GetInt());
    }

    return true;
}
