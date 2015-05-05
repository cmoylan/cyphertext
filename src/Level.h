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

#define BUFFER_OFFSET(offset) ((void *)(offset))

//using namespace rapidjson;
/**
 * TMX Subset:
 * Layer 0: Platforms - surfaces the player can walk on
 */


struct Layer {
    int tileCount;
    std::vector<int> tiles;
};

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
typedef std::map<std::string, Layer> LayerList;

struct point {
    GLfloat x;
    GLfloat y;
};

//struct LevelVertex {
//    GLfloat x;
//    GLflost y;
//    GLfloat tx;
//    GLfloat ty;
//};

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



    enum VAO_IDs { Triangles, NumVAOs };
    enum Buffer_IDs { ArrayBuffer, ElementArrayBuffer, NumBuffers };
    enum Attrib_IDs { vPosition = 0, vTexPosition = 0 };
    GLuint VAOs[NumVAOs];
    GLuint Buffers[NumBuffers];
    GLuint NumVertices;

public:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    Vector2D camera;


    std::vector<int> platforms;
    int platformCount;

    LayerList layers;
    TextureList textures;


    Level();
    ~Level();

    void initGL();
    void initGL2();

    bool loadFromJson(const std::string& filename);
    bool loadTileset(const rapidjson::Value& data);
    bool setPlatforms(const rapidjson::Value& data);
    bool setMetadata(const rapidjson::Value& data);

    void render();
    void render2();

    /**
     * Sets the correct texture for the given GID
     * @param gid
     */
    void useTextureFor(int gid);

    void print();

    void unloadTextures();
};
