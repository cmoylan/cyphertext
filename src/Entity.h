#pragma once

#include "OpenGL.h"
#include "Constants.h"
#include "Shader.h"
#include "Util.h"


class Entity {

public:

    // allow direct access to these...why not
    Vector2D origin;
    Vector2D size;

    virtual void hit() = 0;

    GLuint vao, tex, uniTrans;
    GLuint shaderProgram;

    void initGL(std::string texture);
    void render();

};
