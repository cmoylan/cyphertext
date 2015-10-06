#include "Game.h"


//Config* config;
Game* game;

void cleanup();
void debug();


int
main()
{
    //config = Config::getInstance();

    game = new Game();
    game->init();
#   ifdef DEBUG
    debug();
#   endif
    game->run();

    return 0;
}


void debug()
{
    printf("OpenGL version %s\n", glGetString(GL_VERSION));
    printf("GLSL version %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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

