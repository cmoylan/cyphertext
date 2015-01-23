#include "OpenGL.h"
#include <SDL2/SDL_mixer.h>

#include "Character.h"
#include "Enemy.h"
#include "BulletRegistry.h"
#include "CollisionManager.h"
#include "EnemyAI.h"
#include "Sound.h"
#include "Game.h"


bool quit = false;

// global openGL objects
SDL_Window *window;
SDL_GLContext context;

Character *character;
Enemy *enemy;
CollisionManager *collisionManager;
EnemyAI *enemyAI; // enemy AI manager

int playerInputX;
int playerInputY;

void initAI();
void initEntities();
void initSDL();
void initAudio();
void cleanup();
void update(int ticks);
void handleKeys();
void render();
void debug();

void handleInput(SDL_Event* event);
void keyUp(SDL_Event* event);
void keyDown(SDL_Event* event);



int
//main(int argc, char *args[])
main()
{
    Uint32 startTime;
    int ticks = 0;

    initSDL();
    initAudio();
    initEntities();
    initAI();

    Game::getInstance().resetScore();

    // main loop
    // TODO: how to change the framerate?
    while (!quit) {
        // count every 10 frames
        if (ticks > 9) {
            ticks = 0;
        }

        startTime = SDL_GetTicks();


        //printf("ticks:: %d\n", ticks);
        update(ticks);
        //handleKeys();
        render();

        SDL_GL_SwapWindow(window);

	SDL_Event event;
	while (SDL_PollEvent(&event)) {
	    handleInput(&event);
	}
	character->move(playerInputX, playerInputY);

        if (1000 / FPS > SDL_GetTicks() - startTime) {
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - startTime));
        }

        ticks += 1;
    }

    cleanup();

    return 0;
}


void
handleInput(SDL_Event* event)
{
    //switch(event->type) {
    //case SDL_KEYUP:
    //	keyDown(event);
    //	break;
    //case SDL_KEYDOWN:
    //	keyUp(event);
    //	break;
    //}

    if (event->type == SDL_QUIT) {
	quit = true;
    }

    if (event->type == SDL_KEYDOWN) {
	// TODO: allow keys to be remapped
	switch (event->key.keysym.sym) {
	case SDLK_ESCAPE:
	    quit = true;
	    break;
	case SDLK_w:
	    //if (playerInputY < 6)
		playerInputY = 3;
	    break;
	case SDLK_a:
	    //if (playerInputY > -6)
		playerInputX = -3;
	    break;
	case SDLK_s:
	    //if (playerInputY > -6)
		playerInputY = -3;
	    break;
	case SDLK_d:
	    //if (playerInputX < 6)
		playerInputX = 3;
	    break;
	case SDLK_SPACE:
	    character->fire();
	    break;
	}
    }

    if (event->type == SDL_KEYUP) {
	switch(event->key.keysym.sym) {
	case SDLK_w:
	case SDLK_s:
	    playerInputY = 0;
	    break;
	case SDLK_a:
	case SDLK_d:
	    playerInputX = 0;
	    break;
	}
    }
}


void
keyDown(SDL_Event* event)
{

}


void
keyUp(SDL_Event* event)
{
}


void
initAI()
{
    // setup enemy AI
    enemyAI = new EnemyAI();
    enemyAI->registerPlayer(character);
    enemyAI->registerEnemy(enemy);

    collisionManager = new CollisionManager();
    collisionManager->registerEntity(character);
    collisionManager->registerEntity(enemy);
}


void
initEntities()
{
    Coordinate position = { 30, 80 };
    enemy = new Enemy("res/enemy.png", position);

    position.y = -70;
    character = new Character("res/spaceship.png", position);
}


void
initSDL()
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // --- Audio initialization
    // TODO: remove magic numbers
    Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
    //Mix_Volume(0 - 128);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // TODO: remove magic numbers
    window = SDL_CreateWindow("OpenGL", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    context = SDL_GL_CreateContext(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();
}


void
initAudio()
{
    Sound *sound = Sound::getInstance();

    sound->load("laser", "res/laser.wav");
    sound->load("enemy-laser", "res/enemy-laser.wav");
    sound->load("hit", "res/hit.wav");
}


void
cleanup()
{
    Sound::getInstance()->freeAll();
    Mix_CloseAudio();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void
update(int ticks)
{
    //printf("ticking: %d\n", ticks);
    BulletRegistry::getInstance().tick();

    // enemy logic here
    enemyAI->tick(ticks);
    collisionManager->tick();
}


void
handleKeys()
{
    // TODO: still a little weird, fix framerate
    SDL_Event windowEvent;

    if (SDL_PollEvent(&windowEvent)) {
        if (windowEvent.type == SDL_QUIT) {
            quit = true;
        }

        if (windowEvent.type == SDL_KEYDOWN) {
            // TODO: allow keys to be remapped
            switch (windowEvent.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            case SDLK_w:
		playerInputY = 1;
                break;
            case SDLK_a:
		playerInputX = -1;
		break;
            case SDLK_s:
		playerInputY = -1;
                break;
            case SDLK_d:
		playerInputX = 1;
                break;
            case SDLK_SPACE:
                character->fire();
                break;
            }
        }

	if (windowEvent.type == SDL_KEYUP) {
	    switch(windowEvent.key.keysym.sym) {
	    case SDLK_w:
	    case SDLK_s:
		playerInputY = 0;
		break;
	    case SDLK_a:
	    case SDLK_d:
		playerInputX = 0;
		break;
	    }
	}

	character->move(playerInputX, playerInputY);
    }
}


void
render()
{
    // Clear the screen to black
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render each component on the screen
    character->render();
    enemy->render();
    BulletRegistry::getInstance().render();
}


void debug()
{
    SDL_version compile_version;
    const SDL_version *link_version = Mix_Linked_Version();
    SDL_MIXER_VERSION(&compile_version);
    printf("compiled with SDL_mixer version: %d.%d.%d\n",
           compile_version.major,
           compile_version.minor,
           compile_version.patch);
    printf("running with SDL_mixer version: %d.%d.%d\n",
           link_version->major,
           link_version->minor,
           link_version->patch);
}
