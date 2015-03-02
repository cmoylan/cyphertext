#include "Shader.h"

Shader::Shader()
{
    set("glyph", "src/shaders/glyph.v.glsl", "src/shaders/glyph.f.glsl");
    set("square", "src/shaders/square.v.glsl", "src/shaders/square.f.glsl");
    set("texturedSquare", 
	"src/shaders/texturedSquare.v.glsl", 
	"src/shaders/texturedSquare.f.glsl");

}


Shader::~Shader()
{

}


GLuint 
Shader::get(const std::string name)
{
    GLuint program = shaders.find(name);
    
    if (program != shaders.end()) {
	// exists
	return program;
    }
    else {
      // try to set it by infering the file names
      return 0;
    }
}


GLuint 
Shader::set(std::string name, std::string vertexSrc, std::string fragmentSrc)
{ 
    GLuint program;
    
    program = Util::createProgramFromShaders(vertexSrc, fragmentSrc);
    shaders[name] = program;
                            
    return program;
}
