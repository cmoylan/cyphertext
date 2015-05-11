#include "Level.h"

Level::Level()
{

    tileSizeX = (2 * SCREEN_X) / TILES_ON_SCREEN_X;
    tileSizeY = (2 * SCREEN_Y) / TILES_ON_SCREEN_Y;
    NumVertices = 6;
    initGL();

    // TODO: temp
    glGenTextures(1, &tex);
    Util::loadTexture(tex, "res/test-tileset/grass-tiles-2-small.png");
    //Util::loadTexture(tex, "res/col-test.png");

}


Level::~Level()
{
    // clear textures
}


void
Level::initGL()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[ElementArrayBuffer]);

    // TODO: move to shader object
    shaderProgram = Util::createProgramFromShaders("src/shaders/level.v.glsl",
                    "src/shaders/level.f.glsl");
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
Level::isBlocked(int y, int startX, int endX)
{
    // TODO: store everything in the same format

    // assume each tile is 10 wide - move this to a constant or calculate it

    // convert those into 2 col positions
    // check the array at those 2 places
    // that's it
    int col1 = (startX + 100) / 10;
    int col2 = (endX + 100) / 10;
    int row = (y + 100) / 10;
    //printf("checking %d: %d..%d\n", row, col1, col2);

    // need to detect the presence of tiles in the platform layers
    Layer& layer = layers.find("platforms")->second;
    int gid1 = layer.tiles[(row * 20) + col1];
    int gid2 = layer.tiles[(row * 20) + col2];
    //printf("gids are: %d %d\n", gid1, gid2);

    if (gid1 != 0 || gid2 != 0) {
        return true;
    }
    return false;
}

void
Level::render()
{
    // TODO: only need to do this once
    Point tl, tr, bl, br;
    //tl = { -1.f, (SCALE_Y * tileSizeY) - 1.f};
    //tr = { (SCALE_X * tileSizeX) - 1.f, (SCALE_Y * tileSizeY) - 1.f };
    //br = { (SCALE_X * tileSizeX) - 1.f, -1.f };
    //bl = { -1.f, -1.f };

    tl = { -1.f, 1.f };
    tr = { (SCALE_X * tileSizeX) - 1.f, 1.f };
    br = { (SCALE_X * tileSizeX) - 1.f, 1.f - (SCALE_Y * tileSizeY) };
    bl = { -1.f, 1.f - (SCALE_Y * tileSizeY) };

    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);

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

            // NOTE: the y axis has to be inverted because the level is stored
            //       from top to bottom in the tmx format. We should change the
            //       way the level is stored to avoid doing that.
            //       Because the y axis is inverted, the texcoord y axis also has
            //       to be inverted.

            //vertex
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

    // ----- print out the level ----- //
    std::vector<int>::iterator p;
    TextureList::iterator t;
    int i, row;

    // do this for each layer
    Layer& layer = layers.find("platforms")->second;
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

    // iterate over tilsets
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
