#include "Level.h"

Level::Level()
{

    tileSizeX = (2 * SCREEN_X) / TILES_ON_SCREEN_X;
    tileSizeY = (2 * SCREEN_Y) / TILES_ON_SCREEN_Y;

    initGL();
}


Level::~Level()
{

}


void
Level::initGL()
{
    Util::createAndBindContext(&vao);

    glGenTextures(1, &tex);
    Util::loadTexture(tex, "res/red-square.png");

    shaderProgram = Shader::getInstance()->get("texturedSquare");
    glUseProgram(shaderProgram);

    Util::mapPositionAttribute(shaderProgram);
    Util::mapTextureAttribute(shaderProgram);

    // translation attr from vector shader
    uniTrans = glGetUniformLocation(shaderProgram, "trans");

    // --- send initial data to the shader
    // TODO: centralize elements[]
    GLuint elements[] = {
        0, 1, 2,
        2, 3, 0
    };

    // TODO: scaling magic
    GLfloat vertices[] = {
        0.f, (SCALE_Y * tileSizeY), // top left
        (SCALE_X * tileSizeX), (SCALE_Y * tileSizeY), // top right
        (SCALE_X * tileSizeX), 0.f, // bottom right
        0.f, 0.f, // bottom left

        // Texcoords
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements),
                 elements, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    Util::resetGlState();
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

    // TODO: load sprites into textures
    // load sprites into textured
    // use glTexSubImage2d to only use a portion of the texture

    //printf("platforms is populated. size: %d\n", platforms.size());
    //render();
    return true;
}



GLuint
Level::loadTexture(const std::string filename)
{

}


void
Level::print()
{
    printf("----- Level Info:\n");
    printf("width: %d, height: %d\n", mapWidth, mapHeight);
    printf("tilewidth: %d, tileheight: %d\n", tileWidth, tileHeight);

    // ----- print out the level ----- //
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


void
Level::render()
{
    // only create one shader, we will reuse it
    // textures, ideally will be atlased
    // for now we can just upload the individual textures
    std::vector<int>::iterator p;
    int i, row, col;
    glm::mat4 trans;

    //printf("tilesizeX is : %f\n");
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_2D, tex);

    // TODO: start row at 1, remove (row+1) in glm::vec3 call
    row = 0;
    col = 0;
    for (p = platforms.begin(), i = 1; p != platforms.end(); ++p, ++i) {
        //printf("row and col are: %d, %d\n", row, col);

        // if (*p > 0) {
        //    useTexture(*p)
        if (*p == 8) {
            glm::mat4 trans;
            trans = glm::translate(trans,
                                   //glm::vec3((SCALE_X * (10 * col)),
                                   //      (SCALE_Y * (10 * row)),
                                   glm::vec3((SCALE_X * ((float) - SCREEN_X + (tileSizeX * col))),
                                             (SCALE_Y * ((float) SCREEN_Y - (tileSizeY * (row + 1)))),
                                             1.0f));

            uniTrans = glGetUniformLocation(shaderProgram, "trans");
            glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        if (col < (TILES_ON_SCREEN_X - 1)) {
            ++col;
        }
        else {
            col = 0;
            ++row;
        }
    }

    Util::resetGlState();
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
