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

    void debug();

    /**
     * Move the character in a direction in 2-dimensional space
     * @param x the direction (-1, 0, 1) to move along the x axis
     * @param y the direction (-1, 0, 1) to move along the y axis
     *
     * Takes a 2D direction and attempts to move the character accordingly.
     * This will interogate the current level to determine if the
     * desired new location is clear and only move if it is.
     */
    void move(int x, int y);

    virtual void hit();

};
