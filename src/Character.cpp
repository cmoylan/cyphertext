#include "Character.h"


Character::Character(Game* game, std::string texture, Vector2D position)
{
    this->game = game;

    size.x = 10;
    size.y = 10;

    origin.x = position.x - (size.x / 2);
    origin.y = position.y - (size.y / 2);

    initGL(texture);

    // --- TODO: move to entity
    jumping = false;
    jumpVelocity = 0;
    falling = false;
    fallVelocity = 0;
    // ---
}


Character::~Character()
{
    glDeleteProgram(shaderProgram);
    glDeleteVertexArrays(1, &vao);
}


void
Character::center()
{
    origin.x = -(size.x / 2);
    origin.y = -(size.y / 2);
}


void
Character::fire()
{
    Vector2D heading = { 0, 1 };
    Rectangle element = { origin, size };

    BulletRegistry::getInstance().add(element, heading);
    Sound::getInstance()->play("laser");
}


void
Character::hit()
{
    game->removePoints();
    Sound::getInstance()->play("hit");
}


void
Character::jump()
{
    if (!falling && !jumping) {
        jumping = true;
        jumpVelocity = JUMP_VELOCITY_MAX;
    }
}


void
Character::move(int x, int y)
{
    int newX = origin.x + (x * CHARACTER_MOVE_SIZE);
    int newY = origin.y + (y * CHARACTER_MOVE_SIZE);

    if ((newX >= -SCREEN_X) && ((newX + size.x) <= SCREEN_X)) {
        origin.x = newX;
    }
    if ((newY >= -SCREEN_Y) && ((newY + size.y) <= SCREEN_Y)) {
        origin.y = newY;
    }
    //printf("moving to %d, %d\n", origin.x, origin.y);
}


void
Character::update()
{
    // handle jump first, then fall
    // start with jump velocity,
    // subtract decay
    if (jumping && jumpVelocity > 0) {
        jumpVelocity -= JUMP_DECAY;
        origin.y += jumpVelocity;
        return;
    }
    if (jumping && jumpVelocity == 0) {
        jumping = false;
    }

    // if origin.x and origin.x + size.x
    // if any of that range coincides with the ranges
    // of the platforms, stop falling
    if (game->level->isBlocked(origin.y, origin.x, (origin.x + size.x))) {
        // can't fall, stop falling
        //jumping = false;
        //falling = false;
        //printf("falling check is true for: y: %d, x: %d\n", origin.y, origin.x);
    }
    //continue falling
    else {

        // fall if we're not off screen
        if (origin.y > -SCREEN_Y) {
            falling = true;

            if (fallVelocity < FALL_VELOCITY_MAX) {
                fallVelocity += FALLY_ACCELERATION;
            }
            origin.y -= fallVelocity;
        }

        // stop falling if we are off screen
        // NOTE: won't need this is all levels have floors
        //       or could use this to detect death - if they fell in a hole
        if (falling && origin.y <= -SCREEN_Y) {
            falling = false;
            fallVelocity = 0;
            origin.y = -SCREEN_Y;
        }
    }



}
