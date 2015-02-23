#include "Game.h"

void
Game::cleanup()
{
    Sound::getInstance()->freeAll();
    Mix_CloseAudio();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void
Game::init()
{
    initSDL();
    initAudio();
    initEntities();
    initAI();

    menu = new Menu();
}


void
Game::initAI()
{
    // setup enemy AI
    enemyAI = new EnemyAI();
    //enemyAI->registerPlayer(character);
    //enemyAI->registerEnemy(enemy);

    collisionManager = new CollisionManager();
    collisionManager->registerEntity(character);
    collisionManager->registerEntity(enemy);
}


void
Game::initAudio()
{
    Sound *sound = Sound::getInstance();

    sound->load("laser", "res/laser.wav");
    sound->load("enemy-laser", "res/enemy-laser.wav");
    sound->load("hit", "res/hit.wav");
}


void
Game::initEntities()
{
    background = new Background("res/background.png");

    Coordinate position = { 30, 80 };
    this->enemy = new Enemy(this, "res/enemy.png", position);

    position.y = -70;
    this->character = new Character(this, "res/spaceship.png", position);
}


void
Game::initSDL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // --- Audio initialization
    Mix_OpenAudio(MIXER_FREQ, MIX_DEFAULT_FORMAT, MIXER_CHANNELS, MIXER_CHUNKSIZE);
    //Mix_Volume(-1, 0..128);
    Mix_Volume(-1, 64);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    window = SDL_CreateWindow(GAME_TITLE, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH,
                              WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    // enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
}
