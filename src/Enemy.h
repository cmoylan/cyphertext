#ifndef ENEMY_H
#define ENEMY_H

#include <string>

#include "OpenGL.h"
#include "Util.h"
#include "Constants.h"
#include "Entity.h"
#include "BulletRegistry.h"
#include "Sound.h"
#include "Game.h"


class Game;


class Enemy : public Entity {

    Game* game;

public:
    // constructor
    Enemy(Game* game, std::string texture, Vector2D position);

    // destructor
    ~Enemy();

    int edgeBottom();
    int edgeLeft();
    int edgeRight();
    int edgeTop();

    void fire(int x = 0, int y = -1);

    void move(int x, int y);

    virtual void hit();
};

#endif
