#pragma once

#include <map>
#include <string>

#include "OpenGL.h"
#include "Util.h"

typedef std::map<std::string, GLuint> ShaderList;

class Shader {

    ShaderList shaders;

public:

    static Shader *getInstance()
    {
        static Shader instance;
        return &instance;
    }

    GLuint get(const std::string name);

    void use(const std::string name);

private:

    GLuint set(const std::string name,
               const std::string vertexSrc,
               const std::string fragmentSrc);

    // --- Singleton things
    Shader(); // Don't implement
    ~Shader();
    Shader(Shader const&); // Don't implement
    void operator=(Shader const&); // Don't implement

};
