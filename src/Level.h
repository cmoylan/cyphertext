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
    int width;
    int height;
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
    int numRows;
    int numCols;
    float scaleFactorX;
    float scaleFactorY;
};

struct Point {
    GLfloat x;
    GLfloat y;
};

//struct LevelVertex {
//    GLfloat x;
//    GLflost y;
//    GLfloat tx;
//    GLfloat ty;
//};

struct TexCoord {
    Point tl;
    Point tr;
    Point bl;
    Point br;
};

typedef std::map<std::string, LevelTexture> TextureList;
typedef std::map<std::string, Layer> LayerList;
typedef std::map<int, TexCoord> GidTexCoords;


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

    GidTexCoords gidTexCoords;

public:
    int mapWidth;
    int mapHeight;
    int tileWidth;
    int tileHeight;
    Vector2D camera;

    LayerList layers;
    TextureList textures;


    Level();
    ~Level();

    void initGL();

    // ----- Level loading ----- //
    /**
     * Loads a TMX level from a json file
     * @param filename the json file from which to load the level
     * @returns a boolean indicating success or failure
     */
    bool loadFromJson(const std::string& filename);

    /**
     * Populates a LevelTexture struct from tileset json data
     * @param data a rapidjson value reference to the tileset data
     * @returns a boolean indicating success/failure
     */
    bool loadTileset(const rapidjson::Value& data);

    //void clearTileset(const std::string& name);

    /**
     * Populates a Layer struct and pushes it onto the level's LayerList
     * @param layerName a string key for the LayerList map
     * @param data a rapidjson referece to the json data to load
     * @param width the width of the level in tiles
     * @param height the height of the level in tiles
     * @return a boolean to indicate load success/failure
     *
     * Detailed description follows.
     */
    bool loadLayer(const std::string& layerName, const rapidjson::Value& data,
                   int width, int height);
    /**
     * Deletes a Layer from the LayerList
     * @param name a string key for the Layer to remove from LayerList
     */
    void clearLayer(const std::string& name);

    // ----- Rendering ----- //
    /**
     * Renders the level to the screen
     */
    void render();

    /**
     * Sets the correct texture for the given GID.
     * @param gid
     * @returns the coordinates for the subtexture
     */
    TexCoord useTextureFor(int test);

    /**
     * Sends level debugging information to stdout
     */
    void print();

    // DEPRECATED
    void unloadTextures();
};
