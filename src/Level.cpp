#include "Level.h"

Level::Level(int tilesOnScreenX, int tilesOnScreenY)
{
    this->tilesOnScreenX = tilesOnScreenX;
    this->tilesOnScreenY = tilesOnScreenY;
    tileSizeX = (2 * SCREEN_X) / tilesOnScreenX;
    tileSizeY = (2 * SCREEN_Y) / tilesOnScreenY;
    //printf("tilesize x y is: %f %f\n", tileSizeX, tileSizeY);
    NumVertices = 6;
}


Level::~Level()
{
    // clear textures
    // erase other things you set up
}


// Get a range of X's at a certain Y
// Convert those into platform array positions
// Check to see if there are platforms at given positions
// TODO: store everything in the same format
bool
Level::canFall(int y, int startX, int endX)
{
    // Don't fall off the screen...
    if (y <= -SCREEN_Y)  {
        return false;
    }

    // TODO: magic numbers
    int col1 = (startX + 100) / (int) tileSizeX;
    int col2 = (endX + 99) / (int) tileSizeX;
    //int row = ((-1 * y) + 100) / (int) tileSizeY;
    int row = ((-1 * y) + 100) / (int) tileSizeY;

    // need to detect the presence of tiles in the platform layers
    Layer& layer = layers.find("platforms")->second;
    int gid1 = layer.tiles[(row * mapWidth) + col1];
    int gid2 = layer.tiles[(row * mapWidth) + col2];

#   ifdef DEBUG_LEVEL
    // make sure if you change how gid1 and 2 are retreived, this changes also
    printf("-- indices are: %d, %d\n", ((row * mapWidth) + col1), ((row * mapWidth) + col2));
    printf("-- gid1 and 2 are: %d, %d\n", gid1, gid2);
#   endif

    return gid1 == 0 && gid2 == 0;
}


void
Level::initGL()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[ElementArrayBuffer]);

    shaderProgram = Shader::getInstance()->get("level");
    glUseProgram(shaderProgram);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    GLint vTexPosition = glGetAttribLocation(shaderProgram, "vTexPosition");
    glVertexAttribPointer(vTexPosition, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          BUFFER_OFFSET(2 * sizeof(float)));
    glEnableVertexAttribArray(vTexPosition);

    Util::resetGlState();
}


bool
Level::isBlocked(int originX, int originY, Vector2D size)
{
    int sizeX = size.x - 1;
    int sizeY = size.y - 1;

    // possibly correct way
    //if (valueAt(originX, originY) != 0 ||
    //   valueAt(originX + size.x, originY) != 0 ||
    //   valueAt(originX, originY + size.y) != 0 ||
    //   valueAt(originX + size.x, originY + size.y) != 0
    //) {

    if (valueAt(originX, originY) != 0) {
#       ifdef DEBUG_LEVEL
        printf("--blocked at [%d, %d]--\n", originX, originY);
#       endif
        return true;
    }
    if (valueAt(originX + sizeX, originY) != 0) {
#       ifdef DEBUG_LEVEL
        printf("--blocked at [%d, %d]--\n", originX + sizeX, originY);
#       endif
        return true;
    }
    if (valueAt(originX, originY + sizeY) != 0) {
#       ifdef DEBUG_LEVEL
        printf("--blocked at [%d, %d]--\n", originX, originY + sizeY);
#       endif
        return true;
    }
    if (valueAt(originX + sizeX, originY + sizeY) != 0) {
#       ifdef DEBUG_LEVEL
        printf("--blocked at [%d, %d]--\n", originX + sizeX, originY + sizeY);
#       endif
        return true;
    }

    return false;
}


