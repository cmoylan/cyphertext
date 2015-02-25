#pragma once

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
#define FALLY_ACCELERATION 1
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

#define GAME_TITLE "Star Commander"
#define BASE_SCORE_STRING "Score: "


typedef struct {
    int x;
    int y;
} Vector2D;

typedef struct {
    Vector2D origin;
    Vector2D size;
} Rectangle;
