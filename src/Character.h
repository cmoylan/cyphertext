#ifndef CHARACTER_H
#define CHARACTER_H

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGL.h"
#include "Util.h"
#include "Constants.h"
#include "BulletRegistry.h"


class Character {

  // opengl stuff
  // TODO: rename most of these, they are awful
  GLuint vao, vbo, ebo, tex, uniTrans;
  GLuint shaderProgram;

  Vector2D origin;
  Vector2D size;

  void initGL(std::string texture);

public:
  // the location of the character on the screen
  coordinate_t screenPos;

  // size of the actual character
  int width;
  int height;


  // constructor
  Character(std::string texture, Coordinate startingPos);

  // destructor
  ~Character();

  void center();

  void fire();

  void move(unsigned char direction);

  void render();

};

#endif