void
Level::render()
{
    // TODO: only need to do this once
    // start in upper left corner of screen: (-1.f, 1.f)
    Point tl, tr, bl, br;
    tl = { -1.f, 1.f };
    tr = { (SCALE_X * tileSizeX) - 1.f, 1.f };
    br = { (SCALE_X * tileSizeX) - 1.f, 1.f - (SCALE_Y * tileSizeY) };
    bl = { -1.f, 1.f - (SCALE_Y * tileSizeY) };

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    // TODO: This is a shim for now, need to correctly load the textures once
    // per render cycle
    GLuint texId = textures.find("grass-tiles-2-small")->second.textureId;
    glBindTexture(GL_TEXTURE_2D, texId);

    // TODO: do this for each layer
    // TODO: will need to do them in the correct order so that bg is behind fg
    Layer& layer = layers.find("platforms")->second;
    // TODO: magic number
    GLfloat vertices[layer.tileCount * 24];

    // main loop to set up vertex data for a tile layer
    std::vector<int>::iterator tile;
    int row = 0, col = 0, c = 0;

    for (tile = layer.tiles.begin(); tile != layer.tiles.end(); ++tile) {
        if (*tile != 0) {
            // bl, tl, tr, br
            float transformX = SCALE_X * (float)(tileSizeX * col);
            float transformY = SCALE_Y * (float)(tileSizeY * row);
            TexCoord texCoord = useTextureFor(*tile);

            // first triangle
            vertices[c++] = bl.x + transformX; // 0
            vertices[c++] = (bl.y - transformY);
            vertices[c++] = texCoord.bl.x; // texcoord
            vertices[c++] = texCoord.bl.y;

            vertices[c++] = tl.x + transformX; //1
            vertices[c++] = (tl.y - transformY);
            vertices[c++] = texCoord.tl.x; // texcoord
            vertices[c++] = texCoord.tl.y;

            vertices[c++] = tr.x + transformX; //2
            vertices[c++] = (tr.y - transformY);
            vertices[c++] = texCoord.tr.x; // texcoord
            vertices[c++] = texCoord.tr.y;

            // second triangle
            vertices[c++] = bl.x + transformX; // 0
            vertices[c++] = (bl.y - transformY);
            vertices[c++] = texCoord.bl.x; // texcoord
            vertices[c++] = texCoord.bl.y;

            vertices[c++] = tr.x + transformX; // 2
            vertices[c++] = (tr.y - transformY);
            vertices[c++] = texCoord.tr.x;
            vertices[c++] = texCoord.tr.y;

            vertices[c++] = br.x + transformX; // 3
            vertices[c++] = (br.y - transformY);
            vertices[c++] = texCoord.br.x;
            vertices[c++] = texCoord.br.y;
        }

        if (col < (TILES_ON_SCREEN_X - 1)) {
            col++;
        }
        else { // new row
            col = 0;
            row++;
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDrawArrays(GL_TRIANGLES, 0, c / 4);

    glFlush(); // TODO: remove
}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);
    //printf("platform count: %d\n", platformCount);

    // TODO: do this for each layer
    // TODO: rename to printLayers
    printPlatforms();

    // iterate over tilsets
    TextureList::iterator t;
    for (t = textures.begin(); t != textures.end(); ++t) {
        LevelTexture& tex = t->second;
        printf("\ntileset: %s \n", t->first.c_str());
        printf("--- details: firstGID: %d lastGid: %d, w: %d, h: %d, tw: %d, th: %d,\n",
               tex.firstGid, tex.lastGid, tex.width, tex.height, tex.tileWidth,
               tex.tileHeight);
        printf("             rows: %d, cols: %d scaleX: %f, scaleY: %f\n",
               tex.numRows, tex.numCols, tex.scaleFactorX, tex.scaleFactorY);
    }

    printf("\n");
}


void
Level::printPlatforms()
{
    // ----- print out the level ----- //
    Layer& layer = layers.find("platforms")->second;
    std::vector<int>::iterator p;
    int i, row;
    row = 0;

    printf("\n[%d]:  ", row);
    for (p = layer.tiles.begin(), i = 1; p != layer.tiles.end(); ++p, ++i) {
        printf("%d | ", *p);
        if ((i % mapWidth == 0) && (row < (mapHeight - 1))) {
            ++row;
            printf("\n[%d]:  ", row);
        }
    }
    printf("\n");
    printf("tile count is: %d\n", layer.tileCount);
}


void
Level::unloadTextures()
{
}


TexCoord
Level::useTextureFor(int tmxGid)
{
    // check the memo for pre-calculated values
    GidTexCoords::iterator cached = gidTexCoords.find(tmxGid);
    if (cached != gidTexCoords.end()) {
        return cached->second;
    }

    int gid = tmxGid - 1; // it's easier to generate texcoords with a zero-index gid
    TexCoord texCoord;
    TextureList::iterator t;

    // --- find correct tileset
    for (t = textures.begin(); t != textures.end(); ++t) {
        LevelTexture& texture = t->second;

        if ((texture.firstGid <= tmxGid) && (texture.lastGid >= tmxGid)) {
            // --- calculate texcoords
            // TODO: make this its own method?
            // LEFT OFF HERE
            // this isn't working
            // one texture per layer
            // probably combine all textures into one large sprite

            int row = gid / texture.numCols;
            int col = gid % texture.numCols;
            int tw = texture.tileWidth;
            int th = texture.tileHeight;
            float scaleFactorX = texture.scaleFactorX;
            float scaleFactorY = texture.scaleFactorY;

            // TODO: maybe just use an array here
            texCoord.tl = *new Point { scaleFactorX * (float)(col * tw), scaleFactorY * (float)(row * th) };
            texCoord.tr = *new Point { scaleFactorX * (float)((col + 1) * tw), scaleFactorY * (float)(row * th) };
            texCoord.br = *new Point { scaleFactorX * (float)((col + 1) * tw), (scaleFactorY * (float)(row + 1) * th) };
            texCoord.bl = *new Point { scaleFactorX * (float)(col * tw), scaleFactorY * (float)((row + 1) * th) };
            //printf("-- details for %d: [x,y]: %d %d, [x2,y2]: %d %d\n", gid, col * tw,
            //       row * th, (col + 1) * tw, (row + 1) * th);
            //printf("     row: %d, col: %d\n", row, col);
            gidTexCoords[tmxGid] = texCoord; // add it to cache

            break;
        }
    }

    return texCoord;
}


int
Level::valueAt(int x, int y)
{
    int row = (((-1 * y) + 99) / (int) tileSizeY);
    int col = (x + 100) / (int) tileSizeX;

    //printf("- checking row %d, col %d \n", row, col);
    // NOTE: assume platforms for now
    Layer& layer = layers.find("platforms")->second;

    return layer.tiles[(row * tilesOnScreenX) + col];
}
