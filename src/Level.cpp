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


void Level::initGL()
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    //glGenBuffers(1, &vbo);
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffers[ElementArrayBuffer]);

    GLuint elements[] = { 0, 1, 2, 2, 3, 0 };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
                 GL_STATIC_DRAW);

    // TODO: move to shader object
    shaderProgram = Util::createProgramFromShaders("src/shaders/level.v.glsl",
                    "src/shaders/level.f.glsl");
    glUseProgram(shaderProgram);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);
}


void Level::render()
{
    int c = 0;

    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);

    GLfloat vertices[16];
    vertices[c++] = -0.90;
    vertices[c++] = -0.90;
    vertices[c++] = 0.9;
    vertices[c++] = -0.9;
    vertices[c++] = 0.9;
    vertices[c++] = 0.9;
    vertices[c++] = -0.9;
    vertices[c++] = 0.9;
    vertices[c++] = 0.0f;
    vertices[c++] = 0.0f;
    vertices[c++] = 1.0f;
    vertices[c++] = 0.0f;
    vertices[c++] = 1.0f;
    vertices[c++] = 1.0f;
    vertices[c++] = 0.0f;
    vertices[c++] = 1.0f;


    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, (void*)0);
    glFlush(); // TODO: remove

}


void
Level::initGL2()
{
    // TODO:
    // Set this up the way the demo does
    // Send the vertex + texcoord data during render
    //Util::createAndBindContext(&vao);

    glGenTextures(1, &tex);
    Util::loadTexture(tex, "res/red-blue-square.png");

    // ----- new code ----- //
    shaderProgram = Shader::getInstance()->get("level");
    // TODO: make sure none of these is -1
    attributeCoord = glGetAttribLocation(shaderProgram, "coord");
    uniformTex = glGetUniformLocation(shaderProgram, "tex");
    uniformColor = glGetUniformLocation(shaderProgram, "color");

    glGenBuffers(1, &vbo);
    // ----- end new code ----- //

    //glUseProgram(shaderProgram);

    //Util::mapPositionAttribute(shaderProgram);
    // TODO: change this
    //Util::mapTextureAttribute(shaderProgram);

    // translation attr from vector shader
    //uniTrans = glGetUniformLocation(shaderProgram, "trans");

    // --- send initial data to the shader
    // TODO: centralize elements[]
    //GLuint elements[] = {
    //    0, 1, 2,
    //    2, 3, 0
    //};
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements),
    //             elements, GL_STATIC_DRAW);

    // TODO: scaling magic
    //GLfloat vertices[] = {
    //    0.f, (SCALE_Y * tileSizeY), // top left
    //    (SCALE_X * tileSizeX), (SCALE_Y * tileSizeY), // top right
    //    (SCALE_X * tileSizeX), 0.f, // bottom right
    //    0.f, 0.f//, // bottom left
    //
    //    // Texcoords
    //    //0.0f, 0.0f,
    //    //1.0f, 0.0f,
    //    //1.0f, 1.0f,
    //    //0.0f, 1.0f
    //};

    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Util::resetGlState();
}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);
    printf("platform count: %d\n", platformCount);

    // ----- print out the level ----- //
    std::vector<int>::iterator p;
    TextureList::iterator t;
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

    // iterate over tilsets
    for (t = textures.begin(); t != textures.end(); t++) {
        printf("\ntileset: %s \n", t->first.c_str());
        printf("--- details: firstGID: %d lastGid: %d, w: %d, h: %d, tw: %d, th: %d\n",
               t->second.firstGid, t->second.lastGid, t->second.width,
               t->second.height, t->second.tileWidth, t->second.tileHeight);
    }

    printf("\n");
}


void
Level::render2()
{
    // only create one shader, we will reuse it
    // textures, ideally will be atlased
    // for now we can just upload the individual textures
    std::vector<int>::iterator p;
    int i, row, col;
    glm::mat4 trans;

    //printf("tilesizeX is : %f\n");
    glUseProgram(shaderProgram);
    //glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);

    // ----- new code ----- //
    glEnableVertexAttribArray(attributeCoord);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attributeCoord, 4, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements,
                 GL_STATIC_DRAW);

    // TODO: this is not quite correct, it includes blank spaces as well
    //point coords[4 * platforms.size()];
    point coords[1] = {
        {1.0, 1.0, 1.0, 1.0}
    };

    // ----- end new code ----- //

    // TODO: start row at 1, remove (row+1) in glm::vec3 call
    row = 0;
    col = 0;
    for (p = platforms.begin(), i = 0; p != platforms.end(); p++, i++) {
        //printf("row and col are: %d, %d\n", row, col);

        // if (*p > 0) {
        //    useTexture(*p)
        if (*p == 8) {
            //glm::mat4 trans;
            //trans = glm::translate(trans,
            //                       //glm::vec3((SCALE_X * (10 * col)),
            //                       //      (SCALE_Y * (10 * row)),
            //                       glm::vec3((SCALE_X * ((float) - SCREEN_X + (tileSizeX * col))),
            //                                 (SCALE_Y * ((float) SCREEN_Y - (tileSizeY * (row + 1)))),
            //                                 1.0f));
            //
            //uniTrans = glGetUniformLocation(shaderProgram, "trans");
            //
            //glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
            //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        if (col < (TILES_ON_SCREEN_X - 1)) {
            col++;
        }
        else {
            col = 0;
            row++;
        }
    }

    Util::resetGlState();
}


void
Level::unloadTextures()
{
}


void
Level::useTextureFor(int gid)
{
    // TODO: cache this in a hash...hash this
    // ---- IDEAS --- //
    // create hash, keyed on gid
    // contents of hash are texID, and offset/subset measurements
    // ...that's it!
    // ---- END IDEA ----//

    TextureList::iterator t;
    // --- find correct tileset
    // iterate over tilesets
    // we now have lastGID
    for (t = textures.begin(); t != textures.end(); t++) {
        //t->first
        //t->second
        if ((t->second.firstGid <= gid) && (t->second.lastGid >= gid)) {
            // this is the right tileset
            // calculate ofset

        }
    }


    // --- calculate texture subcoords

    // --- use texture with subcoords
}
