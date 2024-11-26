#include <SDL2/SDL.h>
#include <stdio.h>
#include <game.h>
#include <player.h>


int main(int arg, char **argv)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        SDL_Log("Unable to Initialize SDL: %s",SDL_GetError());
        return 0;
    }

    SDL_DisplayMode DisplayMode;
    if(SDL_GetDesktopDisplayMode(0,&DisplayMode)<0)
    {
        printf("Error getting desktop resolution\n %s",SDL_GetError());
        return 0;
    }

    Game Game;
    Game.rect = (SDL_Rect){0,0,1960,1080};
    Game.window = SDL_CreateWindow("AI_Project",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            1920, 1080,
                                            SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
    /*Game.window = SDL_CreateWindow("AI_Project",
                                            SDL_WINDOWPOS_CENTERED,
                                            SDL_WINDOWPOS_CENTERED,
                                            800, 600,
                                            SDL_WINDOW_SHOWN); */
                                        
    if(!Game.window)
    {
        SDL_Log("Failed to create window: %s",SDL_GetError());
        SDL_Quit();
        return 1;
    }

    Game.renderer = SDL_CreateRenderer(Game.window, -1, SDL_RENDERER_ACCELERATED);
    if(!Game.renderer)
    {
        SDL_Log("Failed to create renderer: %s",SDL_GetError());
        SDL_DestroyWindow(Game.window);
        SDL_Quit();
        return 1;
    }

    Player *player = CreatePlayer(&Game, 960,540);

    runGameLoop( &Game, player);

    KillPlayer(player);
    SDL_DestroyRenderer(Game.renderer);
    SDL_DestroyWindow(Game.window);
    SDL_Quit();

    return 1;
}
