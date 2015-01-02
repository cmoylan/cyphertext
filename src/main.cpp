#include "OpenGL.h"
#include "Character.h"
#include "Enemy.h"
#include "BulletRegistry.h"
#include "CollisionManager.h"
#include "EnemyAI.h"

bool quit = false;

// global openGL objects
SDL_Window *window;
SDL_GLContext context;

Character *character;
Enemy *enemy;
CollisionManager *collisionManager;
EnemyAI *enemyAI; // enemy AI manager


void initAI();
void initEntities();
void initGraphics();
void update(int ticks);
void handleKeys();
void render();


int
main(int argc, char *args[])
{
  Uint32 startTime;
  int ticks = 0;

  initGraphics();
  initEntities();
  initAI();

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
    handleKeys();
    render();

    SDL_GL_SwapWindow(window);

    if (1000 / FPS > SDL_GetTicks() - startTime) {
      SDL_Delay(1000 / FPS - (SDL_GetTicks() - startTime));
    }

    ticks += 1;
  }

  SDL_GL_DeleteContext(context);
  SDL_Quit();

  return 0;
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
  Coordinate position = { 0, 70 };
  enemy = new Enemy(position);

  character = new Character("res/spaceship.png", position);
  character->center();
}


void
initGraphics()
{
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  window = SDL_CreateWindow("OpenGL", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
  context = SDL_GL_CreateContext(window);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  glewInit();
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
  SDL_Event windowEvent;

  if (SDL_PollEvent(&windowEvent)) {
    if (windowEvent.type == SDL_QUIT) {
      quit = true;
    }

    if (windowEvent.type == SDL_KEYUP) {
      // TODO: this won't really work if the user holds the key down
      // TODO: allow keys to be remapped
      switch (windowEvent.key.keysym.sym) {

      case SDLK_ESCAPE:
        quit = true;
        break;

      case SDLK_w:
        character->move('u');
        break;

      case SDLK_a:
        character->move('l');
        break;

      case SDLK_s:
        character->move('d');
        break;

      case SDLK_d:
        character->move('r');
        break;

      case SDLK_SPACE:
        character->fire();
        break;
      }
    }
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
