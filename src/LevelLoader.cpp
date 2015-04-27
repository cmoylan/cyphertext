#include "Level.h"

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

    // TODO: possibly abstract
    // Using a reference for consecutive access is handy and faster.
    const Value& layers = document["layers"];

    for (i = 0; i < layers.Size(); i++) {
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

    // load sprites into textured
    // use glTexSubImage2d to only use a portion of the texture
    const Value& tilesets = document["tilesets"];
    for (i = 0; i < tilesets.Size(); i++) {
        if (!loadTileset(tilesets[i])) {
            return false;
        }
    }

    return true;
}


bool
Level::loadTileset(const rapidjson::Value& data)
{
    using namespace rapidjson;

    GLuint tex;
    int firstGid, lastGid, w, h, tw, th;
    std::string filename;
    std::string layername;

    glGenTextures(1, &tex);

    filename = data["image"].GetString();
    layername = data["name"].GetString();
    // FIXME: test if this stuff is found...it should be there
    //if (!filename || !layername) {
    //	return false;
    //}

    //Util::loadTexture(tex, filename);

    firstGid = data["firstgid"].GetInt();
    w = data["imagewidth"].GetInt();
    h = data["imageheight"].GetInt();
    tw = data["tilewidth"].GetInt();
    th = data["tileheight"].GetInt();
    lastGid = firstGid + ((w / tw) * (h / th)) - 1;

    LevelTexture levelTexture = { tex, firstGid, lastGid, w, h, tw, th };
    textures[layername] = levelTexture;

    return true;
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
    //layers[""];
    int value;

    platforms.clear();
    platformCount = 0;

    for (int i = 0; i < data.Size(); i++) {
        value = data[i].GetInt();
        platforms.push_back(value);

        if (value > 0) {
            platformCount++;
        }
    }

    return true;
}
