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

    // if the position under the player is blocked, do not fall
    // if the position under the player is not blocked, start falling
    bool canFall = !game->level->isBlocked(origin.y - 1, origin.x,
                                           (origin.x + size.x));

    // fall
    if (canFall) { //(origin.y > -SCREEN_Y) {
        falling = true;
        if (fallVelocity < FALL_VELOCITY_MAX) {
            fallVelocity += FALLY_ACCELERATION;
        }
        origin.y -= fallVelocity;
        //printf("origin.y and fallVelocity: %d %d\n", origin.y, fallVelocity);
    }

    // stop falling
    if (falling && !canFall) { //origin.y <= -SCREEN_Y) {
        falling = false;
        fallVelocity = 0;
        //origin.y = -SCREEN_Y;
    }
}
