#pragma once

#include <string>

#include "OpenGL.h"
#include "Util.h"
#include "Constants.h"
#include "Entity.h"
#include "BulletRegistry.h"
#include "Sound.h"
#include "Game.h"


class Game;


class Character : public Entity {

    Game* game;

public:
    // --- TODO: move to entity
    bool jumping; // = false;
    int jumpVelocity; // = 0;
    bool falling; // = false;
    int fallVelocity; // = 0;
    void update();
    // ---

    // constructor
    Character(Game* game, std::string texture, Vector2D potision);

    // destructor
    ~Character();

    void center();

    void fire();

    void jump();

    void move(int x, int y);

    virtual void hit();

};
