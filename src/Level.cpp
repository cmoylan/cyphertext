#include "Level.h"

Level::Level()
{

    tileSizeX = (2 * SCREEN_X) / TILES_ON_SCREEN_X;
    tileSizeY = (2 * SCREEN_Y) / TILES_ON_SCREEN_Y;
    NumVertices = 6;
    initGL();

    // TODO: temp
    glGenTextures(1, &tex);
    Util::loadTexture(tex, "res/red-blue-square.png");
}


Level::~Level()
{
    // clear textures
}


void
Level::initGL()
{
    int e, i, elementCount;

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


void
Level::render()
{
    // TODO: only need to do this once
    Point tl, tr, bl, br;
    tl = { -1.f, (SCALE_Y * tileSizeY) - 1.f};
    tr = { (SCALE_X * tileSizeX) - 1.f, (SCALE_Y * tileSizeY) - 1.f };
    br = { (SCALE_X * tileSizeX) - 1.f, -1.f };
    bl = { -1.f, -1.f };

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
            //printf("col: %d, val: %f ", col, transformX);
            // NOTE: the y axis has to be inverted because the level is stored
            //       from top to bottom in the tmx format. We should change the
            //       way the level is stored to avoid doing that.

            //vertex
            vertices[c++] = bl.x + transformX; // 0
            vertices[c++] = -1.f * (bl.y + transformY);
            vertices[c++] = 0.f; // texcoord
            vertices[c++] = 1.f;

            vertices[c++] = tl.x + transformX; //1
            vertices[c++] = -1.f * (tl.y + transformY);
            vertices[c++] = 0.f; // texcoord
            vertices[c++] = 0.f;

            vertices[c++] = tr.x + transformX; //2
            vertices[c++] = -1.f * (tr.y + transformY);
            vertices[c++] = 1.f; // texcoord
            vertices[c++] = 0.f;

            // second triangle
            vertices[c++] = bl.x + transformX; // 0
            vertices[c++] = -1.f * (bl.y + transformY);
            vertices[c++] = 0.f; // texcoord
            vertices[c++] = 1.f;

            vertices[c++] = tr.x + transformX; // 2
            vertices[c++] = -1.f * (tr.y + transformY);
            vertices[c++] = 1.f;
            vertices[c++] = 0.f;

            vertices[c++] = br.x + transformX; // 3
            vertices[c++] = -1.f * (br.y + transformY);
            vertices[c++] = 1.f;
            vertices[c++] = 1.f;
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
        printf("\ntileset: %s \n", t->first.c_str());
        printf("--- details: firstGID: %d lastGid: %d, w: %d, h: %d, tw: %d, th: %d, rows: %d, cols: %d\n",
               t->second.firstGid, t->second.lastGid, t->second.width,
               t->second.height, t->second.tileWidth, t->second.tileHeight,
               t->second.rows, t->second.cols);
    }

    printf("\n");
}


void
Level::unloadTextures()
{
}


TexCoord
Level::useTextureFor(int gid)
{
    // --- from gid find and use correct texture
    // --- calculate and return texcoords

    // TODO: cache this in a hash...hash this
    // ---- IDEAS --- //
    // create hash, keyed on gid
    // contents of hash are texID, and offset/subset measurements
    // ...that's it!
    // ---- END IDEA ----//

    TextureList::iterator t;
    // --- find correct tileset
    for (t = textures.begin(); t != textures.end(); ++t) {
        if ((t->second.firstGid <= gid) && (t->second.lastGid >= gid)) {
            // this is the right tileset
            // calculate ofset
            // offset x  = gid divided by rows
            // offset y = gid deivided by cols
            // offset x * tilesizex
            // offset y * tilesizey

        }
    }

}
