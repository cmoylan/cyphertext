#include "Shader.h"

Shader::Shader()
{
    // set up initial shaders
    set("glyph", "src/shaders/glyph.v.glsl", "src/shaders/glyph.f.glsl");
    set("square", "src/shaders/square.v.glsl", "src/shaders/square.f.glsl");
    set("texturedSquare",
        "src/shaders/texturedSquare.v.glsl",
        "src/shaders/texturedSquare.f.glsl");
}


Shader::~Shader()
{
    ShaderList::iterator program;

    for (program = shaders.begin(); program != shaders.end(); ++program) {
        //printf("deleting shader prog id: %d\n", program->second);
        glDeleteProgram(program->second);
    }
    shaders.clear();
}


GLuint
Shader::get(const std::string name)
{
    ShaderList::iterator program;

    program = shaders.find(name);

    if (program != shaders.end()) {
        return program->second;
    }
    else {
        // try to set it by infering the file names
        return (GLuint) 0;
    }
}


GLuint
Shader::set(const std::string name,
            const std::string vertexSrc,
            const std::string fragmentSrc)
{
    GLuint program;

    program = Util::createProgramFromShaders(vertexSrc, fragmentSrc);
    shaders[name] = program;

    return program;
}


void
Shader::use(const std::string name)
{
    GLuint program = get(name);

    if (program != 0) {
        glUseProgram(program);
    }

}
