#include "Level.h"

bool
Level::loadFromJson(const std::string& filename)
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
        if (!loadLayer(layerName, layers[i]["data"], layers[i]["width"].GetInt(),
                       layers[i]["height"].GetInt())) {
            return false;
        }
    }

    // load sprites into textured
    const Value& tilesets = document["tilesets"];
    for (i = 0; i < tilesets.Size(); i++) {
        if (!loadTileset(tilesets[i])) {
            return false;
        }
    }

    return true;
}


// TODO: don't take a rapidjson reference here, just an array pointer
bool
Level::loadLayer(const std::string& layerName, const rapidjson::Value& data,
                 int width, int height)
{
    int count = 0;
    Layer layer;

    clearLayer(layerName);

    for (size_t i = 0; i < data.Size(); i++) {
        int value = data[i].GetInt();
        layer.tiles.push_back(value);

        if (value > 0) {
            count++;
        }
    }

    layer.tileCount = count;

    layers[layerName] = layer;

    // TODO: actually check that something was loaded
    return true;
}


void
Level::clearLayer(const std::string& name)
{
    // TODO: do something
    //Layer layer = layers.find(name);
    //delete layer;
}


bool
Level::loadTileset(const rapidjson::Value& data)
{
    using namespace rapidjson;

    GLuint tex;
    glGenTextures(1, &tex);

    std::string filename = data["image"].GetString();
    std::string layername = data["name"].GetString();
    // FIXME: test if this stuff is found...it should be there
    //if (!filename || !layername) {
    //	return false;
    //}

    Util::loadTexture(tex, filename);

    int firstGid = data["firstgid"].GetInt();
    int w = data["imagewidth"].GetInt();
    int h = data["imageheight"].GetInt();
    int tw = data["tilewidth"].GetInt();
    int th = data["tileheight"].GetInt();
    int lastGid = firstGid + ((w / tw) * (h / th)) - 1;
    int numRows = h / th;
    int numCols = w / tw;
    float scaleFactorX = 1.f / (float) w;
    float scaleFactorY = 1.f / (float) h;

    LevelTexture levelTexture = { filename, tex, firstGid, lastGid, w, h, tw, th, numRows, numCols, scaleFactorX, scaleFactorY };
    textures[layername] = levelTexture;

    return true;
}
