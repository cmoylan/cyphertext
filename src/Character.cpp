#include "Character.h"


Character::Character(Game* game, std::string texture, Vector2D position)
{
    this->game = game;

    size.x = 10;
    size.y = 10;

    origin.x = position.x - (size.x / 2);
    origin.y = position.y - (size.y / 2);

    initGL(texture);
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
    //origin.y += JUMP_VELOCITY_MAX;
    jumping = true;
    jumpVelocity = JUMP_VELOCITY_MAX;
}


void
Character::move(int x, int y)
{
    int newX = origin.x;
    int newY = origin.y;
    int movementSize = CHARACTER_MOVE_SIZE;

    newX += (x * movementSize);
    newY += (y * movementSize);

    // TODO: figure out your coordinate system
    if ((newX >= -SCREEN_X) && ((newX + size.x) <= SCREEN_X)) {
        origin.x = newX;
    }
    if ((newY >= -SCREEN_Y) && ((newY + size.y) <= SCREEN_Y)) {
        origin.y = newY;
    }
    //printf("newx and y are: [%dd, %d]\n", newX, newY);
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

    // fall
    if (origin.y > -SCREEN_Y) {
        falling = true;
        if (fallVelocity < FALL_VELOCITY_MAX) {
            fallVelocity += FALLY_ACCELERATION;
        }
        origin.y -= fallVelocity;
        //printf("origin.y and fallVelocity: %d %d\n", origin.y, fallVelocity);
    }

    // stop falling
    if (falling && origin.y <= -SCREEN_Y) {
        falling = false;
        fallVelocity = 0;
    }

}
