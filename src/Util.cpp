#include "Util.h"


void
Util::createAndBindContext(GLuint* vao)
{
    // TODO: error handling
    GLuint vbo, ebo;

    glGenVertexArrays(1, vao);
    glBindVertexArray(*vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
}


GLuint
Util::createProgram(const std::vector<GLuint>& shaderList)
{
    size_t i;
    GLuint program;
    GLint status;

    program = glCreateProgram();

    // TODO: rewrite to use vector iterator?
    for (i = 0; i < shaderList.size(); i++) {
        glAttachShader(program, shaderList.at(i));
    }

    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        printf("ERROR: error linking shader program\n");
    }

    for (i = 0; i < shaderList.size(); i++) {
        glDetachShader(program, shaderList.at(i));
    }

    return program;
}


GLuint
Util::createProgramFromShaders(std::string vertexFile,
                               std::string fragmentFile)
{
    GLuint program = glCreateProgram();
    GLuint shader;
    GLint linkOk = GL_FALSE;

    if (!vertexFile.empty()) {
        shader = createShader(GL_VERTEX_SHADER, vertexFile);
        if (!shader) {
            printf("shader didn't compile!\n");
            return 0;
        }
        else {
            glAttachShader(program, shader);
        }
    }

    if (!fragmentFile.empty()) {
        shader = createShader(GL_FRAGMENT_SHADER, fragmentFile);
        if (!shader) {
            printf("shader didn't compile!\n");
            return 0;
        }
        else {
            glAttachShader(program, shader);
        }
    }

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linkOk);
    if (!linkOk) {
        printf("program did not link\n");
        glDeleteProgram(program);
    }

    return program;
}


GLuint
Util::createShader(GLenum shaderType, const std::string& shaderFile)
{
    GLint status;
    std::string shaderSource;

    shaderSource = loadStringFromFile(shaderFile.c_str());

    GLuint shader = glCreateShader(shaderType);

    const char *strFileData = shaderSource.c_str();

    glShaderSource(shader, 1, &strFileData, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        printf("ERROR: unable to compile shader: %s\n", shaderSource.c_str());

        //GLchar *strInfoLog = newGLchar[infoLogLength + 1];
        //glGetShaderInfoLog(shader, strInfoLog,  infoLogLength);
        //
        //// TODO: log to stderror
        //printf("shader did not compile: %s - %s\n", shaderType, strInfoLog);
        //
        //delete[] strInfoLog;
    }

    return shader;
}


std::string
Util::loadStringFromFile(const std::string& filename)
{
    std::stringstream sstr;
    std::ifstream file;

    file.open(filename.c_str(), std::ifstream::in);

    if (file.is_open()) {
        sstr << file.rdbuf();
        return sstr.str();
    }
    else {
        printf("ERROR: unable to open file %s\n", filename.c_str());
        return "";
    }
}


void
//#args: buffer, filename
Util::loadTexture(GLuint buffer, const std::string& filename)
{
    int width, height;
    unsigned char* image;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, buffer);

    image = SOIL_load_image(filename.c_str(),
                            &width,
                            &height,
                            0,
                            SOIL_LOAD_RGBA);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, image);

    glGenerateMipmap(GL_TEXTURE_2D);

    // TODO: change to gl_nearest_mipmap_nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);

    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0);
}


void
Util::mapPositionAttribute(GLuint program, const std::string name)
{
    // TODO: error handing and return bool
    GLint posAttrib = glGetAttribLocation(program, name.c_str());
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(posAttrib);
}


void
Util::mapTextureAttribute(GLuint program, const std::string name)
{
    // TODO: error handling and return bool
    // texture position
    GLint texAttrib = glGetAttribLocation(program, name.c_str());
    glEnableVertexAttribArray(texAttrib);

    // the texcoords are tightly packed after the verticies in the array
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 0,
                          (void*)(8 * sizeof(float)));
}


void
Util::resetGlState()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}