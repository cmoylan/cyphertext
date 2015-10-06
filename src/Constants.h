#pragma once

#include "OpenGL.h"

// window
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

// screen constants
#define SCREEN_X 100
#define SCREEN_Y 100
// TODO: caculate from screen_x and screen_y
#define SCALE_X 0.01f
#define SCALE_Y 0.01f

// video
#define FPS 30

// audio
#define MIXER_FREQ 22050
#define MIXER_CHANNELS 2
#define MIXER_CHUNKSIZE 4096

// movement
#define ENEMY_MOVE_SIZE 5
#define CHARACTER_MOVE_SIZE 5
#define INPUT_WEIGHT_X 1
#define INPUT_WEIGHT_Y 1
#define JUMP_VELOCITY_MAX 10
#define JUMP_DECAY 1
#define FALL_Y_ACCELERATION 1
#define FALL_VELOCITY_MAX 5

// entities
#define BULLET_WIDTH 2
#define BULLET_HEIGHT 2

// level
#define TILES_ON_SCREEN_X 20
#define TILES_ON_SCREEN_Y 20

// game
#define MENU_ITEM_WIDTH 50
#define MENU_ITEM_HEIGHT 20

#define GAME_TITLE "Cyphertext"
#define BASE_SCORE_STRING "Score: "


// TODO: move to a new file separate from constants?
struct Vector2D {
    int x;
    int y;
};

struct Rectangle {
    Vector2D origin;
    Vector2D size;
};

struct Point {
    GLfloat x;
    GLfloat y;
};

//struct LevelVertex {
//    GLfloat x;
//    GLflost y;
//    GLfloat tx;
//    GLfloat ty;
//};

struct TexCoord {
    Point tl;
    Point tr;
    Point bl;
    Point br;
};

//"I never am really satisfied that I understand anything; because, understand it well as I may, my comprehension can only be an infinitesimal fraction of all I want to understand"
