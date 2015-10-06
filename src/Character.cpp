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
    fallVelocity =
        FALL_Y_ACCELERATION; // technically we're always trying to fall into the ground
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
Character::debug()
{
    printf("--\nCharacter: origin: [%d, %d] Size: [%d, %d]\n--\n", origin.x,
           origin.y, size.x, size.y);
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


// x and y can be -1, 0 or 1
void
Character::move(int x, int y)
{
    // Don't move if no direction information is given
    if (x == 0 && y == 0) {
        return;
    }

    int newX = origin.x + (x * CHARACTER_MOVE_SIZE);
    int newY = origin.y + (y * CHARACTER_MOVE_SIZE);

    // TODO: check if level is blocked at the desired location
    // FIXME: don't need to check if value is 0
    // right now this just prevents the character from wandering off screen
    //if ((newX >= -SCREEN_X) && ((newX + size.x) <= SCREEN_X)) {
    //printf("---");
    if (!game->level->isBlocked(newX, origin.y, size)) {
        origin.x = newX;
    }
    //if ((newY >= -SCREEN_Y) && ((newY + size.y) <= SCREEN_Y)) {
    if (!game->level->isBlocked(origin.x, newY, size)) {
        origin.y = newY;
    }
    //printf("moving to %d, %d\n", origin.x, origin.y);

    // if can move
//     if (level->isBlocked(x+1, y, size) {
//       allow x move to go through
//     }
//     if (level->isBlocked(x, y+1, size) {
//       allow y move to go through
//     }

}


// TODO: this should be part of a physics package
void
Character::update()
{
    // handle jump first, then fall
    // start with jump velocity,
    // subtract decay
    if (jumping) {
        // handle jump
        if (jumpVelocity > 0) {
            jumpVelocity -= JUMP_DECAY;
            origin.y += jumpVelocity;
            printf("jumping: y is %d\n", origin.y);
            return;
        }
        if (jumpVelocity == 0) {
            jumping = false;
        }
    }
    else {
        // handle fall
        // if the position under the player is blocked, do not fall
        // if the position under the player is not blocked, start falling
        bool canFall = game->level->canFall(origin.y - fallVelocity, origin.x,
                                            (origin.x + size.x));

        // fall
        if (canFall) {
            falling = true;
            if (fallVelocity < FALL_VELOCITY_MAX) {
                fallVelocity += FALL_Y_ACCELERATION;
            }
            origin.y -= fallVelocity;
#           ifdef DEBUG
            printf("falling: y is %d\n", origin.y);
#           endif
        }

        // stop falling
        if (falling && !canFall) {
#           ifdef DEBUG
	    printf("stop falling at y: %d\n", origin.y);
#           endif
            falling = false;
            // try to fall a little bit more
	    // TODO: this doesn't work quite right.
            for (int v = fallVelocity; v >= 0; v--) {
	      //printf("checking %d - %d\n", v, origin.y - v);
                if (game->level->canFall((origin.y - v), origin.x, (origin.x + size.x))) {
#                   ifdef DEBUG
                    printf("could fall a little bit more - v is %d\n", v);
#                   endif
                    origin.y -= (v + 1); // this is bad
                    break;
                }
            }
            fallVelocity = FALL_Y_ACCELERATION; // reset
        }
    }
}
