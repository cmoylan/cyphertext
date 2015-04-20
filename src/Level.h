#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "OpenGL.h"
#include "Constants.h"
#include "Shader.h"
#include "Util.h"

//using namespace rapidjson;
/**
 * TMX Subset:
 * Layer 0: Platforms - surfaces the player can walk on
 */

struct LevelTexture {
    //const std::string filename;
    GLuint textureId;
    int firstGid;
    int lastGid;
    int width;
    int height;
    int tileWidth;
    int tileHeight;
};

typedef std::map<std::string, LevelTexture> TextureList;

struct point {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
};

class Level {

    GLuint vao, tex, uniTrans;
    GLuint shaderProgram;

    // new vars
    GLint attributeCoord;
    GLint uniformTex;
    GLint uniformColor;
    GLuint vbo;
    // end new vars

    float tileSizeX, tileSizeY;

public:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    Vector2D camera;

    std::vector<int> platforms;
    TextureList textures;


    Level();
    ~Level();

    void initGL();

    bool loadFromJson(const std::string filename);
    bool loadTileset(const rapidjson::Value& data);
    bool setPlatforms(const rapidjson::Value& data);
    bool setMetadata(const rapidjson::Value& data);

    void render();

    /**
     * Sets the correct texture for the given GID
     * @param gid
     */
    void useTextureFor(int gid);

    void print();

    void unloadTextures();
};
